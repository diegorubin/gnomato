/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * win_main.h
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

#ifndef _WIN_MAIN_H_
#define _WIN_MAIN_H_

#include <glibmm/i18n.h>
#include <gtkmm.h>
#include <iostream>
#include <libnotify/notify.h>
#include <sstream>

#include "boost/date_time/posix_time/posix_time.hpp"

#include "config.h"
#include "dialog_preferences.h"
#include "dialog_task.h"
#include "dialog_work_log_entries.h"
#include "python_executor.h"
#include "sound_player.h"
#include "task.h"
#include "task_list.h"
#include "utils.h"

using namespace Gtk;
using namespace boost::posix_time;

const int MINUTE_IN_SECONDS = 60000;

class WinMain : public Gtk::ShortcutsWindow {
public:
  WinMain(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
  virtual ~WinMain();

  void on_menu_file_new_task();
  void force_show();
  void load_lists();
  void load_tasks();

  void set_green_icon();
  void set_red_icon();
  void set_gray_icon();

  Glib::ustring get_current_time();
  Glib::ustring get_current_task_title();
  Glib::ustring get_cycle();

private:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord {
  public:
    ModelColumns() {
      add(id);
      add(title);
    }
    Gtk::TreeModelColumn<Glib::ustring> id;
    Gtk::TreeModelColumn<Glib::ustring> title;
  };

  class TasksView : public Gtk::TreeView {

  public:
    TasksView(BaseObjectType *cobject,
              const Glib::RefPtr<Gtk::Builder> &refGlade);

    void set_win_main_ref(WinMain *win_main);
    Glib::RefPtr<Gtk::Builder> m_refGlade;
    virtual void on_drag_end(const Glib::RefPtr<Gdk::DragContext> &context);
    virtual void on_menu_move_task();
    virtual bool on_button_press_event(GdkEventButton *ev);

    WinMain *win_main;
    Gtk::Menu menu;
  };

  // attributes
  bool showed;
  bool started;
  unsigned int time_elapsed;
  unsigned int minutes, seconds;
  int cycle_number;

  Config configs;

  Task *currentTask;
  std::list<TaskList *> lists;

  // For pomodoro
  sigc::slot<bool> timer;
  sigc::connection timeout;

  // For inactive
  sigc::slot<bool> inactive_timer;
  sigc::connection check_inactive;

  Glib::RefPtr<Gtk::Builder> m_refGlade;

  // widgets
  Button *btnStart;
  Button *btnRestart;
  Button *btnAddTask;
  Button *btnDelTask;
  Button *btnFinish;
  Button *btnCancelTask;
  Button *btnClearList;

  ImageMenuItem *mnuNew;
  ImageMenuItem *mnuQuit;
  ImageMenuItem *mnuPreferences;
  ImageMenuItem *mnuWorkLogEntries;
  ImageMenuItem *mnuAbout;

  Label *lblDisplay;
  Label *lblCycle;
  Label *lblTaskTitle;
  Label *lblPomodoros;
  Label *lblNotification;

  Switch *swtFinishedTasks;

  Entry *entFilter;

  Frame *frmWorkOn;

  TasksView *trvTasks;
  ModelColumns mdlColumn;

  ComboBoxText *cmbLists;

  Glib::RefPtr<StatusIcon> systray;
  Glib::RefPtr<Gtk::UIManager> mnuSystray;

  Glib::RefPtr<ActionGroup> actMenu;
  Glib::RefPtr<Action> mnuIniciar;
  Glib::RefPtr<Gtk::ListStore> treTasks;

  // dialogs
  DialogTask *dlgTask;
  AboutDialog *abtDialog;

  // methods
  std::string generate_display();
  std::string generate_cycle();
  void generate_pomodoros();
  void show_task();
  void log_work(string hook);

  void notify(const char *icon, const char *message);
  void notify(const char *message);
  void notify_with_green_icon(const char *message);
  void notify_with_gray_icon(const char *message);

  Task *get_current_task();
  void inc_current_task();
  void execute(string script);
  void run_python_script(string script);
  void set_notification(string notification);
  Glib::ustring get_current_list();
  void update_positions();
  void move_task(string list);
  void set_systray();
  void configure_interface();

  void lock();
  void unlock();
  void show_task_buttons();
  void hide_task_buttons();

  // callback methods
  virtual void on_filter_changed();
  virtual void on_systray_activated();
  virtual void on_systray_popup(guint button, guint activate_time);
  virtual void on_button_start_clicked();
  virtual void on_button_restart_clicked();
  virtual void on_button_finish_clicked();
  virtual void on_button_cancel_clicked();
  virtual void on_button_clear_list_clicked();
  virtual void on_treeview_tasks_row_activated(const TreeModel::Path &path,
                                               TreeViewColumn *column);
  virtual void on_button_del_task_clicked();
  virtual bool on_timeout(int timer_number);
  virtual bool on_inactive_timeout(int timer_number);
  virtual void on_cursor_changed();
  virtual void on_list_changed();
  virtual bool on_display_finished_changed(bool changed);

  // callback methods - menu
  virtual void on_menu_file_quit();
  virtual void on_menu_edit_preferences();
  virtual void on_menu_edit_work_log_entries();
  virtual void on_menu_help_about();

  // callback methods - window
  virtual void on_resize();
};

#endif //__WIN_MAIN_H_
