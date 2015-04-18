/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* config.cc
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

#include "config.h"

Config::Config()
{
  Gio::init();
  settings = Gio::Settings::create("com.diegorubin.gnomato");
}

Config::~Config()
{
}

bool Config::save()
{
  settings->set_string("work-interval", work_interval);
  settings->set_string("break-interval", break_interval);
  settings->set_string("long-interval", long_interval);
  settings->set_string("inactive-interval", inactive_interval);
  settings->set_string("current-list", current_list);
  return true;
}

bool Config::load()
{
  work_interval = settings->get_string("work-interval");
  break_interval = settings->get_string("break-interval");
  long_interval = settings->get_string("long-interval");
  inactive_interval = settings->get_string("inactive-interval");
  current_list = settings->get_string("current-list");
  return true;
}

