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

#ifndef GSTREAMER_PIPELINE_H_INCLUDED
#define GSTREAMER_PIPELINE_H_INCLUDED

#include <gst/gst.h>

#include <QObject>

#include <memory>
#include <string>

class Logger;
class Volume;

// The responsibility of this class is to provide an interface to a GStreamer
// pipeline.
class GStreamer_pipeline : public QObject {
    Q_OBJECT
public:
    struct Gst_data {
        GstElement* pipeline;
        GstElement* source;
        GstElement* convert;
        GstElement* volume;
        GstElement* level;
        GstElement* sink;
        GstBus* bus;
        GstState state;
    };

    enum class State {
        initial,
        ready,
        paused,
        playing
    };

    // GStreamer callback data.
    struct User_data {
        GStreamer_pipeline* pipeline;
        std::shared_ptr<Logger> logger;
        GstElement* convert;
    };

    // Create a simple audio pipeline.
    static std::unique_ptr<GStreamer_pipeline> make(std::shared_ptr<Logger> logger);

    GStreamer_pipeline(const Gst_data& data, std::shared_ptr<Logger> logger);
    ~GStreamer_pipeline();

    // Set source URI. Must include protocol i.e. file://, http:// etc.
    void set_uri(const std::string& uri);
    // Set pipeline state.
    void set_state(State state);
    // Set volume level.
    void set_volume(const Volume& volume);
signals:
    void end_of_stream();
    void state_changed(State old_state, State new_state);
private:
    Gst_data data;
    std::shared_ptr<Logger> logger;
    User_data user_data;
};


#endif
