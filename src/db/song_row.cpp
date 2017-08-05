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

#include "song_row.h"

Song_row::Song_row(long song_id, long playlist_id, const Song& song)
    : song_id(song_id),
      playlist_id(playlist_id),
      song(song)
{
}

long Song_row::get_song_id() const
{
    return song_id;
}

long Song_row::get_playlist_id() const
{
    return playlist_id;
}

Song Song_row::get_song() const
{
    return song;
}
