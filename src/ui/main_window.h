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

#ifndef MAIN_WINDOW_H_INCLUDED
#define MAIN_WINDOW_H_INCLUDED

#include "engine_output.h"

#include <QFileDialog>
#include <QMainWindow>

#include <memory>

class Database;
class Logger;
class Player;
class Song;

namespace Ui {
class mainWindow;
}

class Main_window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Main_window(std::unique_ptr<Database> db,
                         std::unique_ptr<Player> player,
                         std::shared_ptr<Logger> logger,
                         QWidget* parent = nullptr);
    ~Main_window();
signals:
    void song_created(const Song& song);
private slots:
    void open_song();
    void play();
    void pause();
    void show_engine_output();
private:
    void init();
    void create_song(const QUrl& url);
    void update_seeker();
    void update_status_bar(const Song& song);

    Ui::mainWindow* ui;
    std::unique_ptr<Database> db;
    std::unique_ptr<Player> player;
    std::shared_ptr<Logger> logger;
    Engine_output engine_output;
};

#endif
