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

#ifndef VOLUME_SLIDER_H_INCLUDED
#define VOLUME_SLIDER_H_INCLUDED

#include "core/volume.h"

#include <QSlider>

class Volume_slider : public QSlider
{
    Q_OBJECT
public:
    explicit Volume_slider(QWidget* parent = 0);
signals:
    void changed_volume(const Volume& volume);
private:
    void init();
    void value_changed(int value);

    Volume volume;
};

#endif
