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

#include <QObject>
#include <QUrl>

#include <memory>

class Engine;
class Volume;

// The responsibility of this class is provide an interface to interact with
// the audio engine.
class Player : public QObject {
    Q_OBJECT
public:
    explicit Player(QObject* parent = nullptr);
public slots:
    virtual void load(const QUrl& url) = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void set_volume(const Volume& volume) = 0;
    virtual Engine* get_engine() = 0;
signals:
    void playing();
    void paused();
    void volume_changed(const Volume& volume);
};

class Player_impl : public Player {
public:
    explicit Player_impl(std::unique_ptr<Engine> engine, QObject* parent = nullptr);
    void load(const QUrl& url);
    void play() final;
    void pause() final;
    void set_volume(const Volume& volume) final;
    Engine* get_engine() final;
private:
    std::unique_ptr<Engine> engine;
};

#endif
