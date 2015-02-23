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
char *cError = 0;

TaskList::TaskList()
{
  cError = 0;
}

TaskList::TaskList(std::string id)
{
  cError = 0;
  task_lists_aux.clear();

  char sql[500];
  sprintf(sql, SELECT_TASK_LIST, id.c_str());
  execute_query(sql);
   
  this->id = task_lists_aux.front()->get_id();
  this->name = task_lists_aux.front()->get_name();
}

TaskList::~TaskList()
{
}

bool TaskList::create()
{
  char sql[500];
  sprintf(sql, INSERT_TASK_LIST, name.c_str());
  
  return execute_query(sql);
}

bool TaskList::destroy()
{
  char sql[500];
  sprintf(sql, DELETE_TASK_LIST, id.c_str());
  
  return execute_query(sql);
}

bool TaskList::save()
{
  char sql[500];
  sprintf(sql, UPDATE_TASK_LIST, name.c_str(), id.c_str());

  return execute_query(sql);
}

// setters
void TaskList::set_id(std::string id)
{
  this->id = id;
}

void TaskList::set_name(std::string value)
{
  name = value; 
}

// getters
std::string TaskList::get_id()
{
  return id;
}

std::string TaskList::get_name()
{
  return name;
}

std::list<TaskList*> TaskList::all()
{
  task_lists_aux.clear();

  char sql[500];
  sprintf(sql, SELECT_ALL_TASK);
  execute_query(sql);

  return task_lists_aux;
}

bool execute_query(char query[1000])
{
  int rc;

  rc = sqlite3_exec(db, query, load, 0, &cError);
  if(rc != SQLITE_OK){
    std::cerr << "Sqlite3: " << cError << std::endl;
    sqlite3_free(cError);
    return false;
  }
  
  return true;
}


static int
load (void *NotUsed, int argc, char **argv, char **azColName)
{

  TaskList *task_aux = new TaskList();

  task_aux->set_id(argv[0] ? argv[0] : ""); 
  task_aux->set_name(argv[1] ? argv[1] : "");

  task_lists_aux.push_back(task_aux);

  return 0;
}

