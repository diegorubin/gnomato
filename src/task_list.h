/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* task_list.h
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

#ifndef _TASK_LIST_H_
#define _TASK_LIST_H_

#include "database.h"
#include <list>

class TaskList : public Database {
public:
  TaskList();
  virtual ~TaskList();

  //setters
  void set_name(std::string value);

  //getters
  std::string get_name();

  static std::list<TaskList*> all();
    
private:
  // attributes
  std::string name;
  
};

static int load_task_list(void *NotUsed, int argc, char **argv, char **azColName);

#endif //_TASK_LIST_H_

