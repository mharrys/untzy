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

#include "playlist_tab.h"

#include "ui/widgets/playlist.h"

#include <QTabBar>
#include <QMouseEvent>
#include <QInputDialog>

Playlist_tab::Playlist_tab(QWidget* parent)
    : QTabWidget(parent),
      playlist_menu(this),
      bar_menu(this),
      selected_tab_index(-1)
{
    init();
}

void Playlist_tab::sync_with_database(std::unique_ptr<Database> db)
{
    this->db = std::move(db);

    auto playlists = db->select_playlists();
    if (playlists.isEmpty())
        append_playlist(tr("Default"));
    else {
        // add without inserting into database
        for (auto playlist_row : playlists) {
            append_playlist(playlist_row);
            for (auto song_row : db->select_songs(playlist_row.get_id()))
                append_song(song_row);
        }
    }
}

void Playlist_tab::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton || event->button() == Qt::MiddleButton) {
        event->accept();

        auto click_pos = event->localPos().toPoint();
        auto tab_bar = tabBar();
        auto inside_tab_bar = tab_bar->rect().contains(click_pos);
        if (inside_tab_bar) {
            auto index = clicked_tab_index(click_pos);
            if (event->button() == Qt::RightButton) {
                selected_tab_index = index;
                playlist_menu.popup(event->globalPos());
            } else if (event->button() == Qt::MiddleButton)
                removeTab(index);
        } else
            bar_menu.popup(event->globalPos());
    }
}

void Playlist_tab::append_song(const Song& song)
{
    auto playlist = current_playlist();
    auto song_row = db->insert_song(song, playlist->get_playlist_id());
    append_song(song_row);
}

void Playlist_tab::append_playlist(const QString& name)
{
    append_playlist(db->insert_playlist(name));
}

void Playlist_tab::init()
{
    tabBar()->setMovable(true);

    auto add_new_action = new QAction(tr("Add new playlist"), this);
    playlist_menu.addAction(add_new_action);
    bar_menu.addAction(add_new_action);
    connect(add_new_action, &QAction::triggered, [=]() {
        bool ok;
        auto playlist_name = QInputDialog::getText(
            this,
            tr("Add new playlist"),
            tr("Playlist name:"),
            QLineEdit::Normal,
            "",
            &ok);
        if (ok && !playlist_name.isEmpty())
            append_playlist(playlist_name);
    });

    auto rename_action = new QAction(tr("Rename playlist"), this);
    playlist_menu.addAction(rename_action);
    connect(rename_action, &QAction::triggered, [=]() {
        auto tab_index = selected_tab_index;
        auto tab_name = tabText(tab_index);
        bool ok;
        auto playlist_name = QInputDialog::getText(
            this,
            tr("Rename playlist"),
            tr("Playlist name:"),
            QLineEdit::Normal,
            tab_name,
            &ok);
        if (ok && !playlist_name.isEmpty()) {
            auto playlist = playlist_from_index(tab_index);
            db->update_playlist(playlist->get_playlist_id(), playlist_name);
            setTabText(tab_index, playlist_name);
        }
    });

    auto delete_action = new QAction(tr("Delete playlist"), this);
    playlist_menu.addAction(delete_action);
    connect(delete_action, &QAction::triggered, [=]() {
        auto playlist = playlist_from_index(selected_tab_index);
        db->delete_playlist(playlist->get_playlist_id());
        removeTab(selected_tab_index);
    });
}

int Playlist_tab::clicked_tab_index(const QPoint& click_pos)
{
    auto tab_bar = tabBar();
    for (auto i = 0; i < tab_bar->count(); i++) {
        if (tab_bar->tabRect(i).contains(click_pos)) {
            return i;
        }
    }
    return -1;
}

Playlist* Playlist_tab::current_playlist()
{
    return playlist_from_index(currentIndex());
}

Playlist* Playlist_tab::playlist_from_index(int index)
{
    return dynamic_cast<Playlist*>(widget(index));
}

void Playlist_tab::append_song(const Song_row& song_row)
{
    current_playlist()->append_song(song_row);
}

void Playlist_tab::append_playlist(const Playlist_row& playlist_row)
{
    auto playlist = new Playlist(playlist_row.get_id());
    connect(playlist, &Playlist::select_song, [=](const Song_row& row) {
        emit select_song(row.get_song());
    });
    connect(playlist, &Playlist::drop_file, [=](const QUrl& url) {
        emit drop_file(url);
    });
    addTab(playlist, playlist_row.get_name());
    // switch to added playlist
    setCurrentIndex(count() - 1);
}
