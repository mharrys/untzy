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

#include "core/logger.h"
#include "core/player.h"
#include "core/song.h"

#include <QMessageBox>
#include <QStandardPaths>

Main_window::Main_window(std::unique_ptr<Player> player,
                         std::shared_ptr<Logger> logger,
                         QWidget* parent)
    : QMainWindow(parent),
      player(std::move(player)),
      logger(logger),
      ui(new Ui::mainWindow)
{
    init();
}

Main_window::~Main_window()
{
    delete ui;
}

void Main_window::open_song()
{
    auto dir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    auto filename = QFileDialog::getOpenFileName(this, tr("Open File"), dir);
    if (!filename.isEmpty()) {
        current_song = QFileInfo(filename);
        auto url = QUrl::fromLocalFile(current_song.absoluteFilePath());
        create_song(url);
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

void Main_window::init()
{
    ui->setupUi(this);

    connect(ui->openFileAction,  &QAction::triggered, this, &Main_window::open_song);
    connect(ui->exitAction,  &QAction::triggered, this, &Main_window::close);
    connect(ui->playButton,  &QPushButton::clicked, this, &Main_window::play);
    connect(ui->pauseButton, &QPushButton::clicked, this, &Main_window::pause);
    connect(ui->incVolumeButton, &QPushButton::clicked, this, &Main_window::increase_volume);
    connect(ui->decVolumeButton, &QPushButton::clicked, this, &Main_window::decrease_volume);
    connect(ui->viewEngineOutputAction, &QAction::triggered, this, &Main_window::show_engine_output);

    // present logged messages with a GUI instead of to terminal
    connect(logger.get(), &Logger::new_message, [=](Logger::Tag, Logger::Level level, const QString& msg) {
        auto title = "Untzy";
        if (level == Logger::Level::warning)
            QMessageBox::warning(NULL, title, msg);
        else if (level == Logger::Level::critical)
            QMessageBox::critical(NULL, title, msg);
    });

    // change window title whenever a new song is playing
    connect(player.get(), &Player::playing, [=]() {
        setWindowTitle(current_song.baseName());
    });

    // view all messages produced by the audio engine
    connect(player->get_engine(), &Engine::new_message, &engine_output, &Engine_output::new_message);

    // user opened new song through file browser
    connect(this, &Main_window::song_created, [=](const Song& song) {
        ui->playlistWidget->append_song(song);
        player->load(song);
        player->play();
    });

    // user double clicks on song in playlist
    connect(ui->playlistWidget, &Playlist_widget::select_song, [=](const Song& song) {
        player->load(song);
        player->play();
    });
}

void Main_window::create_song(const QUrl& url)
{
    try {
        auto song = Song::make(url);
        emit song_created(song);
    } catch (std::runtime_error& e) {
        auto msg = QObject::tr("Unable to load song \"%1\". Reason: %2");
        auto msgArgs = msg.arg(url.toString(), e.what());
        logger->warn(Logger::Tag::core, msgArgs);
    }
}
