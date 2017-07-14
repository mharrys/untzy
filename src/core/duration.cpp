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

#include "duration.h"

#include <QString>

#include <sstream>

Duration::Duration()
    : unknown(true),
      hours(0),
      minutes(0),
      seconds(0)
{
}

Duration::Duration(time_unit length)
    : unknown(false)
{
    seconds = length % 60;
    time_unit temp_min = (length - seconds) / 60;
    minutes = temp_min % 60;
    hours = (temp_min - minutes) / 60;
}

Duration::Duration(time_unit hours, time_unit minutes, time_unit seconds)
    : unknown(false),
      hours(hours),
      minutes(minutes),
      seconds(seconds)
{
}

bool Duration::is_unknown() const
{
    return unknown;
}

time_unit Duration::get_hours() const
{
    return hours;
}

time_unit Duration::get_minutes() const
{
    return minutes;
}

time_unit Duration::get_seconds() const
{
    return seconds;
}

QString Duration::get_str() const
{
    if (unknown)
        return QString("?");

    std::stringstream ss;

    if (hours > 0) {
        ss << hours;
        ss << ":";
    }

    ss << minutes;
    ss << ":";

    if (seconds < 10)
        ss << "0";
    ss << seconds;

    return QString::fromStdString(ss.str());
}
