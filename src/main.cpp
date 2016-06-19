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

#include "core/logger.h"
#include "core/player.h"
#include "engines/gstreamer_engine.h"
#include "ui/main_window.h"

#include <QApplication>
#include <QMessageBox>

void init_msg_box(Logger::Tag tag, Logger::Level level, const QString& msg)
{
    if (level > Logger::Level::warning)
        QMessageBox::critical(NULL, "Untzy", msg);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto logger = std::make_shared<Logger>();
    // Displays message box during the initialization phase.
    auto conn = QObject::connect(logger.get(), &Logger::new_message, &init_msg_box);
    auto engine = GStreamer_engine::make(logger);
    if (!engine)
        return 1;
    auto player = std::make_unique<Player_impl>(std::move(engine));
    QObject::disconnect(conn);

    Main_window main_window(std::move(player), logger);
    main_window.show();

    return app.exec();
}
