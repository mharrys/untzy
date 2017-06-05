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

#include "gstreamer_thread.h"

#include <memory>
#include <vector>

// The responsibility of this class is to provide an interface to a GStreamer
// pipeline.
class GStreamer_pipeline : public Gst_observer {
public:
    // Describes current state.
    enum class State {
        initial,
        ready,
        paused,
        playing,
        ended
    };

    // Describes message severity level.
    enum class Level {
        info,
        warning,
        error
    };

    struct Observer {
        virtual void state_changed(State old_state, State new_state) = 0;
        virtual void message(Level level, const std::string& msg) = 0;
    };

    // Create a simple audio pipeline.
    static std::unique_ptr<GStreamer_pipeline> make();

    GStreamer_pipeline(const Gst_data& data);
    ~GStreamer_pipeline();

    // Start listen to state changes.
    void add_observer(Observer* observer);
    // Stop listen to state changes.
    void remove_observer(Observer* observer);

    // Set source URL. Must include protocol i.e. file://, http://.
    void set_uri(const std::string& uri);
    // Set pipeline state.
    void set_state(State state);
    // Set volume level.
    void set_volume(double level);
protected:
    // GStreamer thread callbacks
    void end_of_stream() override;
    void state_changed(GstState old_state, GstState new_state) override;
    void message(Gst_level level, const std::string& msg) override;
private:
    void notify_state_changed(State old_state, State new_state);
    void notify_message(Level level, const std::string& msg);
    void notify(std::function<void(Observer*)> fn);

    Gst_data data;
    Gst_user_data user_data;

    std::vector<Observer*> observers;
};

#endif
