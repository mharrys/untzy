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

#include "playlist_model.h"

Playlist_model::Playlist_model(QObject* parent)
    : QAbstractListModel(parent)
{
}

void Playlist_model::append_song(const Song& song)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    songs << song;
    endInsertRows();
}

QVariant Playlist_model::data(const QModelIndex& index, int role) const
{
    // map data to each column
    if (role == Qt::DisplayRole) {
        const Song* song = &songs.at(index.row());
        switch (index.column()) {
            case 0: return song->get_source().fileName();
            case 1: return song->get_title();
            case 2: return song->get_artist();
            case 3: return song->get_duration().get_str();
        }
    }
    return QVariant();
}

QVariant Playlist_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    // map name to each column
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Filename");
            case 1: return tr("Title");
            case 2: return tr("Artist");
            case 3: return tr("Duration");
        }
    }
    return QAbstractListModel::headerData(section, orientation, role);
}

QModelIndex Playlist_model::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column);
}

int Playlist_model::rowCount(const QModelIndex&) const
{
    return songs.count();
}

int Playlist_model::columnCount(const QModelIndex&) const
{
    return 4;
}
