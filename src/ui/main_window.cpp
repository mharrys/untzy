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

#include <QFileDialog>
#include <QStandardPaths>

Main_window::Main_window(std::unique_ptr<Player> player, QWidget* parent)
    : player(std::move(player)),
      QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_open, SIGNAL(triggered()), SLOT(open_file_dialog()));
    connect(ui->action_exit, SIGNAL(triggered()), SLOT(close()));
}

Main_window::~Main_window()
{
    delete ui;
}

void Main_window::open_file_dialog()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    QString filename = QFileDialog::getOpenFileName(this, tr("Open"), dir);
    if (!filename.isEmpty()) {
        QUrl url = QUrl::fromLocalFile(QFileInfo(filename).absoluteFilePath());
        player->load(url);
        player->play();
    }
}
