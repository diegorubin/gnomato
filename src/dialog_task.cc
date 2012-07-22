/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* dialog_task.cc
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

#include "dialog_task.h"

DialogTask::DialogTask(BaseObjectType* cobject, 
                       const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::Dialog(cobject),
  m_refGlade(refGlade)
{
  // buttons
  m_refGlade->get_widget("btnTaskOk", btnOk);
  m_refGlade->get_widget("btnTaskCancel", btnCancel);

  m_refGlade->get_widget("entTaskName", entName);

  // connect signals
  btnCancel->signal_clicked().
             connect(sigc::mem_fun(*this,
                    &DialogTask::on_button_cancel_clicked));
  
  btnOk->signal_clicked().
         connect(sigc::mem_fun(*this,
               &DialogTask::on_button_ok_clicked));
  
  entName->signal_activate().
         connect(sigc::mem_fun(*this,
               &DialogTask::on_button_ok_clicked));

  show_all();
}

DialogTask::~DialogTask()
{
}

void DialogTask::set_id(std::string value)
{
  id = value;
  Task t(id);

  entName->set_text(t.get_name());
}

void DialogTask::on_button_cancel_clicked()
{
  id = "";
  entName->set_text("");
  hide();
}

void DialogTask::on_button_ok_clicked()
{
  if(id == ""){
    task = new Task();
    task->set_name(entName->get_text().c_str());
    task->create();
  }else{
    task = new Task(id);
    task->set_name(entName->get_text().c_str());
    task->save();
  }

  hide();

  id = "";
  entName->set_text("");
}

