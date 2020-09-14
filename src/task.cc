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

std::list<Task *> tasks_aux;
std::list<WorkLogEntry *> worklog_entries_aux;
std::list<std::string> worklog_days_aux;
unsigned int tasks_total;

const std::locale date_format(std::locale::classic(),
                              new boost::gregorian::date_facet("%Y-%m-%d"));

std::string format(const boost::gregorian::date &date) {
  std::ostringstream os;
  os.imbue(date_format);
  os << date;
  return os.str();
}

int to_minutes(std::string value) {
  return atoi(value.substr(0, 2).c_str()) * 60 +
         atoi(value.substr(3, 2).c_str());
}

int get_position_in_day(time_duration current_time_duration) {
  int result =
      current_time_duration.hours() * 60 + current_time_duration.minutes();
  return result;
}

WorkLogEntry::WorkLogEntry(std::string task_id) {
  ptime now = second_clock::local_time();
  date today = now.date();
  time_duration duration = now.time_of_day();

  this->task_id = task_id;
  this->start_date_entry = format(today);
  this->start_hour_entry = get_position_in_day(duration);
  this->end_date_entry = "";
  this->end_hour_entry = 0;
}

WorkLogEntry::WorkLogEntry(std::string task_id, std::string start_date_entry,
                           int start_hour_entry) {
  this->task_id = task_id;
  this->start_date_entry = start_date_entry;
  this->start_hour_entry = start_hour_entry;
  this->end_date_entry = "";
  this->end_hour_entry = 0;
}

WorkLogEntry::WorkLogEntry(std::string task_id, std::string start_date_entry,
                           int start_hour_entry, std::string end_date_entry,
                           int end_hour_entry) {
  this->task_id = task_id;
  this->start_date_entry = start_date_entry;
  this->start_hour_entry = start_hour_entry;
  this->end_date_entry = end_date_entry;
  this->end_hour_entry = end_hour_entry;
}

bool WorkLogEntry::update() {

  ptime now = second_clock::local_time();
  date today = now.date();
  time_duration duration = now.time_of_day();

  this->end_date_entry = format(today);
  this->end_hour_entry = get_position_in_day(duration);

  return this->save();
}

bool WorkLogEntry::create() {
  char sql[SQL_SIZE];
  sprintf(sql, INSERT_WORK_LOG_ENTRY, task_id.c_str(), start_date_entry.c_str(),
          start_hour_entry);

  return execute_query(sql, load_task);
}

bool WorkLogEntry::save() {
  char sql[SQL_SIZE];
  if (id.empty()) {
    sprintf(sql, UPDATE_WORK_LOG_ENTRY, end_date_entry.c_str(), end_hour_entry,
            task_id.c_str(), start_date_entry.c_str(), start_hour_entry);
  } else {
    sprintf(sql, UPDATE_WORK_LOG_ENTRY_BY_ID, start_date_entry.c_str(),
            start_hour_entry, end_date_entry.c_str(), end_hour_entry,
            id.c_str());
  }

  std::cout << "updating worklog: " << sql << std::endl;
  return execute_query(sql, load_task);
}

std::list<WorkLogEntry *> WorkLogEntry::all(std::string date) {
  char sql[SQL_SIZE];
  sprintf(sql, SELECT_WORKLOG_ENTRIES_BY_DATE, date.c_str());
  return all_worklog_entries_by_sql(sql);
}

std::list<WorkLogEntry *> WorkLogEntry::all_worklog_entries_by_sql(char *sql) {
  worklog_entries_aux.clear();
  execute_query(sql, load_worklog_entry);
  return worklog_entries_aux;
}

WorkLogEntry *WorkLogEntry::find_by_id(std::string id) {
  char sql[SQL_SIZE];
  sprintf(sql, SELECT_WORKLOG_ENTRY_BY_ID, id.c_str());

  worklog_entries_aux.clear();
  execute_query(sql, load_worklog_entry);
  return worklog_entries_aux.front();
}

std::list<std::string> WorkLogEntry::work_days() {
  worklog_days_aux.clear();

  char sql[SQL_SIZE];
  sprintf(sql, SELECT_WORKLOG_DAYS);

  execute_query(sql, load_workdays);
  return worklog_days_aux;
}

void WorkLogEntry::set_id(std::string id) { this->id = id; }

void WorkLogEntry::set_task_name(std::string task_name) {
  this->task_name = task_name;
}

void WorkLogEntry::set_start_hour(std::string hour) {
  this->start_hour_entry = to_minutes(hour);
}

void WorkLogEntry::set_end_date(std::string date) {
  this->end_date_entry = date;
}

void WorkLogEntry::set_end_hour(std::string hour) {
  this->end_hour_entry = to_minutes(hour);
}

std::string WorkLogEntry::get_id() { return this->id; }

std::string WorkLogEntry::get_task_id() { return this->task_id; }

std::string WorkLogEntry::get_task_name() { return this->task_name; }

std::string WorkLogEntry::get_start_date_entry() {
  return this->start_date_entry;
}

int WorkLogEntry::get_start_hour_entry() { return this->start_hour_entry; }

std::string WorkLogEntry::get_end_date_entry() { return this->end_date_entry; }

int WorkLogEntry::get_end_hour_entry() { return this->end_hour_entry; }

Task::Task() {
  pomodoros = 0;
  done = 0;
  position = 0;
  workLlogEntry = NULL;
}

Task::Task(std::string id) {
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
  this->workLlogEntry = NULL;
}

Task::~Task() {}

void Task::start() {
  workLlogEntry = new WorkLogEntry(id);
  workLlogEntry->create();
}

void Task::update() {
  if (workLlogEntry != NULL) {
    workLlogEntry->update();
  }
}

bool Task::create() {
  char sql[SQL_SIZE];
  sprintf(sql, INSERT_TASK, name.c_str(), pomodoros, list.c_str(), done,
          position);

  return execute_query(sql, load_task);
}

bool Task::destroy() {
  char sql[SQL_SIZE];
  sprintf(sql, DELETE_TASK, id.c_str());

  return execute_query(sql, load_task);
}

bool Task::save() {
  char sql[SQL_SIZE];
  sprintf(sql, UPDATE_TASK, name.c_str(), pomodoros, list.c_str(), done,
          position, id.c_str());

  return execute_query(sql, load_task);
}

bool Task::finish() {
  done = 1;
  return save();
}

bool Task::exists() {
  char sql[SQL_SIZE];
  sprintf(sql, TASK_EXISTS, name.c_str(), list.c_str());
  execute_query(sql, check_exists);
  return tasks_total == 1;
}

// setters
void Task::set_id(std::string id) { this->id = id; }

void Task::set_name(std::string value) { name = value; }

void Task::set_pomodoros(int value) { pomodoros = value; }

void Task::set_done(int value) { done = value; }

void Task::set_list(std::string value) { list = value; }

void Task::set_position(int value) { position = value; }

// getters
std::string Task::get_id() { return id; }

std::string Task::get_name() { return name; }

int Task::get_pomodoros() { return pomodoros; }

int Task::get_done() { return done; }

std::string Task::get_list() { return list; }

int Task::get_position() { return position; }

int Task::elapsed_time() {
  if (workLlogEntry) {
    ptime now = second_clock::local_time();
    time_duration duration = now.time_of_day();
    return get_position_in_day(duration) -
           workLlogEntry->get_start_hour_entry();
  }
  return 0;
}

std::list<Task *> Task::all() {
  char sql[SQL_SIZE];
  sprintf(sql, SELECT_ALL_TASK);
  return all_by_sql(sql);
}

std::list<Task *> Task::all(std::string list, int done) {
  char sql[SQL_SIZE];
  sprintf(sql, SELECT_ALL_TASK_BY_LIST, list.c_str(), done);
  return all_by_sql(sql);
}

std::list<Task *> Task::all(std::string list, std::string filter, int done) {
  char sql[SQL_SIZE];
  sprintf(sql, SELECT_ALL_TASK_BY_LIST_AND_FILTER, list.c_str(), filter.c_str(),
          done);
  return all_by_sql(sql);
}

std::list<Task *> Task::all_by_sql(char *sql) {
  tasks_aux.clear();

  execute_query(sql, load_task);

  return tasks_aux;
}

void Task::update_position(std::string id, int position) {
  char sql[SQL_SIZE];
  sprintf(sql, UPDATE_TASK_POSITION, position, id.c_str());
  execute_query(sql, load_task);
}

static int check_exists(void *NotUsed, int argc, char **argv,
                        char **azColName) {
  tasks_total = atoi(argv[0]);
  return 0;
}

static int load_task(void *NotUsed, int argc, char **argv, char **azColName) {

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

static int load_worklog_entry(void *NotUsed, int argc, char **argv,
                              char **azColName) {

  WorkLogEntry *entry = new WorkLogEntry(
      argv[1], argv[2] ? argv[2] : "", argv[3] ? atoi(argv[3]) : 0,
      argv[4] ? argv[4] : "", argv[5] ? atoi(argv[5]) : 0);
  entry->set_task_name(argv[6]);
  entry->set_id(argv[0]);

  worklog_entries_aux.push_back(entry);

  return 0;
}

static int load_workdays(void *NotUsed, int argc, char **argv,
                         char **azColName) {

  worklog_days_aux.push_back(argv[0]);

  return 0;
}
