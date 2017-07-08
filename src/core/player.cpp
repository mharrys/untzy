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

#include "core/volume.h"
#include "core/song.h"
#include "engines/engine.h"

#include <QUrl>

Player::Player(std::unique_ptr<Engine> engine, QObject* parent)
    : engine(std::move(engine)),
      QObject(parent)
{
}

void Player::load(const Song& song)
{
    engine->load(song.get_source());
}

void Player::play()
{
    engine->play();
    emit playing();
}

void Player::pause()
{
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
