/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* dbus_client.cc
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

#include "dbus_client.h"

DbusClient::DbusClient(int argc, char **argv) 
{
  this->argc = argc;
  this->argv = argv;

  // Get the user session bus connection.
  Glib::RefPtr<Gio::DBus::Connection> connection =
    Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);

  // Check for an unavailable connection.
  if(!connection)
  {
    std::cerr << "The user's session bus is not available." << std::endl;
    //return 1;
  }

  // Create the proxy to the bus asynchronously.
  Gio::DBus::Proxy::create(connection, "com.diegorubin.Gnomato",
    "/com/diegorubin/Gnomato", "com.diegorubin.Gnomato",
    sigc::mem_fun(*this, &DbusClient::on_dbus_proxy_available));
  
}

DbusClient::~DbusClient()
{
}

void DbusClient::on_dbus_proxy_available(Glib::RefPtr<Gio::AsyncResult>& result)
{
}

