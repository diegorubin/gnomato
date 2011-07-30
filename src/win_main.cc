/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* main.cc
* Copyright (C) Diego Rubin 2011 <rubin.diego@gmail.com>
*
* Gnomato is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Gnomato is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Author: Diego Rubin <rubin.diego@gmail.com>
*
*/

#include "win_main.h"

WinMain::WinMain(BaseObjectType* cobject, 
                 const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Window(cobject),
  m_refGlade(refGlade),

  lblDisplay(0)
  
{
  showed = false;
  started = false;

  configs.load();

  // get widgets
  m_refGlade->get_widget("lblDisplay", lblDisplay);
  m_refGlade->get_widget("lblCycle", lblCycle);

  m_refGlade->get_widget("btnStart", btnStart);
  m_refGlade->get_widget("btnRestart", btnRestart);
  m_refGlade->get_widget("btnAddTask", btnAddTask);
  m_refGlade->get_widget("btnDelTask", btnDelTask);
  
  m_refGlade->get_widget("trvTasks", trvTasks);

  m_refGlade->get_widget("mnuNew", mnuNew);
  m_refGlade->get_widget("mnuQuit", mnuQuit);
  m_refGlade->get_widget("mnuPreferences", mnuPreferences);
  m_refGlade->get_widget("mnuAbout", mnuAbout);

  treTasks = Gtk::ListStore::create(mdlColumn);
  trvTasks->set_model(treTasks);

  on_button_restart_clicked();

  // timer
  timer = sigc::bind(sigc::mem_fun(*this,
              &WinMain::on_timeout), 0);

  // connect signals
  btnStart->signal_clicked().
            connect(sigc::mem_fun(*this, &WinMain::on_button_start_clicked));

  btnRestart->signal_clicked().
            connect(sigc::mem_fun(*this, &WinMain::on_button_restart_clicked));

  btnAddTask->signal_clicked().
              connect(sigc::mem_fun(*this, &WinMain::on_menu_file_new_task));

  btnAddTask->signal_clicked().
            connect(sigc::mem_fun(*this, &WinMain::on_button_del_task_clicked));

  trvTasks->signal_row_activated().
            connect(sigc::mem_fun(*this, &WinMain::on_treeview_tasks_row_activated));

  // connect menu
  mnuNew->signal_activate().
            connect(sigc::mem_fun(*this, &WinMain::on_menu_file_new_task));
  mnuQuit->signal_activate().
            connect(sigc::mem_fun(*this, &WinMain::on_menu_file_quit));
  mnuPreferences->signal_activate().
            connect(sigc::mem_fun(*this, &WinMain::on_menu_edit_preferences));
  mnuAbout->signal_activate().
            connect(sigc::mem_fun(*this, &WinMain::on_menu_help_about));

  load_tasks();

	show_all_children();
}

WinMain::~WinMain()
{
}

void WinMain::set_systray(Glib::RefPtr<StatusIcon> tray)
{
  systray = tray;

  //criacao do menu para o systray
	actMenu = Gtk::ActionGroup::create();
  
	actMenu->add(Gtk::Action::create("Start",_("Start/Pause")),
	             sigc::mem_fun(*this, &WinMain::on_button_start_clicked));
	actMenu->add(Gtk::Action::create("Quit",Gtk::Stock::QUIT),
	             sigc::mem_fun(*this, &WinMain::on_menu_file_quit));
	
	mnuSystray = Gtk::UIManager::create();
	mnuSystray->insert_action_group(actMenu);
	
	Glib::ustring ui_info =
    "<ui>"
    "  <popup name='Popup'>"
    "    <menuitem action='Start' />"
    "    <separator/>"
    "    <menuitem action='Quit' />"
    "  </popup>"
    "</ui>";	

  mnuSystray->add_ui_from_string(ui_info);

  systray->set_tooltip_text("Gnomato");
 
  systray->signal_popup_menu().
           connect(sigc::mem_fun(*this,&WinMain::on_systray_popup));
  
  systray->signal_activate().
           connect(sigc::mem_fun(*this,&WinMain::on_systray_activated));

}

// methods implementations
std::string WinMain::generate_display()
{
  std::stringstream display;
  std::string attributes;
  
  minutes = time_elapsed / 60;
  seconds = time_elapsed % 60;

  if(minutes < 10) display << "0";
  display << minutes;

  display << ":";

  if(seconds < 10) display << "0";
  display << seconds;

  return display.str();
}

std::string WinMain::generate_cycle()
{
  std::string cycle;
  int i;

  cycle = "";
  for(i = 0; i <= 3; i++){
    if((cycle_number/2) == i){
      if(cycle_number % 2){ //break
        cycle.append(_("W -"));
        if(i == 3) cycle.append(_("<span color=\"red\"><b>LB</b></span>"));
        else cycle.append(_("<span color=\"red\"><b>B</b></span> -"));
      }else{//pomodoro
        cycle.append(_("<span color=\"green\"><b>W</b></span> -"));
        if(i == 3) cycle.append(_("LB"));
        else cycle.append(_("B -"));
      }
    }else{
      cycle.append(_("W -"));
      if(i == 3) cycle.append(_("LB"));
      else cycle.append(_("B -"));
    }
  }

  return cycle;
}

void WinMain::load_tasks()
{
  std::list<Task*> tasks = Task::all();
  treTasks->clear();
  trvTasks->remove_all_columns();

  while(!tasks.empty()){
    Gtk::TreeModel::Row row = *(treTasks->append());

    row[mdlColumn.id] = tasks.front()->get_id();
    row[mdlColumn.title] = tasks.front()->get_name();

    tasks.pop_front();
  }

  trvTasks->append_column("ID", mdlColumn.id);
  trvTasks->append_column(_("Title"), mdlColumn.title);
}

// callbacks implementations
void WinMain::on_systray_activated()
{
  if(showed){
    showed = false;
    hide();
  }else{
    showed = true;
    show();
  }
}

void WinMain::on_systray_popup(guint button, guint activate_time)
{
  Gtk::Menu* pMenu = static_cast<Gtk::Menu*>(mnuSystray->get_widget("/Popup"));
 
  if(pMenu)
    pMenu->popup(button, activate_time);
}

void WinMain::on_button_start_clicked()
{
  if(started){
    started = false;
    btnStart->set_label(_("Start"));
    conn.disconnect();
  }else{
    started = true;
    btnStart->set_label(_("Pause"));
    conn = Glib::signal_timeout().connect(timer, 1000);
  }
}

void WinMain::on_button_restart_clicked()
{
  cycle_number = 0;
  time_elapsed = atoi(configs.work_interval.c_str())*60;
  
  lblCycle->set_markup(generate_cycle());
	lblDisplay->set_text(generate_display());
}

void WinMain::on_treeview_tasks_row_activated(const TreeModel::Path& path,
                                              TreeViewColumn* column)
{
}

void WinMain::on_button_del_task_clicked()
{

}

bool WinMain::on_timeout(int timer_number)
{
	if(!time_elapsed){
	  cycle_number++;

	  if(cycle_number % 2) 
	    if(cycle_number == 7)
        time_elapsed = atoi(configs.long_interval.c_str()) * 60;
      else
        time_elapsed = atoi(configs.break_interval.c_str()) * 60;
    else 
	    time_elapsed = atoi(configs.work_interval.c_str()) * 60;

    lblCycle->set_markup(generate_cycle());

	  if(cycle_number == 7)
      cycle_number = 0;

    showed=true;
    show();
  }

	lblDisplay->set_text(generate_display());
  --time_elapsed;
 
  conn.disconnect();
  conn = Glib::signal_timeout().connect(timer, 1000);
}

// callbacks implementations - menu
void WinMain::on_menu_file_new_task()
{
  DialogTask *dlgTask = 0;

  m_refGlade->get_widget_derived("DialogTask",dlgTask);
  dlgTask->run();

  load_tasks();
}

void WinMain::on_menu_file_quit()
{
  Main::quit();
}

void WinMain::on_menu_edit_preferences()
{
  DialogPreferences *dlgPreferences = 0;

  m_refGlade->get_widget_derived("DialogPreferences",dlgPreferences);
  dlgPreferences->run();

  configs.load();
}

void WinMain::on_menu_help_about()
{
  m_refGlade->get_widget("winAbout", abtDialog);
  abtDialog->run();
}

