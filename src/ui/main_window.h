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

#include "core/logger.h"
#include "core/volume.h"

#include <QFileDialog>
#include <QMainWindow>

#include <memory>

class Player;

namespace Ui {
class MainWindow;
}

class Main_window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Main_window(
        std::unique_ptr<Player> player,
        std::shared_ptr<Logger> logger,
        QWidget* parent = nullptr);
    ~Main_window();
private slots:
    void open_file();
    void play();
    void pause();
    void increase_volume();
    void decrease_volume();
private:
    Ui::MainWindow* ui;
    std::unique_ptr<Player> player;
    QFileInfo current_song;
    Volume volume;
};

#endif
