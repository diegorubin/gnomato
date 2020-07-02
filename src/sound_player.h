/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * sound_player.h
 * Copyright (C) Diego Rubin 2020 <rubin.diego@gmail.com>
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

#ifndef _SOUND_PLAYER_H_
#define _SOUND_PLAYER_H_

#include <glibmm/i18n.h>
#include <gsound.h>
#include <iostream>
#include <sstream>

using namespace std;

class SoundPlayer {
public:
  SoundPlayer(string file);
  SoundPlayer();

  virtual ~SoundPlayer();

  // methods
  void play();

private:
  // attributes
  string file;
  GSoundContext *gsound;
};

#endif // SOUND_PLAYER
