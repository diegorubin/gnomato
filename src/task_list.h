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

#ifndef _TASK_LIST_H_
#define _TASK_LIST_H_

#include "database.h"
#include <list>

class TaskList : public Database {
public:
  TaskList();
  TaskList(std::string id);
  virtual ~TaskList();

  //methods
  bool create();
  bool save();
  bool destroy();

  //setters
  void set_id(std::string id);
  void set_name(std::string value);

  //getters
  std::string get_id();
  std::string get_name();

  static std::list<TaskList*> all();
    
private:
  // attributes
  std::string id;
  std::string name;
  
};

static int load(void *NotUsed, int argc, char **argv, char **azColName);
bool execute_query(char query[1000]);

#endif //_TASK_LIST_H_

