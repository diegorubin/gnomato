/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * dialog_work_log_entries.h
 * Copyright (C) Diego Rubin 2020 <rubin.diego@gmail.com>
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

#ifndef _WIN_WORK_LOG_ENTRIES_H_
#define _WIN_WORK_LOG_ENTRIES_H_

#include <glibmm/i18n.h>
#include <gtkmm.h>

#include "task.h"

using namespace Gtk;

class DialogWorkLogEntries : public Gtk::Dialog {
public:
  DialogWorkLogEntries(BaseObjectType *cobject,
                       const Glib::RefPtr<Gtk::Builder> &refGlade);

  virtual ~DialogWorkLogEntries();

private:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord {
  public:
    ModelColumns() {
      add(task_id);
      add(task_name);
      add(start_date_entry);
      add(start_hour_entry);
      add(end_date_entry);
      add(end_hour_entry);
    }
    Gtk::TreeModelColumn<Glib::ustring> task_id;
    Gtk::TreeModelColumn<Glib::ustring> task_name;
    Gtk::TreeModelColumn<Glib::ustring> start_date_entry;
    Gtk::TreeModelColumn<Glib::ustring> start_hour_entry;
    Gtk::TreeModelColumn<Glib::ustring> end_date_entry;
    Gtk::TreeModelColumn<Glib::ustring> end_hour_entry;
  };

  class WorkLogEntriesView : public Gtk::TreeView {

  public:
    WorkLogEntriesView(BaseObjectType *cobject,
                       const Glib::RefPtr<Gtk::Builder> &refGlade);

    void set_win_main_ref(DialogWorkLogEntries *dlgWorkLogEntries);
    Glib::RefPtr<Gtk::Builder> m_refGlade;

    DialogWorkLogEntries *dlgWorkLogEntries;
    Gtk::Menu menu;
  };

  // widgets
  Button *btnClose;

  ComboBoxText *cmbWorkDays;

  WorkLogEntriesView *trvEntries;
  ModelColumns mdlColumn;

  Glib::RefPtr<Gtk::ListStore> treEntries;

  Glib::RefPtr<Gtk::Builder> m_refGlade;

  // methods
  void load_days();
  void load_entries();
  std::string format_hour(int hour);

  // callback methods
  virtual void on_button_close_clicked();
};

#endif // WIN_WORK_LOG_ENTRIES
