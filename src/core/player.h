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

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "song.h"

#include <QObject>
#include <QTimer>

#include <memory>

class Engine;
class Volume;

class QUrl;

// The responsibility of this class is provide an interface to interact with
// the audio engine.
class Player : public QObject {
    Q_OBJECT
public:
    explicit Player(std::unique_ptr<Engine> engine, QObject* parent = nullptr);
public slots:
    void load(const Song& song);
    void play();
    void pause();
    void set_volume(const Volume& volume);
    Engine* get_engine();
signals:
    void playing(const Song& song);
    void paused();
    void volume_changed(const Volume& volume);
    // Emitted in 2 Hz when a song is played (will not stop on pause).
    void progress(const Song& song);
private:
    std::unique_ptr<Engine> engine;
    bool song_loaded;
    Song current_song;
    QTimer progress_timer;
};

#endif
