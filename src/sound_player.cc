/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*!
 * sound_player.cc
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

#include "sound_player.h"

SoundPlayer::SoundPlayer(string file) : file(file) {
  gsound = gsound_context_new(NULL, NULL);
}

SoundPlayer::SoundPlayer() : SoundPlayer(GNOMATO_DATADIR "/alert.ogg") {}

SoundPlayer::~SoundPlayer() {}

void SoundPlayer::play() {
  if (!gsound) {
    cout << _("Error on load gsound context") << endl;
    return;
  }

  gsound_context_play_simple(gsound, NULL, NULL, GSOUND_ATTR_MEDIA_FILENAME,
                             "phone-incoming-call", NULL);
}
