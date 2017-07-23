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
    auto file_path = source.toString(QUrl::PreferLocalFile);
    TagLib::FileRef file_ref(file_path.toStdString().c_str());
    if (file_ref.isNull()) {
        auto err = QObject::tr("Unable to read song metadata!");
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
    int bitrate, sample_rate, channels;
    if (file_ref.audioProperties()) {
        TagLib::AudioProperties* properties = file_ref.audioProperties();
        duration = Duration(properties->length());
        bitrate = properties->bitrate();
        sample_rate = properties->sampleRate();
        channels = properties->channels();
    }

    return Song(
        source,
        title,
        artist,
        duration,
        bitrate,
        sample_rate,
        channels);
}

Song::Song()
{
}

Song::Song(const QUrl& source,
           const QString& title,
           const QString& artist,
           const Duration& duration,
           int bitrate,
           int sample_rate,
           int channels)
    : source(source),
      title(title),
      artist(artist),
      duration(duration),
      bitrate(bitrate),
      sample_rate(sample_rate),
      channels(channels)
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

int Song::get_bitrate() const
{
    return bitrate;
}

int Song::get_sample_rate() const
{
    return sample_rate;
}

int Song::get_channels() const
{
    return channels;
}
