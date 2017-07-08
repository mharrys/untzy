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

#include "song.h"

#include <QObject>

#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>

Song Song::make(const QUrl& source)
{
    TagLib::FileRef file_ref(source.toString().toStdString().c_str()); // trainwreck!!
    if (file_ref.isNull()) {
        auto err = QObject::tr("Unable to read song details: File not found");
        throw std::runtime_error(err.toStdString());
    }

    QString title, artist;
    if (file_ref.tag()) {
        TagLib::Tag* tag = file_ref.tag();
        auto convert = [](TagLib::String s) {
            return s.isNull() ? QString() : TStringToQString(s);
        };
        title = convert(tag->title());
        artist = convert(tag->artist());
    }

    Duration duration;
    if (file_ref.audioProperties()) {
        TagLib::AudioProperties* properties = file_ref.audioProperties();
        duration = Duration(properties->length());
    }

    return Song(source, title, artist, duration);
}

Song::Song(const QUrl& source,
           const QString& title,
           const QString& artist,
           const Duration& duration)
    : source(source),
      title(title),
      artist(artist),
      duration(duration)
{
}

QUrl Song::get_source() const
{
    return source;
}

QString Song::get_title() const
{
    return title;
}

QString Song::get_artist() const
{
    return artist;
}

Duration Song::get_duration() const
{
    return duration;
}
