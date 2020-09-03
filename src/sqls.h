/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * sqls.h
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

#ifndef _SQLS_
#define _SQLS_

#define CREATE_TASK                                                            \
  "CREATE TABLE \
                       Task \
                         (\
                           id INTEGER, \
                           name VARCHAR(200), \
                           pomodoros INTEGER, \
                           done INTEGER, \
                           list VARCHAR(200), \
                           primary key(id)\
)"

#define CREATE_SETTINGS                                                        \
  "CREATE TABLE \
                       Settings \
                         (\
                           id INTEGER, \
                           name VARCHAR(200), \
                           value VARCHAR(200), \
                           primary key(id), \
                           unique(name, value) \
)"

#define CREATE_WORLOG_ENTRIES                                                  \
  "CREATE TABLE \
                       WorkLogEntries \
                         (\
                           id INTEGER, \
                           task_id INTEGER, \
                           start_date_entry VARCHAR(10), \
                           end_date_entry VARCHAR(10), \
                           start_hour_entry INTEGER, \
                           end_hour_entry INTEGER, \
                           primary key(id)\
)"

#define SELECT_TASK "SELECT * FROM Task WHERE id = %s;"

#define SELECT_ALL_TASK                                                        \
  "SELECT * FROM Task WHERE done = 0 ORDER BY position asc;"

#define SELECT_ALL_TASK_BY_LIST                                                \
  "SELECT * FROM Task WHERE list = '%s' AND done = %d ORDER BY position asc;"

#define SELECT_ALL_TASK_BY_LIST_AND_FILTER                                     \
  "SELECT * FROM Task WHERE list = '%s' AND name like '%%%s%%' AND done = %d " \
  "ORDER BY position asc;"

#define SELECT_WORKLOG_ENTRIES_BY_DATE                                         \
  "SELECT WorkLogEntries.id, task_id, start_date_entry, start_hour_entry, "    \
  "end_date_entry, "                                                           \
  "end_hour_entry, name FROM WorkLogEntries INNER JOIN Task ON Task.id = "     \
  "WorkLogEntries.task_id WHERE start_date_entry = '%s' ORDER BY "             \
  "start_hour_entry DESC;"

#define SELECT_WORKLOG_ENTRY_BY_ID                                             \
  "SELECT WorkLogEntries.id, task_id, start_date_entry, start_hour_entry, "    \
  "end_date_entry, "                                                           \
  "end_hour_entry, name FROM WorkLogEntries INNER JOIN Task ON Task.id = "     \
  "WorkLogEntries.task_id WHERE WorkLogEntries.id = %s "

#define SELECT_WORKLOG_DAYS                                                    \
  "SELECT start_date_entry "                                                   \
  "FROM WorkLogEntries "                                                       \
  "GROUP BY start_date_entry "                                                 \
  "ORDER BY start_date_entry DESC;"

#define INSERT_WORK_LOG_ENTRY                                                  \
  "INSERT INTO \
                      WorkLogEntries \
                        ( \
                          task_id,\
                          start_date_entry,\
                          start_hour_entry\
                        )\
                      VALUES \
                        (\
                          '%s',\
                          '%s',\
                          %d \
);"

#define UPDATE_WORK_LOG_ENTRY                                                  \
  "UPDATE \
                       WorkLogEntries \
                     SET \
                       end_date_entry = '%s', \
                       end_hour_entry = %d \
                     WHERE \
task_id = '%s' AND start_date_entry = '%s' AND start_hour_entry = %d;"

#define UPDATE_WORK_LOG_ENTRY_BY_ID                                            \
  "UPDATE \
                       WorkLogEntries \
                     SET \
                       start_date_entry = '%s', \
                       start_hour_entry = %d, \
                       end_date_entry = '%s', \
                       end_hour_entry = %d \
                     WHERE \
id = '%s' ;"

#define INSERT_TASK                                                            \
  "INSERT INTO \
                      Task \
                        ( \
                          name,\
                          pomodoros,\
                          list,\
                          done, \
                          position \
                        )\
                      VALUES \
                        (\
                          '%s',\
                           %d, \
                           '%s',\
                           %d, \
                           %d \
);"

#define UPDATE_TASK                                                            \
  "UPDATE \
                       Task \
                     SET \
                       name = '%s', \
                       pomodoros = %d,\
                       list = '%s',\
                       done = %d, \
                       position = %d \
                     WHERE \
id = %s;"

#define UPDATE_TASK_POSITION                                                   \
  "UPDATE \
                       Task \
                     SET \
                       position = %d \
                     WHERE \
id = %s;"

#define TASK_EXISTS                                                            \
  "SELECT \
                      COUNT(id) as TOTAL \
                     FROM Task \
WHERE name = '%s' AND list = '%s' AND done = 0;"

#define DELETE_TASK "DELETE FROM Task WHERE id = %s;"
#define DELETE_ALL_TASK_LISTS "DELETE FROM Task WHERE list = '%s';"

#define SELECT_ALL_TASK_LISTS                                                  \
  "SELECT list FROM Task WHERE done = 0 GROUP BY list ORDER BY list;"

// Migrations
#define CHECK_SETTINGS "SELECT * FROM Settings;"
#define CHECK_WORKLOG_ENTRIES "SELECT * FROM WorkLogEntries;"

#define ADD_TASK_POSITION "ALTER TABLE Task ADD position INTEGER DEFAULT 0;"
#define INSERT_TASK_POSITION                                                   \
  "INSERT INTO Settings (name, value) VALUES ('TASK_POSITION', 'CREATED');"

#endif //_SQLS_
