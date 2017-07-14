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

#ifndef DURATION_H_INCLUDED
#define DURATION_H_INCLUDED

class QString;

typedef long time_unit;

// Describes the audio duration.
class Duration {
public:
    // Create unknown duration.
    Duration();

    // Create duration from total length in seconds.
    Duration(time_unit length);

    // Create duration from specified number of hours, minutes and seconds.
    explicit Duration(time_unit hours, time_unit minutes, time_unit seconds);

    // Validate if the duration is not known i.e. it is a stream.
    bool is_unknown() const;

    time_unit get_hours() const;
    time_unit get_minutes() const;
    time_unit get_seconds() const;

    // Return string representation of the duration.
    QString get_str() const;
private:
    bool unknown;
    time_unit hours, minutes, seconds;
};

#endif
