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

#include "gstreamer_pipeline.h"

#include <sstream>

std::unique_ptr<GStreamer_pipeline> GStreamer_pipeline::make()
{
    if (!gst_is_initialized()) {
        GError *err = NULL;
        if (!gst_init_check(NULL, NULL, &err)) {
            std::stringstream ss;
            ss << "Unable to initialize GStreamer library: " << (err ? err->message : "Unknown reason");
            throw std::runtime_error(ss.str());
        }
    }

    auto elem_err = [](const std::string& elem)
    {
        const auto elem_crit = "Unable to create GStreamer element: ";
        throw std::runtime_error(elem_crit + elem);
    };

    Gst_data data;
    data.source = gst_element_factory_make("uridecodebin", "source");
    if (!data.source)
        elem_err("uridecodebin");

    data.convert = gst_element_factory_make("audioconvert", "convert");
    if (!data.convert)
        elem_err("audioconvert");

    data.volume = gst_element_factory_make("volume", "volume");
    if (!data.volume)
        elem_err("volume");

    data.level = gst_element_factory_make("level", "level");
    if (!data.level)
        elem_err("level");

    data.sink = gst_element_factory_make("autoaudiosink", "sink");
    if (!data.sink)
        elem_err("autoaudiosink");

    data.pipeline = gst_pipeline_new("pipeline");
    if (!data.pipeline)
        elem_err("pipeline");

    gst_bin_add_many(
        GST_BIN(data.pipeline),
        data.source,
        data.convert,
        data.volume,
        data.level,
        data.sink,
        NULL);

    if (!gst_element_link_many(data.convert, data.volume, data.level, data.sink, NULL)) {
        gst_object_unref(data.pipeline);
        throw std::runtime_error("Unable to link elements in GStreamer pipeline");
    }

    data.bus = gst_element_get_bus(data.pipeline);
    data.state = GST_STATE_NULL;
    return std::make_unique<GStreamer_pipeline>(data);
}

GStreamer_pipeline::GStreamer_pipeline(const Gst_data& data)
    : data(data)
{
    user_data = {data.convert, {this}};
    g_signal_connect(this->data.source, "pad-added", G_CALLBACK(gst_pad_added_handler), &user_data);
    gst_bus_set_sync_handler(this->data.bus, gst_bus_sync_handler, &user_data, NULL);
}

GStreamer_pipeline::~GStreamer_pipeline()
{
    gst_bus_set_sync_handler(data.bus, NULL, NULL, NULL);
    gst_object_unref(data.bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
}

// Start listen to state changes.
void GStreamer_pipeline::add_observer(Observer* observer)
{
    observers.push_back(observer);
}

// Stop listen to state changes.
void GStreamer_pipeline::remove_observer(Observer* observer)
{
    for (auto it = observers.begin(); it != observers.end(); it++) {
        if (*it == observer) {
            observers.erase(it);
            break;
        }
    }
}

void GStreamer_pipeline::set_uri(const std::string& uri)
{
    if (data.state >= GST_STATE_READY)
        set_state(State::ready);
    g_object_set(data.source, "uri", uri.c_str(), NULL);
}

void GStreamer_pipeline::set_state(State state)
{
    auto translate_state = [&state]() {
        switch (state) {
        case State::ready:
            return GST_STATE_READY;
        case State::paused:
            return GST_STATE_PAUSED;
        case State::playing:
            return GST_STATE_PLAYING;
        case State::initial:
        default:
            return GST_STATE_NULL;
        }
    };
    auto new_state = translate_state();
    GstStateChangeReturn ret = gst_element_set_state(data.pipeline, new_state);
    if (ret != GST_STATE_CHANGE_FAILURE)
        data.state = new_state;
}

void GStreamer_pipeline::set_volume(double level )
{
    g_object_set(data.volume, "volume", level, NULL);
}

void GStreamer_pipeline::end_of_stream()
{
    notify_state_changed(State::playing, State::ended);
}

static GStreamer_pipeline::State translate_state(GstState state)
{
    switch (state) {
    case GST_STATE_READY:
        return GStreamer_pipeline::State::ready;
    case GST_STATE_PAUSED:
        return GStreamer_pipeline::State::paused;
    case GST_STATE_PLAYING:
        return GStreamer_pipeline::State::playing;
    case GST_STATE_NULL:
    default:
        return GStreamer_pipeline::State::initial;
    }
};

void GStreamer_pipeline::state_changed(GstState old_state, GstState new_state)
{
    notify_state_changed(translate_state(old_state), translate_state(new_state));
}

void GStreamer_pipeline::notify_state_changed(State old_state, State new_state)
{
    notify([old_state, new_state](Observer* observer) {
        observer->state_changed(old_state, new_state);
    });
}

static GStreamer_pipeline::Level translate_level(Gst_level level)
{
    switch (level) {
        case Gst_level::info:
            return GStreamer_pipeline::Level::info;
        case Gst_level::warning:
            return GStreamer_pipeline::Level::warning;
        case Gst_level::error:
            return GStreamer_pipeline::Level::error;
    }
}

void GStreamer_pipeline::message(Gst_level level, const std::string& msg)
{
    notify_message(translate_level(level), msg);
}

void GStreamer_pipeline::notify_message(Level level, const std::string& msg)
{
    notify([level, msg](Observer* observer) {
        observer->message(level, msg);
    });
}

void GStreamer_pipeline::notify(std::function<void(Observer*)> fn)
{
    for (auto it = observers.begin(); it != observers.end();) {
        if (*it == nullptr)
            it = observers.erase(it);
        else {
            fn(*it);
            it++;
        }
    }
}
