/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * python_executor.cc
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

#include "python_executor.h"

PythonExecutor::PythonExecutor() {
  Py_Initialize();

  string python_script = home();
  python_script.append("/scripts");

  PyObject *sysPath = PySys_GetObject("path");

  PyObject *path = PyUnicode_FromString(python_script.c_str());

  PyList_Insert(sysPath, 0, path);
  module = PyImport_ImportModule("gnomato");

  if (module != NULL) {
    klass = PyObject_GetAttrString(module, "Gnomato");
    gnomato = PyObject_CallObject(klass, NULL);
  }
}

PythonExecutor::~PythonExecutor() { Py_Finalize(); }

PyObject *PythonExecutor::execute(string hook, string list_name,
                                  string title_task) {
  PyObject *result = NULL;

  if ((module != NULL) && (klass != NULL) && (gnomato != NULL)) {
    char *_hook = new char[hook.length() + 1];
    strcpy(_hook, hook.c_str());

    char *_list_name = new char[list_name.length() + 1];
    strcpy(_list_name, list_name.c_str());

    char *_title_task = new char[title_task.length() + 1];
    strcpy(_title_task, title_task.c_str());

    result = PyObject_CallMethod(gnomato, _hook, "ss", _list_name, _title_task);

    delete[] _hook;
    delete[] _list_name;
    delete[] _title_task;
  }

  return result;
}
