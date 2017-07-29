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

#include <QMimeData>

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

Song Playlist_model::get_song(const QModelIndex& index) const
{
    return songs.at(index.row());
}

QVariant Playlist_model::data(const QModelIndex& index, int role) const
{
    // map data to each column
    if (role == Qt::DisplayRole) {
        const Song* song = &songs.at(index.row());
        switch (index.column()) {
            case filename:
                return song->get_source().fileName();
            case title:
                return song->get_title();
            case artist:
                return song->get_artist();
            case duration:
                return song->get_duration().get_str();
        }
    }
    return QVariant();
}

QVariant Playlist_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    // map name to each column
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case filename:
                return tr("Filename");
            case title:
                return tr("Title");
            case artist:
                return tr("Artist");
            case duration:
                return tr("Duration");
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
    return header_tag_count;
}

Qt::ItemFlags Playlist_model::flags(const QModelIndex& index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
}

bool Playlist_model::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
    // accept all types of data and let the loader determine if valid or not
    return true;
}

bool Playlist_model::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    auto urls = data->urls();
    for (auto url : urls)
        emit drop_file(url);
    return true;
}
