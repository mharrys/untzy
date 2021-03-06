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

#include "player.h"

#include "volume.h"
#include "engines/engine.h"

#include <QUrl>

Player::Player(std::unique_ptr<Engine> engine, QObject* parent)
    : engine(std::move(engine)),
      song_loaded(false),
      QObject(parent)
{
    // update seeker position
    connect(&progress_timer, &QTimer::timeout, [=]() {
        emit progress(current_song);
    });
}

void Player::load(const Song& song)
{
    song_loaded = true;
    current_song = song;
    engine->load(song.get_source());
}

void Player::play()
{
    if (!song_loaded)
        return;

    engine->play();
    emit playing(current_song);
    // start ticking at 2 Hz
    progress_timer.start(500);
    // one tick immediately
    emit progress(current_song);
}

void Player::pause()
{
    if (!song_loaded)
        return;

    engine->pause();
    emit paused();
}

void Player::set_volume(const Volume& volume)
{
    engine->set_volume(volume);
    emit volume_changed(volume);
}

Engine* Player::get_engine()
{
    return engine.get();
}
