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

#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <QObject>
#include <QString>

// The responsibility of this class is unify the interface and control to log
// messages from different systems.
class Logger : public QObject {
    Q_OBJECT
public:
    enum class Tag {
        core,
        engine
    };

    enum class Level {
        information,
        warning,
        critical
    };

    void info(Tag tag, const QString& message);
    void warn(Tag tag, const QString& message);
    void crit(Tag tag, const QString& message);
signals:
    void new_message(Tag tag, Level level, const QString& message);
};

#endif
