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

#include "volume_slider.h"

Volume_slider::Volume_slider(QWidget* parent)
    : QSlider(parent),
      volume(0.5)
{
    init();
}

void Volume_slider::init()
{
    setMinimum(0);
    setMaximum(100);
    setOrientation(Qt::Horizontal);
    setMinimumWidth(100);
    setMaximumWidth(100);
    connect(this, &QSlider::valueChanged, this, &Volume_slider::value_changed);
    setValue(volume.get_level() * maximum());
}

void Volume_slider::value_changed(int value)
{
    auto real_value = static_cast<double>(value);
    auto level = real_value / maximum();
    emit changed_volume(Volume(level));
}
