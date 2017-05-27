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

#include "gstreamer_engine.h"

#include "core/logger.h"
#include "core/volume.h"

#include <QUrl>

std::unique_ptr<GStreamer_engine> GStreamer_engine::make(std::shared_ptr<Logger> logger)
{
    try {
        auto pipeline = GStreamer_pipeline::make();
        return std::make_unique<GStreamer_engine>(std::move(pipeline));
    } catch (std::runtime_error& e) {
        throw e;
    }
}

GStreamer_engine::GStreamer_engine(std::unique_ptr<GStreamer_pipeline> pipeline)
    : pipeline(std::move(pipeline))
{
}

void GStreamer_engine::load(const QUrl& url)
{
    pipeline->set_uri(url.toString().toStdString());
}

void GStreamer_engine::play()
{
    pipeline->set_state(GStreamer_pipeline::State::playing);
}

void GStreamer_engine::pause()
{
    pipeline->set_state(GStreamer_pipeline::State::paused);
}

void GStreamer_engine::set_volume(const Volume& volume)
{
    pipeline->set_volume(volume.get_level());
}
