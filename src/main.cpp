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
#include "db/database.h"
#include "engines/gstreamer_engine.h"
#include "ui/main_window.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    std::unique_ptr<Database> db;
    try {
        db = Database::make();
    } catch (std::runtime_error& e) {
        auto msg = QObject::tr("Unable to initialize database. Reason: %1").arg(e.what());
        QMessageBox::critical(NULL, "Untzy", msg);
        return 1;
    }

    std::unique_ptr<Engine> engine;
    try {
        engine = GStreamer_engine::make();
    } catch (std::runtime_error& e) {
        auto msg = QObject::tr("Unable to initialize GStreamer. Reason: %1").arg(e.what());
        QMessageBox::critical(NULL, "Untzy", msg);
        return 1;
    }

    auto player = std::make_unique<Player>(std::move(engine));
    auto logger = std::make_shared<Logger>();
    Main_window main_window(std::move(db), std::move(player), logger);
    main_window.show();

    return app.exec();
}
