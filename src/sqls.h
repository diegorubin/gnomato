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

#define CREATE_TASK "CREATE TABLE \
                       Task \
                         (\
                           id INTEGER, \
                           name VARCHAR(200), \
                           pomodoros INTEGER, \
                           done INTEGER, \
                           list VARCHAR(200), \
                           primary key(id)\
                         )"

#define SELECT_TASK "SELECT * FROM Task WHERE id = %s;"

#define SELECT_ALL_TASK "SELECT * FROM Task WHERE done = 0 AND list = '%s';" 

#define INSERT_TASK "INSERT INTO \
                      Task \
                        ( \
                          name,\
                          pomodoros,\
                          list,\
                          done \
                        )\
                      VALUES \
                        (\
                          '%s',\
                           %d, \
                           '%s',\
                           %d \
                         );"

#define UPDATE_TASK "UPDATE \
                       Task \
                     SET \
                       name = '%s', \
                       pomodoros = %d,\
                       list = '%s',\
                       done = %d \
                     WHERE \
                       id = %s;"

#define DELETE_TASK "DELETE FROM Task WHERE id = %s;"

#define SELECT_ALL_TASK_LISTS "SELECT list FROM Task WHERE done = 0 GROUP BY list ORDER BY list;"

#endif //_SQLS_
