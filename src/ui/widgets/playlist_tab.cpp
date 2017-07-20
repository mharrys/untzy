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

#include "playlist.h"

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

void Playlist_tab::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        event->accept();

        auto click_pos = event->localPos().toPoint();
        auto tab_bar = tabBar();

        auto inside_tab_bar = tab_bar->rect().contains(click_pos);
        if (inside_tab_bar) {
            // determine which tab index received right click and show menu
            for (auto i = 0; i < tab_bar->count(); i++) {
                if (tab_bar->tabRect(i).contains(click_pos)) {
                    selected_tab_index = i;
                    playlist_menu.popup(event->globalPos());
                    break;
                }
            }
        } else
            bar_menu.popup(event->globalPos());
    }
}

void Playlist_tab::append_song(const Song& song)
{
    auto playlist = dynamic_cast<Playlist*>(widget(currentIndex()));
    playlist->append_song(song);
}

void Playlist_tab::append_playlist(const QString& name)
{
    auto playlist = new Playlist();
    connect(playlist, &Playlist::select_song, [=](const Song& song) {
        emit select_song(song);
    });
    connect(playlist, &Playlist::drop_file, [=](const QUrl& url) {
        emit drop_file(url);
    });
    addTab(playlist, name);
}

void Playlist_tab::init()
{
    tabBar()->setMovable(true);
    append_playlist(tr("Default"));

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
        if (ok && !playlist_name.isEmpty())
            setTabText(tab_index, playlist_name);
    });

    auto delete_action = new QAction(tr("Delete playlist"), this);
    playlist_menu.addAction(delete_action);
    connect(delete_action, &QAction::triggered, [=]() {
        removeTab(selected_tab_index);
    });
}
