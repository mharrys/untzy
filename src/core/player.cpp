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

#include "engines/engine.h"

Player::Player(QObject* parent)
    : QObject(parent)
{
}

Player_impl::Player_impl(std::unique_ptr<Engine> engine, QObject* parent)
    : engine(std::move(engine)),
      Player(parent)
{
}

void Player_impl::load(QUrl const & url)
{
    engine->load(url.toString().toStdString());
}

void Player_impl::play()
{
    engine->play();
    emit playing();
}

void Player_impl::pause()
{
    engine->pause();
    emit paused();
}

void Player_impl::set_volume(double level)
{
    engine->set_volume(level);
}
