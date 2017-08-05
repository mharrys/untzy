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

#include "database.h"

#include "core/duration.h"

#include <QObject>
#include <QUrl>

#define THROW_QUERY_ERR \
    throw std::runtime_error(query.lastError().text().toStdString());

std::unique_ptr<Database> Database::make(bool in_memory)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(in_memory ? ":memory:" : "untzy.db");
    if (!db.open()) {
        auto msg = QObject::tr("Unable to open database");
        throw std::runtime_error(msg.toStdString());
    }

    QSqlQuery q;
    QStringList tables = db.tables();

    auto create_playlists = ""
        "CREATE TABLE playlists("
        "id integer primary key,"
        "name varchar)";
    if (!tables.contains("playlists") && !q.exec(create_playlists)) {
        auto msg = QObject::tr("Unable to create \"playlists\" database table");
        throw std::runtime_error(msg.toStdString());
    }

    auto create_songs = ""
        "CREATE TABLE songs("
        "id integer primary key,"
        "playlist integer,"
        "source varchar,"
        "title varchar,"
        "artist varchar,"
        "duration long,"
        "bitrate integer,"
        "sample_rate integer,"
        "channels integer)";
    if (!tables.contains("songs") && !q.exec(create_songs)) {
        auto msg = QObject::tr("Unable to create \"songs\" database table");
        throw std::runtime_error(msg.toStdString());
    }

    return std::make_unique<Database>(db);
}

Database::Database(QSqlDatabase db)
    : db(db)
{
}

QList<Song_row> Database::select_songs(long playlist)
{
    QList<Song_row> rows;

    QSqlQuery query;
    query.prepare("SELECT * FROM songs WHERE playlist=:playlist");
    query.bindValue(":playlist", QVariant::fromValue(playlist));

    if (!query.exec())
        THROW_QUERY_ERR

    while (query.next()) {
        auto r = query.record();
        auto s = Song(
            QUrl::fromUserInput(r.value("source").toString()),
            r.value("title").toString(),
            r.value("artist").toString(),
            Duration(r.value("duration").toLongLong()),
            r.value("bitrate").toInt(),
            r.value("sample_rate").toInt(),
            r.value("channels").toInt());
        auto row = Song_row(
            r.value("id").toLongLong(),
            r.value("playlist").toLongLong(),
            s);
        rows << row;
    }
    return rows;
}

QList<Playlist_row> Database::select_playlists()
{
    QList<Playlist_row> rows;
    QSqlQuery query;

    if (!query.exec("SELECT * FROM playlists"))
        THROW_QUERY_ERR

    while (query.next()) {
        auto r = query.record();
        auto row = Playlist_row(
            r.value("id").toLongLong(),
            r.value("name").toString());
        rows << row;
    }
    return rows;
}

Playlist_row Database::insert_playlist(const QString& name)
{
    QSqlQuery query;

    query.prepare("INSERT INTO playlists(name) values(:name)");
    query.bindValue(":name", name);

    if (!query.exec())
        THROW_QUERY_ERR

    return Playlist_row(
        query.lastInsertId().toLongLong(),
        name);
}

Song_row Database::insert_song(const Song& song, long playlist)
{
    QSqlQuery query;

    query.prepare(""
        "INSERT INTO songs("
        "playlist, source, title, artist, duration, bitrate, sample_rate,"
        "channels) values(:playlist, :source, :title, :artist, :duration, "
        ":bitrate, :sample_rate, :channels)");
    query.bindValue(":playlist", QVariant::fromValue(playlist));
    query.bindValue(":source", song.get_source().toString());
    query.bindValue(":title", song.get_title());
    query.bindValue(":artist", song.get_artist());
    query.bindValue(":duration", QVariant::fromValue(song.get_duration().get_length()));
    query.bindValue(":bitrate", song.get_bitrate());
    query.bindValue(":sample_rate", song.get_sample_rate());
    query.bindValue(":channels", song.get_channels());

    if (!query.exec())
        THROW_QUERY_ERR

    return Song_row(
        query.lastInsertId().toLongLong(),
        playlist,
        song);
}

void Database::update_playlist(long playlist, const QString& name)
{
    QSqlQuery query;
    query.prepare("UPDATE playlists SET name=:name WHERE id=:id");
    query.bindValue(":name", name);
    query.bindValue(":id", QVariant::fromValue(playlist));
    if (!query.exec())
        THROW_QUERY_ERR
}

void Database::delete_playlist(long playlist)
{
    QSqlQuery query;

    // delete playlist
    query.prepare("DELETE FROM playlists WHERE id=:playlist");
    query.bindValue(":playlist", QVariant::fromValue(playlist));
    if (!query.exec())
        THROW_QUERY_ERR

    // delete songs belonging to playlist
    query.prepare("DELETE FROM songs WHERE playlist=:playlist");
    query.bindValue(":playlist", QVariant::fromValue(playlist));
    if (!query.exec())
        THROW_QUERY_ERR
}
