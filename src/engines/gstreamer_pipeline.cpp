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

namespace {

    GstBusSyncReply bus_sync_handler(GstBus* bus, GstMessage* message, gpointer user_data)
    {
        GError* error;
        gchar* dbg_info;
        GstState old_state, new_state;
        switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_INFO:
            gst_message_parse_info(message, &error, &dbg_info);
            g_print("gstreamer: Info from element %s: %s\n", GST_OBJECT_NAME(message->src), error->message);
            g_print("gstreamer: Debugging info: %s\n", dbg_info ? dbg_info : "none");
            g_error_free(error);
            g_free(dbg_info);
            break;
        case GST_MESSAGE_WARNING:
            gst_message_parse_warning(message, &error, &dbg_info);
            g_print("gstreamer: Warning from element %s: %s\n", GST_OBJECT_NAME(message->src), error->message);
            g_print("gstreamer: Debugging info: %s\n", dbg_info ? dbg_info : "none");
            g_error_free(error);
            g_free(dbg_info);
            break;
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(message, &error, &dbg_info);
            g_printerr("gstreamer: Error from element %s: %s\n", GST_OBJECT_NAME(message->src), error->message);
            g_printerr("gstreamer: Debugging info: %s\n", dbg_info ? dbg_info : "none");
            g_error_free(error);
            g_free(dbg_info);
            break;
        case GST_MESSAGE_EOS:
            g_print("gstreamer: EOS\n");
            break;
        case GST_MESSAGE_STATE_CHANGED:
            gst_message_parse_state_changed(message, &old_state, &new_state, NULL);
            g_print(
                "gstreamer: Element %s changed state from %s to %s.\n",
                GST_OBJECT_NAME(message->src),
                gst_element_state_get_name(old_state),
                gst_element_state_get_name(new_state));
            break;
        }
        return GST_BUS_PASS;
    }

    void pad_added_handler(GstElement* src, GstPad* new_pad, GstElement* user_data)
    {
        GstPad* sink_pad = gst_element_get_static_pad(user_data, "sink");

        if (gst_pad_is_linked(sink_pad)) {
            gst_object_unref(sink_pad);
            g_print(
                "gstreamer: Attempted to link new pad \"%s\" from \"%s\", but already linked.\n",
                GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src));
            return;
        }

        GstCaps* new_pad_caps = gst_pad_get_current_caps(new_pad);
        GstStructure* new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
        const gchar* new_pad_type = gst_structure_get_name(new_pad_struct);
        if (!g_str_has_prefix(new_pad_type, "audio/x-raw")) {
            g_print("gstreamer: New pad \"%s\" from \"%s\" is not raw audio.\n", new_pad_type);
            gst_caps_unref(new_pad_caps);
            gst_object_unref(sink_pad);
            return;
        }

        GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
        if (GST_PAD_LINK_FAILED(ret)) {
            g_print(
                "gstreamer: Failed to link new pad \"%s\" from \"%s\" with type \"%s\".\n",
                GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src), new_pad_type);
        } else {
            g_print(
                "gstreamer: Successfully linked new pad \"%s\" from \"%s\" with type \"%s\".\n",
                GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src), new_pad_type);
        }
    }

}

std::unique_ptr<GStreamer_pipeline> GStreamer_pipeline::make()
{
    if (!gst_is_initialized()) {
        g_printerr("gstreamer: Unable to create pipeline, GStreamer is not initialized.\n");
        return nullptr;
    }
    Gst_data data;
    data.source = gst_element_factory_make("uridecodebin", "source");
    if (!data.source) {
        g_printerr("gstreamer: Unable to create uridecodebin.\n");
        return nullptr;
    }
    data.convert = gst_element_factory_make("audioconvert", "convert");
    if (!data.convert) {
        g_printerr("gstreamer: Unable to create audioconvert.\n");
        return nullptr;
    }
    data.sink = gst_element_factory_make("autoaudiosink", "sink");
    if (!data.sink) {
        g_printerr("gstreamer: Unable to create autoaudiosink.\n");
        return nullptr;
    }
    data.pipeline = gst_pipeline_new("pipeline");
    if (!data.pipeline) {
        g_printerr("gstreamer: Unable to create new pipeline.\n");
        return nullptr;
    }
    gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.convert , data.sink, NULL);
    if (!gst_element_link(data.convert, data.sink)) {
        g_printerr("gstreamer: Unable to link elements in pipeline.\n");
        gst_object_unref(data.pipeline);
        return nullptr;
    }
    data.bus = gst_element_get_bus(data.pipeline);
    data.state = GST_STATE_NULL;
    return std::unique_ptr<GStreamer_pipeline>(
        new GStreamer_pipeline(data));
}

GStreamer_pipeline::~GStreamer_pipeline()
{
    gst_bus_set_sync_handler(data.bus, NULL, NULL, NULL);
    gst_object_unref(data.bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
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
    if (ret == GST_STATE_CHANGE_FAILURE)
        g_printerr("gstreamer: Unable to set pipeline state.\n");
    else
        data.state = new_state;
}

GStreamer_pipeline::GStreamer_pipeline(const Gst_data& data)
    : data(data)
{
    g_signal_connect(this->data.source, "pad-added", G_CALLBACK(pad_added_handler), this->data.convert);
    gst_bus_set_sync_handler(this->data.bus, bus_sync_handler, this, NULL);
}
