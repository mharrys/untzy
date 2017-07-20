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

class Playlist : public QTableView
{
    Q_OBJECT
public:
    explicit Playlist(QWidget* parent = 0);
public slots:
    void append_song(const Song& song);
signals:
    void select_song(const Song& song);
    void drop_file(const QUrl& url);
private:
    void init();

    Playlist_model playlist_model;
};

#endif
