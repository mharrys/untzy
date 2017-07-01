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

#include "engine_output.h"
#include "ui_engine_output.h"

#include <QScrollBar>

Engine_output::Engine_output(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::engineOutputWidget)
{
    ui->setupUi(this);
}

Engine_output::~Engine_output()
{
    delete ui;
}

void Engine_output::new_message(Engine::Level level, const QString& msg)
{
    ui->outputText->insertPlainText(msg + "\n");
    auto sb = ui->outputText->verticalScrollBar();
    sb->setValue(sb->maximum());
}
