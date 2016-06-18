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

#include "volume.h"

#include <algorithm>
#include <stdexcept>

Volume::Volume(double minimum, double maximum, double step)
    : minimum(minimum),
      maximum(maximum),
      step(step),
      level(maximum)
{
    if (minimum > maximum)
        throw std::invalid_argument("minimum > maximum");
}

void Volume::increase()
{
    set_level(level + step);
}

void Volume::decrease()
{
    set_level(level - step);
}

void Volume::set_level(double level)
{
    this->level = std::min(maximum, std::max(minimum, level));
}

double Volume::get_level() const
{
    return level;
}

double Volume::get_minimum() const
{
    return minimum;
}

double Volume::get_maximum() const
{
    return maximum;
}
