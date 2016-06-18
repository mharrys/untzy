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

#include "main_window.h"
#include "ui_main_window.h"

#include "core/player.h"

#include <QStandardPaths>

Main_window::Main_window(std::unique_ptr<Player> player, QWidget* parent)
    : player(std::move(player)),
      QMainWindow(parent),
      ui(new Ui::MainWindow),
      volume_level(1.0)
{
    ui->setupUi(this);

    connect(ui->action_open,  &QAction::triggered, this, &Main_window::open_file);
    connect(ui->action_play,  &QAction::triggered, this, &Main_window::play);
    connect(ui->action_pause, &QAction::triggered, this, &Main_window::pause);
    connect(ui->action_exit,  &QAction::triggered, this, &Main_window::close);

    connect(this->player.get(), &Player::playing, [=]() {
        ui->songLabel->setText("(Playing) " + current_song.baseName());
    });
    connect(this->player.get(), &Player::paused, [=]() {
        ui->songLabel->setText("(Paused) " + current_song.baseName());
    });

    connect(ui->inc_vol, &QPushButton::clicked, this, &Main_window::increase_volume);
    connect(ui->dec_vol, &QPushButton::clicked, this, &Main_window::decrease_volume);
}

Main_window::~Main_window()
{
    delete ui;
}

void Main_window::open_file()
{
    auto dir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    auto filename = QFileDialog::getOpenFileName(this, tr("Open File"), dir);
    if (!filename.isEmpty()) {
        current_song = QFileInfo(filename);
        auto url = QUrl::fromLocalFile(current_song.absoluteFilePath());
        player->load(url);
    }
}

void Main_window::play()
{
    player->play();
}

void Main_window::pause()
{
    player->pause();
}

void Main_window::increase_volume()
{
    volume_level = std::min(1.0, volume_level + 0.1);
    player->set_volume(volume_level);
}

void Main_window::decrease_volume()
{
    volume_level = std::max(0.0, volume_level - 0.1);
    player->set_volume(volume_level);
}
