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

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <QObject>

class Volume;

// The responsibility of this class is to provide an interface for a
// multimedia framework.
class Engine : public QObject {
    Q_OBJECT
public:
    // Describes current state.
    enum class State {
        ready,
        paused,
        playing,
        ended
    };

    // Describes message severity level.
    enum class Level {
        info,
        warning,
        error
    };

    virtual ~Engine() = default;
    // Prepare the pipeline with a local or remote url.
    virtual void load(const QUrl& url) = 0;
    // Start or continue playback.
    virtual void play() = 0;
    // Pause playback.
    virtual void pause() = 0;
    // Set volume level.
    virtual void set_volume(const Volume& volume) = 0;
signals:
    void new_state(State state);
    void new_message(Level level, const QString& msg);
};

#endif
