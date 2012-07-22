/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* task.cc
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

#include "task.h"

std::list<Task*> tasks_aux;
char *cError = 0;

Task::Task()
{
  pomodoros = 0;
  done = 0;
  
  cError = 0;
}

Task::Task(std::string id)
{
  cError = 0;
  tasks_aux.clear();

  char sql[500];
  sprintf(sql, SELECT_TASK, id.c_str());
  execute_query(sql);
   
  this->id = tasks_aux.front()->get_id();
  this->name = tasks_aux.front()->get_name();
  this->pomodoros = tasks_aux.front()->get_pomodoros();
  this->done = tasks_aux.front()->get_done();
}

Task::~Task()
{
}

bool Task::create()
{
  char sql[500];
  sprintf(sql, INSERT_TASK, name.c_str(), pomodoros, done);
  
  return execute_query(sql);
}

bool Task::destroy()
{
  char sql[500];
  sprintf(sql, DELETE_TASK, id.c_str());
  
  return execute_query(sql);
}

bool Task::save()
{
  char sql[500];
  sprintf(sql, UPDATE_TASK, name.c_str(), pomodoros, done, id.c_str());

  return execute_query(sql);
}

bool Task::finish()
{
  done = 1;
  save();
}

// setters
void Task::set_id(std::string id)
{
  this->id = id;
}

void Task::set_name(std::string value)
{
  name = value; 
}

void Task::set_pomodoros(int value)
{
  pomodoros = value; 
}

void Task::set_done(int value)
{
  done = value; 
}

// getters
std::string Task::get_id()
{
  return id;
}

std::string Task::get_name()
{
  return name;
}

int Task::get_pomodoros()
{
  return pomodoros;
}

int Task::get_done()
{
  return done;
}

std::list<Task*> Task::all()
{
  tasks_aux.clear();

  char sql[500];
  sprintf(sql, SELECT_ALL_TASK);
  execute_query(sql);

  return tasks_aux;
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

  Task *task_aux = new Task();

  task_aux->set_id(argv[0] ? argv[0] : ""); 
  task_aux->set_name(argv[1] ? argv[1] : "");
  task_aux->set_pomodoros(argv[2] ? atoi(argv[2]) : 0);
  task_aux->set_done(argv[3] ? atoi(argv[3]) : 0);

  tasks_aux.push_back(task_aux);

  return 0;
}

