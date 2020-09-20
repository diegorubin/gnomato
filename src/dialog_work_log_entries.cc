/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * dialog_work_log_entries.cc
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

#include "dialog_work_log_entries.h"

DialogWorkLogEntries::DialogWorkLogEntries(
    BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
    : Gtk::Dialog(cobject), m_refGlade(refGlade) {

  m_refGlade->get_widget("cmbWorkDays", cmbWorkDays);
  m_refGlade->get_widget("btnWorkLogClose", btnClose);

  m_refGlade->get_widget_derived("trvWorkLogEntries", trvEntries);

  treEntries = Gtk::ListStore::create(mdlColumn);
  trvEntries->set_model(treEntries);

  // connect signals
  btnClose->signal_clicked().connect(
      sigc::mem_fun(*this, &DialogWorkLogEntries::on_button_close_clicked));
  cmbWorkDays->signal_changed().connect(
      sigc::mem_fun(*this, &DialogWorkLogEntries::on_day_changed));

  load();
  show_all();
}

DialogWorkLogEntries::~DialogWorkLogEntries() {}

DialogWorkLogEntries::WorkLogEntriesView::WorkLogEntriesView(
    BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
    : Gtk::TreeView(cobject), m_refGlade(refGlade) {

  Gtk::MenuItem *item =
      Gtk::manage(new Gtk::MenuItem(_("Remove Worklog Entry"), true));
  item->signal_activate().connect(
      sigc::mem_fun(*this, &WorkLogEntriesView::on_remove_worlog_entry));
  menu.append(*item);
  menu.show_all();

#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
  signal_button_press_event().connect(
      sigc::mem_fun(*this, &TasksView::on_button_press_event), false);
#endif
}

void DialogWorkLogEntries::on_button_close_clicked() { hide(); }
void DialogWorkLogEntries::on_day_changed() { load_entries(); }

void DialogWorkLogEntries::load() {
  load_days();
  load_entries();
}

void DialogWorkLogEntries::load_days() {
  cmbWorkDays->remove_all();

  std::list<std::string> days = WorkLogEntry::work_days();
  while (!days.empty()) {
    cmbWorkDays->append(days.front());
    days.pop_front();
  }

  cmbWorkDays->set_active(0);
}

void DialogWorkLogEntries::load_entries() {
  std::list<WorkLogEntry *> entries;
  Glib::ustring day = cmbWorkDays->get_active_text();

  entries = WorkLogEntry::all(day);

  treEntries->clear();
  trvEntries->remove_all_columns();

  while (!entries.empty()) {
    Gtk::TreeModel::Row row = *(treEntries->append());

    row[mdlColumn.id] = entries.front()->get_id();
    row[mdlColumn.task_name] = entries.front()->get_task_name();
    row[mdlColumn.start_date_entry] = entries.front()->get_start_date_entry();
    row[mdlColumn.start_hour_entry] =
        format_hour(entries.front()->get_start_hour_entry());
    row[mdlColumn.end_date_entry] = entries.front()->get_end_date_entry();
    row[mdlColumn.end_hour_entry] =
        format_hour(entries.front()->get_end_hour_entry());

    entries.pop_front();
  }

  trvEntries->append_column("ID", mdlColumn.id);
  trvEntries->append_column(_("Title"), mdlColumn.task_name);
  trvEntries->append_column(_("Start Date"), mdlColumn.start_date_entry);

  int nStartHourCol = trvEntries->append_column_editable(
      _("Start Hour"), mdlColumn.start_hour_entry);

  int nEndDateCol = trvEntries->append_column_editable(
      _("End Date"), mdlColumn.end_date_entry);

  int nEndHourCol = trvEntries->append_column_editable(
      _("End Hour"), mdlColumn.end_hour_entry);

  auto startHourRenderer = dynamic_cast<Gtk::CellRendererText *>(
      trvEntries->get_column_cell_renderer(nStartHourCol - 1));
  startHourRenderer->signal_edited().connect(
      sigc::mem_fun(*this, &DialogWorkLogEntries::on_start_hour_edited));

  auto endDateCol = dynamic_cast<Gtk::CellRendererText *>(
      trvEntries->get_column_cell_renderer(nEndDateCol - 1));
  endDateCol->signal_edited().connect(
      sigc::mem_fun(*this, &DialogWorkLogEntries::on_end_date_edited));

  auto endHourCol = dynamic_cast<Gtk::CellRendererText *>(
      trvEntries->get_column_cell_renderer(nEndHourCol - 1));
  endHourCol->signal_edited().connect(
      sigc::mem_fun(*this, &DialogWorkLogEntries::on_end_hour_edited));
}

void DialogWorkLogEntries::on_start_hour_edited(
    const Glib::ustring &path_string, const Glib::ustring &new_text) {
  WorkLogEntry *entry = recover_entry(path_string);
  if (entry) {
    entry->set_start_hour(new_text);
    entry->save();
  }
}

void DialogWorkLogEntries::on_end_date_edited(const Glib::ustring &path_string,
                                              const Glib::ustring &new_text) {
  WorkLogEntry *entry = recover_entry(path_string);
  if (entry) {
    entry->set_end_date(new_text);
    entry->save();
  }
}

void DialogWorkLogEntries::on_end_hour_edited(const Glib::ustring &path_string,
                                              const Glib::ustring &new_text) {
  WorkLogEntry *entry = recover_entry(path_string);
  if (entry) {
    entry->set_end_hour(new_text);
    entry->save();
  }
}

WorkLogEntry *
DialogWorkLogEntries::recover_entry(const Glib::ustring &path_string) {
  Gtk::TreePath path(path_string);

  Gtk::TreeModel::iterator iter = treEntries->get_iter(path);

  if (iter) {
    Gtk::TreeModel::Row row = *iter;
    Glib::ustring id = row[mdlColumn.id];
    return WorkLogEntry::find_by_id(id);
  }
  return NULL;
}

void DialogWorkLogEntries::remove_entry() {
  std::vector<Gtk::TreePath> paths =
      trvEntries->get_selection()->get_selected_rows();
  if (paths.size()) {
    Gtk::TreeRow row = Gtk::TreeRow(*treEntries->get_iter(*paths.begin()));

    Glib::ustring id = row[mdlColumn.id];
    WorkLogEntry *entry = WorkLogEntry::find_by_id(id);
    entry->remove();
  }
}

void DialogWorkLogEntries::WorkLogEntriesView::on_remove_worlog_entry() {
  dlgWorkLogEntries->remove_entry();
}

bool DialogWorkLogEntries::WorkLogEntriesView::on_button_press_event(
    GdkEventButton *event) {
  bool return_value = false;
  return_value = TreeView::on_button_press_event(event);

  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)) {
    menu.popup(event->button, event->time);
  }

  return return_value;
}

std::string DialogWorkLogEntries::format_hour(int hour) {

  int hours = hour / 60;
  int minutes = hour % 60;

  std::stringstream strHour;

  if (hours < 10) {
    strHour << "0";
  }
  strHour << hours;
  strHour << ":";
  if (minutes < 10) {
    strHour << "0";
  }
  strHour << minutes;

  return strHour.str();
}
