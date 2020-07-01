/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * dialog_preferences.h
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

#include <gtkmm.h>

using namespace Gtk;

class DialogWorkLogEntries : public Gtk::Dialog {
public:
  DialogWorkLogEntries(BaseObjectType *cobject,
                       const Glib::RefPtr<Gtk::Builder> &refGlade);

  virtual ~DialogWorkLogEntries();

protected:
private:
  // attributes
  Button *btnOk;
  Button *btnCancel;

  Glib::RefPtr<Gtk::Builder> m_refGlade;

  // callback methods
  virtual void on_button_cancel_clicked();
  virtual void on_button_ok_clicked();
};

#endif // WIN_WORK_LOG_ENTRIES
