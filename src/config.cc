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
    work_interval = "25";
    break_interval = "5";
    long_interval = "15";
    wunderlist_token = "";
    
    config_file = cfgfile();

}

Config::Config(string config_file)
{
    this->config_file = config_file;
}

Config::~Config()
{
}

bool Config::config_file_exists()
{
}

bool Config::save()
{
    ofstream file;
    file.open(config_file.c_str(), std::ios::out);

    if(!file.is_open()) return false;

    file << "[work_interval]" << work_interval << "\n";
    file << "[break_interval]" << break_interval << "\n";
    file << "[long_interval]" << long_interval << "\n";
    file << "[wunderlist_token]" << wunderlist_token << "\n";

    file.close();
    return true;
}

bool Config::load()
{
    char c;
    VALUE value;
    ATTR attr;
    int pos_attr = 0;
    int pos_value = 0;

    bool is_value = false;
    ifstream file;
    file.open(config_file.c_str(), std::ios::in);

    if(!file.is_open()) return false;

    while(file.get(c)){
        switch(c){
            case T_BEGIN_ATTR:{
                pos_attr = 0;
                break;
            }
            case T_END_ATTR:{
                pos_value = 0;
                is_value = true;
                attr[pos_attr] = 0;
                break;
            }
            case T_END_VALUE:{
                is_value = false;
                value[pos_value] = 0;
                set_value(attr,value);
                break;
            }
            default:{
                if(is_value) value[pos_value++] = c;
                else attr[pos_attr++] = c;
            }
        }
    }
    file.close();
    return true;
}

void Config::set_value(ATTR attr, VALUE value)
{
    if(!strcmp(attr,"work_interval")) work_interval = value;
    if(!strcmp(attr,"break_interval")) break_interval = value;
    if(!strcmp(attr,"long_interval")) long_interval = value;
    if(!strcmp(attr,"wunderlist_token")) wunderlist_token = value;
}

