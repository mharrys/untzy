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

#include "db/database.h"
#include "db/song_row.h"
#include "db/playlist_row.h"

#include "gmock/gmock.h"

#include <QList>
#include <QString>

#include <vector>

using ::testing::StrEq;

#define CMP_STR(d, str) ASSERT_THAT(d.toStdString(), StrEq(str.toStdString()));

static const QList<QString> names = {
    "Default",
    "Heavy Metal",
    "Untzy",
    "Mängder med osorterad musik!",
    "asdfASDF1234¡@£$"
};

static const QList<Song> songs = {
    Song(QUrl("file:///music/ABBA - Dancing Queen.mp3"), "Dancing Queen", "ABBA", Duration(100), 10, 20, 30),
    Song(QUrl("file://~/"), "Colony", "In Flames", Duration(), -1, -2, -3),
    Song(QUrl("file:///"), "Världsherravälde", "Raubtier", Duration(999), 101010, 0, 0),
    Song(QUrl("http://example.com/Ensiferum%Battle_Song.ogg"), "Battle Song", "Ensiferum", Duration(0), 42000, 42, 2)
};

static auto db = Database::make(true);

TEST(DbTest, StartState)
{
    db->clear();
    ASSERT_TRUE(db->select_songs(1).isEmpty());
    ASSERT_TRUE(db->select_playlists().isEmpty());
}

TEST(DbTest, InsertPlaylist)
{
    db->clear();
    ASSERT_TRUE(db->select_playlists().isEmpty());
    long i = 1;
    for (auto name : names) {
        db->insert_playlist(name);
        auto playlists = db->select_playlists();
        auto row = playlists.at(i - 1);
        ASSERT_EQ(row.get_id(), i);
        CMP_STR(row.get_name(), name);
        i++;
    }
}

TEST(DbTest, InsertSong)
{
    db->clear();
    auto playlist = db->insert_playlist("Dummy").get_id();
    ASSERT_TRUE(db->select_songs(playlist).isEmpty());
    long i = 1;
    for (auto song : songs) {
        db->insert_song(song, playlist);
        auto songs = db->select_songs(playlist);
        auto row = songs.at(i - 1);
        ASSERT_EQ(row.get_song_id(), i);
        ASSERT_EQ(row.get_playlist_id(), playlist);
        auto row_song = row.get_song();
        CMP_STR(song.get_source().toString(), row_song.get_source().toString());
        CMP_STR(song.get_title(), row_song.get_title());
        CMP_STR(song.get_artist(), row_song.get_artist());
        ASSERT_EQ(song.get_duration().get_length(), row_song.get_duration().get_length());
        ASSERT_EQ(song.get_bitrate(), row_song.get_bitrate());
        ASSERT_EQ(song.get_sample_rate(), row_song.get_sample_rate());
        ASSERT_EQ(song.get_channels(), row_song.get_channels());
        i++;
    }
}

TEST(DbTest, UpdatePlaylist)
{
    db->clear();
    auto n1 = QString("Foo");
    auto n2 = QString("Dummy");
    auto n3 = QString("Bar");
    auto n4 = QString("New name");
    auto p1 = db->insert_playlist(n1).get_id();
    auto p2 = db->insert_playlist(n2).get_id();
    auto p3 = db->insert_playlist(n3).get_id();
    auto r1 = db->select_playlists().at(1);
    db->update_playlist(r1.get_id(), n4);
    auto r2 = db->select_playlists().at(1);
    CMP_STR(r1.get_name(), n2);
    CMP_STR(r2.get_name(), n4);
    auto r3 = db->select_playlists().at(0);
    auto r4 = db->select_playlists().at(2);
    CMP_STR(r3.get_name(), n1);
    CMP_STR(r4.get_name(), n3);
}

TEST(DbTest, DeletePlaylist)
{
    db->clear();
    ASSERT_TRUE(db->select_playlists().isEmpty());
    auto n1 = QString("Foo");
    auto p1 = db->insert_playlist(n1).get_id();
    ASSERT_EQ(db->select_playlists().size(), 1);
    db->delete_playlist(p1);
    ASSERT_TRUE(db->select_playlists().isEmpty());

    auto n2 = QString("Foo");
    auto n3 = QString("Bar");
    auto n4 = QString("Zot");
    auto p2 = db->insert_playlist(n1).get_id();
    auto p3 = db->insert_playlist(n2).get_id();
    auto p4 = db->insert_playlist(n3).get_id();
    ASSERT_EQ(db->select_playlists().size(), 3);

    db->delete_playlist(p3);
    auto pl1 = db->select_playlists();
    ASSERT_EQ(pl1.size(), 2);
    auto r1 = pl1.at(0);
    auto r2 = pl1.at(1);
    ASSERT_EQ(r1.get_id(), p2);
    ASSERT_EQ(r2.get_id(), p4);

    db->delete_playlist(p4);
    auto pl2 = db->select_playlists();
    ASSERT_EQ(pl2.size(), 1);
    auto r3 = pl2.at(0);
    ASSERT_EQ(r3.get_id(), p2);

    db->delete_playlist(p2);
    ASSERT_TRUE(db->select_playlists().isEmpty());
}

TEST(DbTest, DeleteSong)
{
    db->clear();

    // songs should no longer exist if playlist is deleted
    auto p1 = db->insert_playlist("Dummy").get_id();
    ASSERT_TRUE(db->select_songs(p1).isEmpty());
    for (auto song : songs) {
        db->insert_song(song, p1);
    }
    ASSERT_EQ(db->select_songs(p1).size(), songs.size());
    db->delete_playlist(p1);
    ASSERT_TRUE(db->select_songs(p1).isEmpty());

    // delete songs from existing playlist
    auto p2 = db->insert_playlist("Dummy").get_id();
    ASSERT_TRUE(db->select_songs(p2).isEmpty());
    std::vector<long> song_ids;
    for (auto song : songs) {
        song_ids.push_back(db->insert_song(song, p2).get_song_id());
    }
    while (!song_ids.empty()) {
        db->delete_song(song_ids[0]);
        song_ids.erase(song_ids.begin());
        ASSERT_EQ(db->select_songs(p2).size(), song_ids.size());
    }
}
