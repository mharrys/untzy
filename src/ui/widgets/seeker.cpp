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

#include "seeker.h"

Seeker::Seeker(QWidget* parent)
    : QSlider(parent),
      dragging(false)
{
    init();
}

void Seeker::set_position(long position)
{
    if (!dragging)
        setSliderPosition(position);
}

void Seeker::set_length(long length)
{
    setRange(0, length);
}

void Seeker::init()
{
    setMinimumWidth(500);
    setOrientation(Qt::Horizontal);
    setSliderPosition(0);

    connect(this, &QSlider::sliderPressed, this, &Seeker::slider_pressed);
    connect(this, &QSlider::sliderReleased, this, &Seeker::slider_released);
}

void Seeker::slider_pressed()
{
    dragging = true;
}

void Seeker::slider_released()
{
    dragging = false;
    emit changed_position(sliderPosition());
}
