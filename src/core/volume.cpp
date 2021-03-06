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

Volume::Volume(double level, double minimum, double maximum)
    : level(level),
      minimum(minimum),
      maximum(maximum)
{
    if (minimum > maximum)
        throw std::invalid_argument("minimum > maximum");
    if (level < minimum)
        throw std::invalid_argument("level < minimum");
    if (level > maximum)
        throw std::invalid_argument("level > minimum");
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
