/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* win_main.cc
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
  currentTask = 0;

  showed = false;
  started = false;

  configs.load();

  // get widgets
  m_refGlade->get_widget("lblDisplay", lblDisplay);
  m_refGlade->get_widget("lblCycle", lblCycle);
  m_refGlade->get_widget("lblTaskTitle", lblTaskTitle);
  m_refGlade->get_widget("lblPomodoros", lblPomodoros);
  m_refGlade->get_widget("lblNotification", lblNotification);

  m_refGlade->get_widget("frmWorkOn", frmWorkOn);

  m_refGlade->get_widget("btnStart", btnStart);
  m_refGlade->get_widget("btnRestart", btnRestart);
  m_refGlade->get_widget("btnAddTask", btnAddTask);
  m_refGlade->get_widget("btnDelTask", btnDelTask);
  m_refGlade->get_widget("btnFinish", btnFinish);
  m_refGlade->get_widget("btnCancelTask", btnCancelTask);
  
  m_refGlade->get_widget("trvTasks", trvTasks);
  m_refGlade->get_widget("cmbLists", cmbLists);

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

  // inactive_timer
  inactive_timer = sigc::bind(sigc::mem_fun(*this,
              &WinMain::on_inactive_timeout), 0);

  check_inactive = Glib::signal_timeout().
    connect(inactive_timer, atoi(configs.inactive_interval.c_str()) * MINUTE_IN_SECONDS);

  // connect signals
  btnStart->signal_clicked().
            connect(sigc::mem_fun(*this, &WinMain::on_button_start_clicked));

  btnRestart->signal_clicked().
            connect(sigc::mem_fun(*this, &WinMain::on_button_restart_clicked));

  btnFinish->signal_clicked().
            connect(sigc::mem_fun(*this, &WinMain::on_button_finish_clicked));

  btnCancelTask->signal_clicked().
            connect(sigc::mem_fun(*this, &WinMain::on_button_cancel_clicked));

  btnAddTask->signal_clicked().
              connect(sigc::mem_fun(*this, &WinMain::on_menu_file_new_task));

  btnDelTask->signal_clicked().
            connect(sigc::mem_fun(*this, &WinMain::on_button_del_task_clicked));

  trvTasks->signal_row_activated().
            connect(sigc::mem_fun(*this, &WinMain::on_treeview_tasks_row_activated));

  trvTasks->signal_cursor_changed().
            connect(sigc::mem_fun(*this, &WinMain::on_cursor_changed));

  cmbLists->signal_changed().
    connect(sigc::mem_fun(*this, &WinMain::on_list_changed));

  // connect menu
  mnuNew->signal_activate().
            connect(sigc::mem_fun(*this, &WinMain::on_menu_file_new_task));
  mnuQuit->signal_activate().
            connect(sigc::mem_fun(*this, &WinMain::on_menu_file_quit));
  mnuPreferences->signal_activate().
            connect(sigc::mem_fun(*this, &WinMain::on_menu_edit_preferences));
  mnuAbout->signal_activate().
            connect(sigc::mem_fun(*this, &WinMain::on_menu_help_about));

  load_lists();
  load_tasks();

	show_all_children();
  hide_task_buttons();
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

void WinMain::generate_pomodoros()
{
  std::string pomodoros = "";
  int i;

  if(currentTask){
    for(i = currentTask->get_pomodoros(); i >= 1; i--)
      pomodoros.append("X ");
  }
  lblPomodoros->set_text(pomodoros);
}

void WinMain::load_lists()
{
  Glib::ustring previous = cmbLists->get_active_text();
  cmbLists->remove_all();

  lists = TaskList::all();
  while(!lists.empty()){
    cmbLists->append(lists.front()->get_name());
    lists.pop_front();
  }

  if(!previous.empty()) cmbLists->set_active_text(previous);
  if(cmbLists->get_active_text().empty()) cmbLists->set_active(0);
  
}

void WinMain::load_tasks()
{
  std::list<Task*> tasks;
  Gtk::TreeModel::iterator iter = cmbLists->get_active();

  if(iter) {
    Gtk::TreeModel::Row row = *iter;
    if(row) {
      tasks = Task::all(cmbLists->get_active_text().c_str());
    }
  }

  if(tasks.empty()) tasks = Task::all();
  
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

void WinMain::notify(const char *message)
{
  NotifyNotification *notMessage;
  GError *error = NULL;

  notMessage = notify_notification_new("Gnomato",
                                       message,
                                       GNOMATO_DATADIR "/tomato.png");
  
  notify_notification_set_timeout(notMessage,3000);

  char category[30] = "Gnomato Notifications";
  notify_notification_set_category(notMessage,category);

  notify_notification_set_urgency(notMessage,NOTIFY_URGENCY_CRITICAL);

  notify_notification_show(notMessage,&error);
}

void WinMain::force_show()
{
  showed = true;
  show();
}

Task* WinMain::get_current_task()
{
  std::vector<Gtk::TreePath> paths = trvTasks->get_selection()->get_selected_rows();
  if(paths.size()){
    Gtk::TreeRow row = Gtk::TreeRow(*treTasks->get_iter(*paths.begin()));

    return(new Task((Glib::ustring)row[mdlColumn.id]));
  }
  return NULL; 
}

void WinMain::inc_current_task()
{
  if(currentTask) {
    currentTask->set_pomodoros(currentTask->get_pomodoros()+1);
    currentTask->save();

    generate_pomodoros();
  }
}

void WinMain::set_notification(string notification)
{

  if(!notification.empty()) {
    string timestamp = to_simple_string(second_clock::local_time());

    string status = "[";
    status.append(timestamp);
    status.append("] - ");
    status.append(notification);

    lblNotification->set_text(status);
  }
}

void WinMain::show_task_buttons() 
{
  btnFinish->show();
  btnCancelTask->show();
}

void WinMain::hide_task_buttons()
{
  btnFinish->hide();
  btnCancelTask->hide();
  lblTaskTitle->set_text("are you not doing anything?");
}

void WinMain::execute(string script)
{
  if(currentTask) {
    Glib::Threads::Thread::create(
      sigc::bind(sigc::mem_fun(this, &WinMain::run_python_script), script));
  }
}

void WinMain::run_python_script(string script)
{
  PythonExecutor *pe = new PythonExecutor();
  pe->set_script(script);
  pe->execute(cmbLists->get_active_text(), currentTask->get_name());
  set_notification(pe->get_result_as_string());
  if(pe != NULL) delete(pe);
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
    timeout.disconnect();

    execute("on_pause.py");

  }else{
    started = true;
    btnStart->set_label(_("Pause"));
    timeout = Glib::signal_timeout().connect(timer, 1000);

    execute("on_start.py");

  }
}

void WinMain::on_button_restart_clicked()
{
  cycle_number = 0;
  time_elapsed = atoi(configs.work_interval.c_str())*60;
  
  lblCycle->set_markup(generate_cycle());
	lblDisplay->set_text(generate_display());
}

void WinMain::on_button_finish_clicked()
{
  if(currentTask){

    execute("on_finish.py");

    currentTask->finish();

    load_lists();
    load_tasks();
    hide_task_buttons();

    currentTask = 0;
  }

}

void WinMain::on_button_cancel_clicked()
{
  if(currentTask){
    execute("on_cancel_task.py");
    hide_task_buttons();
    currentTask = 0;
  }

}
void WinMain::on_treeview_tasks_row_activated(const TreeModel::Path& path,
                                              TreeViewColumn* column)
{
  Gtk::TreeModel::iterator iter = treTasks->get_iter(path);
  if(iter){
    Gtk::TreeModel::Row row = *iter;
    DialogTask *dlgTask = 0;
  
    m_refGlade->get_widget_derived("DialogTask",dlgTask);
    dlgTask->set_id((Glib::ustring)row[mdlColumn.id]);
    dlgTask->run();
  
    load_lists();
    load_tasks();
  
    hide_task_buttons();
  }
}

void WinMain::on_button_del_task_clicked()
{
  if(currentTask){
    currentTask->destroy();

    load_lists();
    load_tasks();
    hide_task_buttons();

    currentTask = NULL;
  }
}

bool WinMain::on_timeout(int timer_number)
{

	if(!time_elapsed){
	  cycle_number++;

    std::cout << "cycle_number: " << cycle_number << std::endl;
	  if(cycle_number % 2){
      
      execute("on_break.py");

      notify(_("Take a break"));
      inc_current_task();
	    if(cycle_number == 7)
        time_elapsed = atoi(configs.long_interval.c_str()) * 60;
      else
        time_elapsed = atoi(configs.break_interval.c_str()) * 60;
    }
    else{

      execute("on_work.py");

      notify(_("End of break"));
	    time_elapsed = atoi(configs.work_interval.c_str()) * 60;
    }

    lblCycle->set_markup(generate_cycle());

	  if(cycle_number == 7) cycle_number = -1;
  
  }

	lblDisplay->set_text(generate_display());
  --time_elapsed;
 
  timeout.disconnect();
  timeout = Glib::signal_timeout().connect(timer, 1000);

}

bool WinMain::on_inactive_timeout(int timer_number)
{
  if(!(currentTask && started)) {
    notify(_("are you not doing anything?"));
  }

  check_inactive.disconnect();
  check_inactive = Glib::signal_timeout().
    connect(inactive_timer, atoi(configs.inactive_interval.c_str()) * MINUTE_IN_SECONDS);
}

void WinMain::on_cursor_changed()
{

  currentTask = get_current_task();
  if(currentTask){
    lblTaskTitle->set_text(currentTask->get_name());
    generate_pomodoros();

    show_task_buttons();

    execute("on_change_task.py");
  }
}

void WinMain::on_list_changed()
{
  load_tasks();
}

// callbacks implementations - menu
void WinMain::on_menu_file_new_task()
{
  DialogTask *dlgTask = 0;

  m_refGlade->get_widget_derived("DialogTask",dlgTask);
  dlgTask->set_list(cmbLists->get_active_text());
  dlgTask->run();

  load_lists();
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
  abtDialog->hide();
}

Glib::ustring WinMain::get_current_time()
{
  return generate_display();
}

Glib::ustring WinMain::get_current_task_title()
{
  if(currentTask != 0) {
    std::stringstream taskName;

    taskName << currentTask->get_name(); 
    taskName << " ( ";
    taskName << currentTask->get_pomodoros();
    taskName << " )";

    return taskName.str();
  } else
    return "";
}

Glib::ustring WinMain::get_cycle()
{
  return generate_cycle();
}

