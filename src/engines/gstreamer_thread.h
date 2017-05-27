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

#ifndef GSTREAMER_THREAD_H_INCLUDED
#define GSTREAMER_THREAD_H_INCLUDED

#include <gst/gst.h>

#include <vector>
#include <string>

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

enum class Gst_level {
    info,
    warning,
    error
};

struct Gst_observer {
    virtual void end_of_stream() = 0;
    virtual void state_changed(GstState old_state, GstState new_state) = 0;
    virtual void message(Gst_level level, const std::string& msg) = 0;
};

// GStreamer callback data.
struct Gst_user_data {
    GstElement* convert;
    std::vector<Gst_observer*> observers;
};

GstBusSyncReply gst_bus_sync_handler(GstBus* bus,
                                     GstMessage* message,
                                     gpointer user_data_ptr);

void gst_pad_added_handler(GstElement* src,
                           GstPad* new_pad,
                           Gst_user_data* user_data);

#endif
