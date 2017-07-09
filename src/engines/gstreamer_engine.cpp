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

#include "core/volume.h"

#include <QUrl>

std::unique_ptr<GStreamer_engine> GStreamer_engine::make()
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
    qRegisterMetaType<Engine::Level>("Level");
    this->pipeline->add_observer(this);
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

void GStreamer_engine::state_changed(GStreamer_pipeline::State,
                                     GStreamer_pipeline::State state)
{
    // only send available states
    if (state == GStreamer_pipeline::State::ready)
        emit new_state(State::ready);
    else if (state == GStreamer_pipeline::State::paused)
        emit new_state(State::paused);
    else if (state == GStreamer_pipeline::State::playing)
        emit new_state(State::playing);
    else if (state == GStreamer_pipeline::State::ended)
        emit new_state(State::ended);
}

static Engine::Level translate_level(GStreamer_pipeline::Level level)
{
    switch (level) {
    case GStreamer_pipeline::Level::info:
        return Engine::Level::info;
    case GStreamer_pipeline::Level::warning:
        return Engine::Level::warning;
    case GStreamer_pipeline::Level::error:
        return Engine::Level::error;
    }
}

void GStreamer_engine::message(GStreamer_pipeline::Level level, const std::string& msg)
{
    emit new_message(translate_level(level), QString::fromStdString(msg));
}
