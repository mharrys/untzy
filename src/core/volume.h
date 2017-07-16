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

#ifndef VOLUME_H_INCLUDED
#define VOLUME_H_INCLUDED

// Describes the audio volume level.
class Volume {
public:
    // Create a new volume level.
    Volume(double level, double minimum = 0.0, double maximum = 1.0);

    double get_level() const;
    double get_minimum() const;
    double get_maximum() const;
private:
    double minimum, maximum, level;
};

#endif
