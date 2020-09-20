/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * database.cc
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

#include "database.h"

Database::Database() {}

Database::~Database() {}

bool Database::execute_query(char query[1000],
                             int (*callback)(void *, int, char **, char **)) {
  int rc;
  char *cError = 0;

  rc = sqlite3_exec(db, query, callback, 0, &cError);
  if (rc != SQLITE_OK) {
    std::cerr << "Sqlite3: " << cError << std::endl;
    sqlite3_free(cError);
    return false;
  }

  return true;
}

bool Database::execute_query(char query[1000]) {
  int rc;
  char *cError = 0;

  rc = sqlite3_exec(db, query, NULL, 0, &cError);
  if (rc != SQLITE_OK) {
    std::cerr << "Sqlite3: " << cError << std::endl;
    sqlite3_free(cError);
    return false;
  }

  return true;
}
