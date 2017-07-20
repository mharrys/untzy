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

#ifndef PLAYLIST_H_INCLUDED
#define PLAYLIST_H_INCLUDED

#include "playlist/playlist_model.h"

#include <QTableView>

// The responsibility of this class is to view a list of songs.
class Playlist : public QTableView
{
    Q_OBJECT
public:
    explicit Playlist(QWidget* parent = 0);
public slots:
    // Append song to playlist.
    void append_song(const Song& song);
signals:
    // Emitted when a user double clicks a song in the playlist.
    void select_song(const Song& song);
    // Emitted when a user drops a file(s) into the playlist.
    void drop_file(const QUrl& url);
private:
    void init();

    Playlist_model playlist_model;
};

#endif
