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

#include <QMouseEvent>
#include <QHeaderView>
#include <QItemSelectionModel>

#include <algorithm>

Playlist::Playlist(long playlist_id, QWidget* parent)
    : QTableView(parent),
      playlist_id(playlist_id),
      song_menu(this)
{
    init();
}

long Playlist::get_playlist_id() const
{
    return playlist_id;
}

void Playlist::append_song(const Song_row& row)
{
    playlist_model.append_song(row);
}

void Playlist::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        event->accept();
        auto index = indexAt(event->pos());
        auto row = playlist_model.get_song(index);
        emit select_song(row);
    }
}

void Playlist::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        event->accept();
        remove_songs(event->globalPos());
    }
}

void Playlist::init()
{
    setAcceptDrops(true);
    setShowGrid(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
    setModel(&playlist_model);
    connect(&playlist_model, &Playlist_model::drop_file, [=](const QUrl& url) {
        emit drop_file(url);
    });
    auto remove_action = new QAction(tr("Remove"), this);
    song_menu.addAction(remove_action);
    connect(remove_action, &QAction::triggered, [=]() {
        for (auto row : selected_rows) {
            emit remove_song(row);
            playlist_model.remove_song(row);
        }
    });
}

void Playlist::remove_songs(const QPoint& global_pos)
{
    auto selection = selectionModel();
    if (!selection->hasSelection())
        return;

    auto selected_indices = selection->selectedRows();
    auto to_song_row = [=](QModelIndex index) {
        return playlist_model.get_song(index);
    };
    selected_rows.clear();
    std::transform(selected_indices.begin(),
                   selected_indices.end(),
                   std::back_inserter(selected_rows),
                   to_song_row);
    song_menu.popup(global_pos);
}
