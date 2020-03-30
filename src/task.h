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

#include <list>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "database.h"

using namespace boost::gregorian;
using namespace boost::posix_time;

class WorkLogEntry : public Database {
public:
  WorkLogEntry(std::string task_id);
  WorkLogEntry(std::string task_id, std::string start_date_entry,
               int start_hour_entry);
  WorkLogEntry(std::string task_id, std::string start_date_entry,
               int start_hour_entry, std::string end_date_entry,
               int end_hour_entry);

  // methods
  bool create();
  bool update();

  // setters
  void set_task_name(std::string task_name);

  // getters
  std::string get_task_id();
  std::string get_task_name();
  std::string get_start_date_entry();
  std::string get_end_date_entry();
  int get_start_hour_entry();
  int get_end_hour_entry();

  static std::list<WorkLogEntry *> all(std::string date);
  static std::list<WorkLogEntry *> all_worklog_entries_by_sql(char *sql);

private:
  std::string task_id;
  std::string task_name;
  std::string start_date_entry;
  std::string end_date_entry;
  int start_hour_entry;
  int end_hour_entry;

  // methods
  bool save();
};

class Task : public Database {
public:
  Task();
  Task(std::string id);
  virtual ~Task();

  // methods
  void start();
  void pause();
  bool create();
  bool save();
  bool destroy();
  bool finish();
  bool exists();
  void update();

  // setters
  void set_id(std::string id);
  void set_name(std::string value);
  void set_pomodoros(int value);
  void set_done(int value);
  void set_list(std::string value);
  void set_position(int value);

  // getters
  std::string get_id();
  std::string get_name();
  int get_pomodoros();
  int get_done();
  std::string get_list();
  int get_position();

  static std::list<Task *> all();
  static std::list<Task *> all(std::string list);
  static std::list<Task *> all(std::string list, std::string filter);
  static std::list<Task *> all_by_sql(char *sql);
  static void update_position(std::string id, int position);

private:
  // attributes
  std::string id;
  std::string name;
  std::string list;
  int pomodoros;
  int done;
  int position;
  WorkLogEntry *workLlogEntry;
};

static int load_task(void *NotUsed, int argc, char **argv, char **azColName);
static int load_worklog_entry(void *NotUsed, int argc, char **argv,
                              char **azColName);
static int check_exists(void *NotUsed, int argc, char **argv, char **azColName);

#endif //_TASK_H_
