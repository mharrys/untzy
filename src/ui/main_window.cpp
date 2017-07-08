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
#include "core/song.h"

#include <QMessageBox>
#include <QStandardPaths>

Main_window::Main_window(std::unique_ptr<Player> player,
                         std::shared_ptr<Logger> logger,
                         QWidget* parent) :
    player(std::move(player)),
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    connect(ui->openFileAction,  &QAction::triggered, this, &Main_window::open_file);
    connect(ui->exitAction,  &QAction::triggered, this, &Main_window::close);
    connect(this->player.get(), &Player::playing, [=]() {
        setWindowTitle(current_song.baseName());
    });
    connect(ui->playButton,  &QPushButton::clicked, this, &Main_window::play);
    connect(ui->pauseButton, &QPushButton::clicked, this, &Main_window::pause);
    connect(ui->incVolumeButton, &QPushButton::clicked, this, &Main_window::increase_volume);
    connect(ui->decVolumeButton, &QPushButton::clicked, this, &Main_window::decrease_volume);
    connect(ui->viewEngineOutputAction, &QAction::triggered, this, &Main_window::show_engine_output);
    connect(logger.get(), &Logger::new_message, [=](Logger::Tag, Logger::Level level, const QString& msg) {
        auto title = "Untzy";
        if (level == Logger::Level::warning)
            QMessageBox::warning(NULL, title, msg);
        else if (level == Logger::Level::critical)
            QMessageBox::critical(NULL, title, msg);
    });
    connect(this->player->get_engine(), &Engine::new_message, &engine_output, &Engine_output::new_message);
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
        try {
            player->load(Song::make(url));
        } catch (std::runtime_error& e) {
            auto msg = QObject::tr("Unable to load song. Reason: %1").arg(e.what());
            QMessageBox::warning(NULL, "Untzy", msg);
        }
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
    volume.increase();
    player->set_volume(volume);
}

void Main_window::decrease_volume()
{
    volume.decrease();
    player->set_volume(volume);
}

void Main_window::show_engine_output()
{
    engine_output.show();
}
