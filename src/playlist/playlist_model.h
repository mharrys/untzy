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

#ifndef PLAYLIST_MODEL_H_INCLUDED
#define PLAYLIST_MODEL_H_INCLUDED

#include "db/song_row.h"

#include <QAbstractListModel>

// The responsibility of this class is to store a list of songs.
class Playlist_model : public QAbstractListModel {
    Q_OBJECT
public:
    // Row header column tags.
    enum Header_tag {
        filename = 0,
        title,
        artist,
        duration,
        header_tag_count
    };

    explicit Playlist_model(QObject* parent = 0);

    // List model override
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex&parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
public slots:
    // Append song to end of playlist
    void append_song(const Song_row& row);
    // Remove song from playlist.
    void remove_song(const Song_row& row);
    // Return song at specified index.
    Song_row get_song(const QModelIndex& index) const;
signals:
    void drop_file(const QUrl& url);
private:
    QList<Song_row> rows;
};

#endif
