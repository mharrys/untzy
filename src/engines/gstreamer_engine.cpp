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

std::unique_ptr<GStreamer_engine> GStreamer_engine::make(std::shared_ptr<Logger> logger)
{
    if (!gst_is_initialized()) {
        GError *err = NULL;
        if (!gst_init_check(NULL, NULL, &err)) {
            auto reason = err ? err->message : tr("Unknown reason");
            logger->crit(
                Logger::Tag::engine,
                tr("Unable to initialize GStreamer library: %1").arg(reason));
            return nullptr;
        }
    }

    auto pipeline = GStreamer_pipeline::make(logger);
    if (!pipeline)
        return nullptr;

    return std::unique_ptr<GStreamer_engine>(
        new GStreamer_engine(std::move(pipeline)));
}

void GStreamer_engine::load(const std::string& uri)
{
    pipeline->set_uri(uri);
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
    pipeline->set_volume(volume);
}

GStreamer_engine::GStreamer_engine(std::unique_ptr<GStreamer_pipeline> pipeline)
    : pipeline(std::move(pipeline))
{
}
