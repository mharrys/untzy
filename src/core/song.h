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

#ifndef SONG_H_INCLUDED
#define SONG_H_INCLUDED

#include "duration.h"

#include <QUrl>

class QString;

// The responsibility of this class is to describe one song i.e. source,
// metadata, audio details etc.
class Song {
public:
    // Create song from file on disk.
    static Song make(const QUrl& source);

    // Create empty song.
    Song();

    // Create a new song.
    explicit Song(const QUrl& source,
                  const QString& title,
                  const QString& artist,
                  const Duration& duration);

    // Return source to song. This could be a file path or a URL to a stream.
    QUrl get_source() const;

    // Return track name.
    QString get_title() const;

    // Return artist name.
    QString get_artist() const;

    // Return song duration.
    Duration get_duration() const;
private:
    QUrl source;
    QString title, artist;
    Duration duration;
};

#endif
