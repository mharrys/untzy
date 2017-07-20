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

#include "playlist.h"

#include <QHeaderView>

Playlist::Playlist(QWidget* parent)
    : QTableView(parent)
{
    init();
}

void Playlist::append_song(const Song& song)
{
    playlist_model.append_song(song);
}

void Playlist::init()
{
    setAcceptDrops(true);
    setShowGrid(false);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSelectionBehavior(QAbstractItemView::SelectRows);
    verticalHeader()->setVisible(false);
    setModel(&playlist_model);
    connect(this, &QTableView::doubleClicked, [=](const QModelIndex& index) {
        auto song = playlist_model.get_song(index);
        emit select_song(song);
    });
    connect(&playlist_model, &Playlist_model::drop_file, [=](const QUrl& url) {
        emit drop_file(url);
    });
}
