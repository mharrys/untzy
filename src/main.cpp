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

#include "core/player.h"
#include "engines/gstreamer_engine.h"
#include "ui/main_window.h"

#include <QApplication>
#include <QMessageBox>

#include <iostream>

int main(int argc, char* argv[])
{
    auto engine = GStreamer_engine::make();
    if (!engine) {
        std::cerr << "untzy: Unable to create GStreamer engine.\n";
        return 1;
    }
    auto player = std::make_unique<Player_impl>(std::move(engine));

    QApplication app(argc, argv);

    Main_window main_window(std::move(player));
    main_window.show();

    return app.exec();
}
