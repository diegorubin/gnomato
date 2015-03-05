/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* python_executor.h
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

#ifndef _PYTHON_EXECUTOR_H_
#define _PYTHON_EXECUTOR_H_

#include <iostream>
#include <Python.h>

#include "utils.h"

using namespace std;

class PythonExecutor 
{
  public:
  PythonExecutor(string script);
  virtual ~PythonExecutor();

  //methods
  void execute(string list_name, string title_task);
  string get_result_as_string();

  private:

  //attribute
  PyObject *main_module, *dict;
  PyObject *execute_function, *result;

  string script;

  FILE *python;

};

#endif // _PYTHON_EXECUTOR_H_

