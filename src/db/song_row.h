// This file is part of Untzy.
//
// Untzy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Untzy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Untzy.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SONG_ROW_H_INCLUDED
#define SONG_ROW_H_INCLUDED

#include "core/song.h"

// The responsibility of this class is to associate database keys with song
// instance.
class Song_row {
public:
    explicit Song_row(long song_id, long playlist_id, const Song& song);
public:
    long get_song_id() const;
    long get_playlist_id() const;
    Song get_song() const;
private:
    long song_id;
    long playlist_id;
    Song song;
};

#endif
