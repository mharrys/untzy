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

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <QObject>

class Volume;

class Engine : public QObject {
public:
    virtual ~Engine() = default;
    virtual void load(const QUrl& url) = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void set_volume(const Volume& volume) = 0;
};

#endif
