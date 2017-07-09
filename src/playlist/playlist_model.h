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

#include "core/song.h"

#include <QAbstractListModel>

class Playlist_model : public QAbstractListModel {
    Q_OBJECT
public:
    explicit Playlist_model(QObject* parent = nullptr);

    // Append song to end of playlist
    void append_song(const Song& song);
    // Return song at specified index.
    Song get_song(const QModelIndex& index) const;

    // Minimal list model override
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex&parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
private:
    QList<Song> songs;
};

#endif
