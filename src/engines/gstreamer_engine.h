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

#ifndef GSTREAMER_ENGINE_H_INCLUDED
#define GSTREAMER_ENGINE_H_INCLUDED

#include "engine.h"
#include "engines/gstreamer_pipeline.h"

#include <memory>

class Volume;

// The responsibility of this class is to provide an interface to the
// GStreamer multimedia framework.
class GStreamer_engine : public Engine, GStreamer_pipeline::Observer {
public:
    // Create a simple audio engine.
    static std::unique_ptr<GStreamer_engine> make();

    GStreamer_engine(std::unique_ptr<GStreamer_pipeline> pipeline);

    void load(const QUrl& url) final;
    void play() final;
    void pause() final;
    void set_volume(const Volume& volume) final;
    void set_seek_position(long seek_pos) final;
    long get_seek_position() final;
private:
    void state_changed(GStreamer_pipeline::State old_state,
                       GStreamer_pipeline::State new_state);
    void message(GStreamer_pipeline::Level level, const std::string& msg);

    std::unique_ptr<GStreamer_pipeline> pipeline;
};

#endif
