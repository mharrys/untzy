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

#ifndef PLAYLIST_ROW_H_INCLUDED
#define PLAYLIST_ROW_H_INCLUDED

#include <QString>

// The responsibility of this class is to associate a database id with a
// playlist name.
class Playlist_row {
public:
    explicit Playlist_row(long id, const QString& name);
public:
    long get_id() const;
    QString get_name() const;
private:
    long id;
    QString name;
};

#endif
