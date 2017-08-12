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

#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include "db/playlist_row.h"
#include "db/song_row.h"

#include <QList>
#include <QtSql>

#include <memory>

class Database {
public:
    // Create and insert expected tables or use existing database.
    static std::unique_ptr<Database> make(bool in_memory = false);

    // Create database with opened and ready to use QSqlDatabase.
    explicit Database(QSqlDatabase db);

    // Return all songs in specified playlist.
    QList<Song_row> select_songs(long playlist);
    // Return all playlists.
    QList<Playlist_row> select_playlists();

    // Insert playlist with specified name. Returns primary key for insertion.
    Playlist_row insert_playlist(const QString& name);
    // Insert song in specified playlist. Returns primary key for insertion.
    Song_row insert_song(const Song& song, long playlist);

    // Update playlist with new name.
    void update_playlist(long playlist, const QString& name);

    // Delete song.
    void delete_song(long song);
    // Delete playlist (and all songs that belongs to it).
    void delete_playlist(long playlist);
private:
    QSqlDatabase db;
};

#endif
