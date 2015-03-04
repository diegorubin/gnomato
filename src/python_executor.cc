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

PythonExecutor::PythonExecutor(string script)
{
  this->script = script;

  Py_Initialize();

}

PythonExecutor::~PythonExecutor()
{
  Py_Finalize();
}

void PythonExecutor::execute(string list_name, string title_task)
{
  string python_script = home();
  python_script.append("/scripts/");
  python_script.append(script);

  cout << "Execute: " << python_script << endl;

  main_module = PyImport_AddModule("__main__");
  dict = PyModule_GetDict(main_module);
  
  python = fopen(python_script.c_str(), "r");
  PyRun_SimpleFile(python, python_script.c_str());

  execute_function = PyDict_GetItemString(dict, "execute");
  result = PyObject_CallFunction(execute_function, "ss", 
                                 list_name.c_str(), title_task.c_str());

}

