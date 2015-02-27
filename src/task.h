/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* task.h
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

#ifndef _TASK_H_
#define _TASK_H_

#include "database.h"
#include <list>

class Task : public Database {
public:
  Task();
  Task(std::string id);
  virtual ~Task();

  //methods
  bool create();
  bool save();
  bool destroy();
  bool finish();

  //setters
  void set_id(std::string id);
  void set_name(std::string value);
  void set_pomodoros(int value);
  void set_done(int value);
  void set_list(std::string value);

  //getters
  std::string get_id();
  std::string get_name();
  int get_pomodoros();
  int get_done();
  std::string get_list();

  static std::list<Task*> all();
  static std::list<Task*> all(std::string list);
  static std::list<Task*> all_by_sql(char *sql);
    
private:
  // attributes
  std::string id;
  std::string name;
  std::string list;
  int pomodoros;
  int done;
  
};

static int load_task(void *NotUsed, int argc, char **argv, char **azColName);

#endif //_TASK_H_

