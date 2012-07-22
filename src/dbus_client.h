/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* dbus_client.h
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

#ifndef _DBUS_CLIENT_H_
#define _DBUS_CLIENT_H_

#include <giomm.h>
#include <glibmm.h>
#include <iostream>

using namespace std;

class DbusClient
{
public:
    DbusClient(int argc, char **argv);
    virtual ~DbusClient();

private:
    int argc;
    char **argv;

    void on_dbus_proxy_available(Glib::RefPtr<Gio::AsyncResult>& result);
};

#endif //_DBUS_CLIENT_H_

