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

#include "gstreamer_thread.h"

#include <functional>
#include <sstream>

static void notify(Gst_user_data &user_data, std::function<void(Gst_observer* observer)> fn)
{
    for (auto it = user_data.observers.begin(); it != user_data.observers.end();) {
        if (*it == nullptr)
            it = user_data.observers.erase(it);
        else {
            fn(*it);
            it++;
        }
    }
}

static void notify_end_of_stream(Gst_user_data &user_data)
{
    notify(user_data, [](Gst_observer* observer) {
        observer->end_of_stream();
    });
}

static void notify_state_changed(Gst_user_data &user_data, GstState old_state, GstState new_state)
{
    notify(user_data, [old_state, new_state](Gst_observer* observer) {
        observer->state_changed(old_state, new_state);
    });
}

static void notify_message(Gst_user_data &user_data, Gst_level level, const std::string& msg)
{
    notify(user_data, [level, msg](Gst_observer* observer) {
        observer->message(level, msg);
    });
}

GstBusSyncReply gst_bus_sync_handler(GstBus* bus, GstMessage* message, gpointer user_data_ptr)
{
    auto user_data = reinterpret_cast<Gst_user_data*>(user_data_ptr);
    GError* error;
    gchar* dbg_info;
    GstState old_state, new_state;
    std::stringstream ss;
    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_INFO:
        gst_message_parse_info(message, &error, &dbg_info);
        ss << "Info from element " << GST_OBJECT_NAME(message->src) << ": " << error->message;
        notify_message(*user_data, Gst_level::info, ss.str());
        ss.str(std::string());
        ss << "Debugging info: " << (dbg_info ? dbg_info : "None");
        notify_message(*user_data, Gst_level::info, ss.str());
        g_error_free(error);
        g_free(dbg_info);
        break;
    case GST_MESSAGE_WARNING:
        gst_message_parse_warning(message, &error, &dbg_info);
        ss << "Warning from element " << GST_OBJECT_NAME(message->src) << ": " << error->message;
        notify_message(*user_data, Gst_level::warning, ss.str());
        ss.str(std::string());
        ss << "Debugging info: " << (dbg_info ? dbg_info : "None");
        notify_message(*user_data, Gst_level::info, ss.str());
        g_error_free(error);
        g_free(dbg_info);
        break;
    case GST_MESSAGE_ERROR:
        gst_message_parse_error(message, &error, &dbg_info);
        ss << "Error from element " << GST_OBJECT_NAME(message->src) << ": " << error->message;
        notify_message(*user_data, Gst_level::error, ss.str());
        ss.str(std::string());
        ss << "Debugging info: " << (dbg_info ? dbg_info : "None");
        notify_message(*user_data, Gst_level::info, ss.str());
        g_error_free(error);
        g_free(dbg_info);
        break;
    case GST_MESSAGE_EOS:
        notify_message(*user_data, Gst_level::info, "End of stream");
        notify_end_of_stream(*user_data);
        break;
    case GST_MESSAGE_STATE_CHANGED:
        gst_message_parse_state_changed(message, &old_state, &new_state, NULL);
        ss << "Element " << GST_OBJECT_NAME(message->src) << " changed state from " << gst_element_state_get_name(old_state) << " to " << gst_element_state_get_name(new_state);
        notify_message(*user_data, Gst_level::info, ss.str());
        notify_state_changed(*user_data, old_state, new_state);
        break;
    }
    return GST_BUS_PASS;
}

void gst_pad_added_handler(GstElement* src, GstPad* new_pad, Gst_user_data* user_data)
{
    std::stringstream ss;
    GstPad* sink_pad = gst_element_get_static_pad(user_data->convert, "sink");

    if (gst_pad_is_linked(sink_pad)) {
        gst_object_unref(sink_pad);
        ss << "Attempted to link new pad \"" << GST_PAD_NAME(new_pad) << "\" from \"" << GST_ELEMENT_NAME(src) << "\", but already linked";
        notify_message(*user_data, Gst_level::info, ss.str());
        return;
    }

    GstCaps* new_pad_caps = gst_pad_get_current_caps(new_pad);
    GstStructure* new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    const gchar* new_pad_type = gst_structure_get_name(new_pad_struct);
    if (!g_str_has_prefix(new_pad_type, "audio/x-raw")) {
        ss << "New pad \"" << GST_PAD_NAME(new_pad) << "\" from \"" << GST_ELEMENT_NAME(src) << "\" is not raw audio";
        notify_message(*user_data, Gst_level::info, ss.str());
        gst_caps_unref(new_pad_caps);
        gst_object_unref(sink_pad);
        return;
    }

    GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
    if (GST_PAD_LINK_FAILED(ret)) {
        ss << "Failed to link new pad \"" << GST_PAD_NAME(new_pad) << "\" from \"" << GST_ELEMENT_NAME(src) << "\" with type \"" << new_pad_type << "\"";
        notify_message(*user_data, Gst_level::error, ss.str());
    } else {
        ss << "Successfully linked new pad \"" << GST_PAD_NAME(new_pad) << "\" from \"" << GST_ELEMENT_NAME(src) << "\" with type \"" << new_pad_type << "\"";
        notify_message(*user_data, Gst_level::info, ss.str());
    }
}
