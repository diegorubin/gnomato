/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * dialog_preferences.cc
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
#include <gdkmm/rgba.h>

DialogWorkLogEntries::DialogWorkLogEntries(
    BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
    : Gtk::Dialog(cobject), m_refGlade(refGlade) {

  // buttons
  m_refGlade->get_widget("btnWorkLogSave", btnOk);
  m_refGlade->get_widget("btnWorkLogCancel", btnCancel);

  // connect signals
  btnCancel->signal_clicked().connect(
      sigc::mem_fun(*this, &DialogWorkLogEntries::on_button_cancel_clicked));
  btnOk->signal_clicked().connect(
      sigc::mem_fun(*this, &DialogWorkLogEntries::on_button_ok_clicked));

  show_all();
}

DialogWorkLogEntries::~DialogWorkLogEntries() {}

void DialogWorkLogEntries::on_button_cancel_clicked() { hide(); }

void DialogWorkLogEntries::on_button_ok_clicked() { hide(); }
