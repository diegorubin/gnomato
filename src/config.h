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
#include <stdlib.h>
#include <string.h>
#include <giomm.h>

#include "utils.h"

using namespace std;

class Config
{
public:
    Config();
    virtual ~Config();

    Glib::RefPtr<Gio::Settings> settings;

    // preferences attributes
    string work_interval;
    string break_interval;
    string long_interval;
    string inactive_interval;
    string current_list;

    // parse methods
    bool save();
    bool load();

};

#endif //_CONFIG_H_

