/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* task_list.cc
* Copyright (C) Diego Rubin 2015 <rubin.diego@gmail.com>
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

#include "task_list.h"

std::list<TaskList*> task_lists_aux;

TaskList::TaskList()
{
}

TaskList::~TaskList()
{
}

// setters
void TaskList::set_name(std::string value)
{
  name = value; 
}

// getters
std::string TaskList::get_name()
{
  return name;
}

std::list<TaskList*> TaskList::all()
{
  task_lists_aux.clear();

  char sql[500];
  sprintf(sql, SELECT_ALL_TASK_LISTS);
  execute_query(sql, load_task_list);

  return task_lists_aux;
}

static int load_task_list(void *NotUsed, int argc, char **argv, char **azColName)
{

  TaskList *task_list_aux = new TaskList();
  task_list_aux->set_name(argv[1] ? argv[1] : "");
  task_lists_aux.push_back(task_list_aux);

  return 0;
}

