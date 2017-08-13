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

#include <QWidget>

// The responsibility of this class is to provide a widget to control the
// volume level.
//
// This is a complete custom widget that works as a QSlider but instead with
// an apperance that look more like a volume slider. The user can hover over
// the widget and use the scroll wheel to either increase or decrease the
// volume level, or click and drag to desired volume level.
class Volume_slider : public QWidget
{
    Q_OBJECT
public:
    explicit Volume_slider(QWidget* parent = 0);
    // change volume on mouse clicks, mouse scroll and movements
    void wheelEvent(QWheelEvent* event) final;
    void mouseMoveEvent(QMouseEvent* event) final;
    void mousePressEvent(QMouseEvent* event) final;
    // custom triangle volume slider
    void paintEvent(QPaintEvent* paint_event) final;
signals:
    void changed_volume(const Volume& volume);
private:
    void init();
    void new_level(double lvl);

    bool left_down;
    Volume volume;
};

#endif
