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

#ifndef ENGINE_OUTPUT_H_INCLUDED
#define ENGINE_OUTPUT_H_INCLUDED

#include "engines/engine.h"

#include <QWidget>

namespace Ui {
class Engine_output;
}

// The responsibility of this class is to display engine messages.
class Engine_output : public QWidget
{
    Q_OBJECT
public:
    explicit Engine_output(QWidget *parent = 0);
    ~Engine_output();
public slots:
    void new_message(Engine::Level level, const QString& msg);
private:
    Ui::Engine_output* ui;
};

#endif
