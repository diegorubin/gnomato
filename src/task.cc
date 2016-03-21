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

const int SQL_SIZE = 10240;

std::list<Task*> tasks_aux;
unsigned int tasks_total;

Task::Task()
{
  pomodoros = 0;
  done = 0;
  position = 0;
}

Task::Task(std::string id)
{
  tasks_aux.clear();

  char sql[SQL_SIZE];
  sprintf(sql, SELECT_TASK, id.c_str());
  execute_query(sql, load_task);
   
  this->id = tasks_aux.front()->get_id();
  this->name = tasks_aux.front()->get_name();
  this->list = tasks_aux.front()->get_list();
  this->pomodoros = tasks_aux.front()->get_pomodoros();
  this->done = tasks_aux.front()->get_done();
  this->position = tasks_aux.front()->get_position();
}

Task::~Task()
{
}

bool Task::create()
{
  char sql[SQL_SIZE];
  sprintf(sql, INSERT_TASK, name.c_str(), pomodoros, list.c_str(), done, position);
  
  return execute_query(sql, load_task);
}

bool Task::destroy()
{
  char sql[SQL_SIZE];
  sprintf(sql, DELETE_TASK, id.c_str());
  
  return execute_query(sql, load_task);
}

bool Task::save()
{
  char sql[SQL_SIZE];
  sprintf(sql, UPDATE_TASK, name.c_str(), pomodoros, 
    list.c_str(), done, position, id.c_str());

  return execute_query(sql, load_task);
}

bool Task::finish()
{
  done = 1;
  save();
}

bool Task::exists()
{
  char sql[SQL_SIZE];
  sprintf(sql, TASK_EXISTS, name.c_str(), list.c_str());
  execute_query(sql, check_exists);
  return tasks_total == 1;
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

void Task::set_position(int value) 
{
  position = value;
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

int Task::get_position()
{
  return position;
}

std::list<Task*> Task::all()
{
  char sql[SQL_SIZE];
  sprintf(sql, SELECT_ALL_TASK);
  return all_by_sql(sql);
}

std::list<Task*> Task::all(std::string list)
{
  char sql[SQL_SIZE];
  sprintf(sql, SELECT_ALL_TASK_BY_LIST, list.c_str());
  return all_by_sql(sql);
}

std::list<Task*> Task::all(std::string list, std::string filter)
{
  char sql[SQL_SIZE];
  sprintf(sql, SELECT_ALL_TASK_BY_LIST_AND_FILTER, list.c_str(), filter.c_str());
  return all_by_sql(sql);
}

std::list<Task*> Task::all_by_sql(char *sql)
{
  tasks_aux.clear();

  execute_query(sql, load_task);

  return tasks_aux;
}

void Task::update_position(std::string id, int position)
{
  char sql[SQL_SIZE];
  sprintf(sql, UPDATE_TASK_POSITION, position, id.c_str());
  execute_query(sql, load_task);
}

static int check_exists(void *NotUsed, int argc, char **argv, char **azColName)
{
  tasks_total = atoi(argv[0]);
  return 0;
}

static int load_task(void *NotUsed, int argc, char **argv, char **azColName)
{

  Task *task_aux = new Task();

  task_aux->set_id(argv[0] ? argv[0] : ""); 
  task_aux->set_name(argv[1] ? argv[1] : "");
  task_aux->set_pomodoros(argv[2] ? atoi(argv[2]) : 0);
  task_aux->set_done(argv[3] ? atoi(argv[3]) : 0);
  task_aux->set_list(argv[4] ? argv[4] : "");
  task_aux->set_position(argv[5] ? atoi(argv[5]) : 0);

  tasks_aux.push_back(task_aux);

  return 0;
}

