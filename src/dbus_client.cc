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

const char *DbusClient::SHOW_WINDOW_SERVICE = "ShowWindow";
DbusClient::DbusClient(int argc, char **argv) 
{
  this->argc = argc;
  this->argv = argv;

  Gio::init();
  loop = Glib::MainLoop::create();

  // Get the user session bus connection.
  Glib::RefPtr<Gio::DBus::Connection> connection =
    Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);

  // Check for an unavailable connection.
  if(!connection)
  {
    std::cerr << "The user's session bus is not available." << std::endl;
  }

  // Create the proxy to the bus asynchronously.
  Gio::DBus::Proxy::create(connection, "com.diegorubin.Gnomato",
    "/com/diegorubin/Gnomato", "com.diegorubin.Gnomato",
    sigc::mem_fun(*this, &DbusClient::on_dbus_proxy_available));

  loop->run();
  
}

DbusClient::~DbusClient()
{
}

void DbusClient::on_dbus_proxy_available(Glib::RefPtr<Gio::AsyncResult>& result)
{
  Glib::RefPtr<Gio::DBus::Proxy> proxy = Gio::DBus::Proxy::create_finish(result);

  if(!proxy)
  {
    std::cerr << "The proxy to the user's session bus was not successfully "
      "created." << std::endl;

    loop->quit();
    return;
  }

  try
  {
    const Glib::VariantContainerBase result = proxy->call_sync(this->argv[1]);

    Glib::Variant<Glib::ustring> names_variant;
    result.get_child(names_variant);

    Glib::ustring remainer = names_variant.get();

    std::cout << remainer << std::endl;
    last_call_success = true;

  }
  catch(const Glib::Error& error)
  {
    last_call_success = false;
    std::cerr << "Got an error: '" << error.what() << "'." << std::endl;
  }

  Glib::signal_idle().
    connect(sigc::mem_fun(*this, &DbusClient::on_main_loop_idle));
}

bool DbusClient::get_last_call_success()
{
  return last_call_success;
}

bool DbusClient::on_main_loop_idle()
{
  loop->quit();
  return false;
}

bool DbusClient::check_if_running(char **argv)
{

  char *new_argv[2];

  new_argv[0] = (char *) malloc(strlen(argv[0]));
  strcpy(new_argv[0], argv[0]);

  new_argv[1] = strdup(DbusClient::SHOW_WINDOW_SERVICE);

  DbusClient *client = new DbusClient(2, new_argv);
  return client->get_last_call_success();
}

