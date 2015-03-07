/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* config.h
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

enum tokens {
    T_BEGIN_ATTR = '[',
    T_END_ATTR = ']',
    T_END_VALUE = '\n'
};

typedef char ATTR[255];
typedef char VALUE[10000];

using namespace std;

class Config
{
public:
    Config();
    Config(string config_file);
    virtual ~Config();

    string config_file;

    // preferences attributes
    string work_interval;
    string break_interval;
    string long_interval;
    string inactive_interval;

    // parse methods
    bool save();
    bool load();

private:
    void set_value(ATTR attr, VALUE value);
    bool config_file_exists();
};

#endif //_CONFIG_H_

