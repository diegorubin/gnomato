/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* dialog_task.h
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

#ifndef _WIN_TASK_H_
#define _WIN_TASK_H_

#include <gtkmm.h>
#include "task.h"

using namespace Gtk;

class DialogTask: public Gtk::Dialog
{
public:
  DialogTask(BaseObjectType* cobject, 
             const Glib::RefPtr<Gtk::Builder>& refGlade);
  DialogTask();
  virtual ~DialogTask();

  //methods
  void set_id(std::string value);

private:

  std::string id;

  Button *btnOk;
  Button *btnCancel;

  Entry *entName;

  Task *task;
  Glib::RefPtr<Gtk::Builder> m_refGlade;

  // callback methods
  virtual void on_button_cancel_clicked();
  virtual void on_button_ok_clicked();
};

#endif //_WIN_TASK_H_
