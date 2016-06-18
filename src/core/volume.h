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

// Describes a volume level.
class Volume {
public:
    // Create a new volume with current level set to max.
    Volume(double minimum = 0.0, double maximum = 1.0, double step = 0.1);

    // Increase volume level with one step.
    void increase();
    // Decrease volume level with one step.
    void decrease();
    // Set volume level to specified value. The level value is clamped within
    // minimum and maximum allowed value.
    void set_level(double level);

    double get_level() const;
    double get_minimum() const;
    double get_maximum() const;
private:
    double minimum, maximum, step, level;
};

#endif
