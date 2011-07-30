/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
* main.cc
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

#include <gtkmm.h>
#include <glibmm/i18n.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "sqls.h"
#include "utils.h"
#include "win_main.h"
#include "database.h"


sqlite3 *db;
char dbpath[1000];

void bootstrap()
{
  std::ifstream *fhome = new std::ifstream(home());

  // diretorio home nao existe
  if(!*fhome){
    mkdir(home(),0700);   
  }

  fhome = new std::ifstream(dbfile());
  
  // banco nao exite
  if(!*fhome){
   
    std::cout << _("Creating database") << std::endl;

    int result = sqlite3_open(dbpath, &db);

    int rc;
    char *cError;

    rc = sqlite3_exec(db,CREATE_TASK,NULL,NULL,&cError);
    if(rc != SQLITE_OK){
      cerr << "Sqlite3:" << cError << std::endl;
    }
    else{
      std::cout << _("Database successfully created ") << std::endl;
    }

    sqlite3_close(db);

  }

  delete(fhome);
  
}

int main(int argc, char **argv)
{

  sprintf(dbpath, "%s", dbfile());
  bootstrap();

  //intltool
  bindtextdomain(GETTEXT_PACKAGE, GNOMATO_LOCALEDIR);
  bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
  textdomain(GETTEXT_PACKAGE);
  
  // connect to database
  int result = sqlite3_open(dbpath, &db);
  if (result) {
      cerr << _("Could not open the database. Error: ") << sqlite3_errmsg(db);
      exit(1);
  }

  Gtk::Main kit(argc, argv);

  Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file(GNOMATO_DATADIR "/gnomato.ui");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  Glib::RefPtr<Gtk::StatusIcon> systray = 
     Gtk::StatusIcon::create_from_file(GNOMATO_DATADIR "/tomato.png");

  WinMain* winMain = 0;
  refBuilder->get_widget_derived("winMain", winMain);
  if(winMain)
  {
    winMain->set_icon_from_file(GNOMATO_DATADIR "/tomato.png");
    winMain->set_systray(systray);
    winMain->show();
    kit.run();
  }

  delete winMain;
  sqlite3_close(db);

  return 0;
}


