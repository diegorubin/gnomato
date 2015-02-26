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

Task::Task()
{
  pomodoros = 0;
  done = 0;
}

Task::Task(std::string id)
{
  tasks_aux.clear();

  char sql[500];
  sprintf(sql, SELECT_TASK, id.c_str());
  execute_query(sql, load_task);
   
  this->id = tasks_aux.front()->get_id();
  this->name = tasks_aux.front()->get_name();
  this->list = tasks_aux.front()->get_list();
  this->pomodoros = tasks_aux.front()->get_pomodoros();
  this->done = tasks_aux.front()->get_done();
}

Task::~Task()
{
}

bool Task::create()
{
  char sql[500];
  sprintf(sql, INSERT_TASK, name.c_str(), pomodoros, list.c_str(), done);
  
  return execute_query(sql, load_task);
}

bool Task::destroy()
{
  char sql[500];
  sprintf(sql, DELETE_TASK, id.c_str());
  
  return execute_query(sql, load_task);
}

bool Task::save()
{
  char sql[500];
  sprintf(sql, UPDATE_TASK, name.c_str(), pomodoros, 
    list.c_str(), done, id.c_str());

  return execute_query(sql, load_task);
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

void Task::set_list(std::string value)
{
  list = value; 
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

std::string Task::get_list()
{
  return list;
}

std::list<Task*> Task::all(std::string list)
{
  tasks_aux.clear();

  char sql[500];
  sprintf(sql, SELECT_ALL_TASK, list.c_str());
  execute_query(sql, load_task);

  return tasks_aux;
}

static int load_task(void *NotUsed, int argc, char **argv, char **azColName)
{

  Task *task_aux = new Task();

  task_aux->set_id(argv[0] ? argv[0] : ""); 
  task_aux->set_name(argv[1] ? argv[1] : "");
  task_aux->set_pomodoros(argv[2] ? atoi(argv[2]) : 0);
  task_aux->set_done(argv[3] ? atoi(argv[3]) : 0);
  task_aux->set_list(argv[4] ? argv[4] : "");

  tasks_aux.push_back(task_aux);

  return 0;
}

