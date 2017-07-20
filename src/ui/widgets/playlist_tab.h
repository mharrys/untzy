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

#ifndef PLAYLIST_TAB_H_INCLUDED
#define PLAYLIST_TAB_H_INCLUDED

#include <QTabWidget>
#include <QMenu>

class Song;

// The responsibility of this class is to act as a tabbed container for
// playlists.
class Playlist_tab : public QTabWidget
{
    Q_OBJECT
public:
    explicit Playlist_tab(QWidget* parent = 0);
    // Override for context menu on right button click
    void mouseReleaseEvent(QMouseEvent* event) final;
public slots:
    // Append song to current selected playlist.
    void append_song(const Song& song);
    // Append a new playlist.
    void append_playlist(const QString& name);
signals:
    // Emitted when a song is double clicked in current playlist.
    void select_song(const Song& song);
    // Emitted when a file is dropped into current playlist.
    void drop_file(const QUrl& url);
private:
    void init();

    QMenu playlist_menu, bar_menu;
    int selected_tab_index;
};

#endif
