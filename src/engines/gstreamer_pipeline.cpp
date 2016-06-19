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

#include "core/logger.h"
#include "core/volume.h"

namespace {

    GStreamer_pipeline::State translate_state(GstState state)
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

    GstBusSyncReply bus_sync_handler(GstBus* bus, GstMessage* message, gpointer user_data_ptr)
    {
        auto user_data = reinterpret_cast<GStreamer_pipeline::User_data*>(user_data_ptr);
        GError* error;
        gchar* dbg_info;
        GstState old_state, new_state;
        switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_INFO:
            gst_message_parse_info(message, &error, &dbg_info);
            user_data->logger->info(
                Logger::Tag::engine,
                QObject::tr("Info from element %1: %2.").arg(GST_OBJECT_NAME(message->src), error->message));
            user_data->logger->info(
                Logger::Tag::engine,
                QObject::tr("Debugging info: %1.").arg(dbg_info ? dbg_info : QObject::tr("None")));
            g_error_free(error);
            g_free(dbg_info);
            break;
        case GST_MESSAGE_WARNING:
            gst_message_parse_warning(message, &error, &dbg_info);
            user_data->logger->warn(
                Logger::Tag::engine,
                QObject::tr("Warning from element %1: %2.").arg(GST_OBJECT_NAME(message->src), error->message));
            user_data->logger->info(
                Logger::Tag::engine,
                QObject::tr("Debugging info: %1.").arg(dbg_info ? dbg_info : QObject::tr("None")));
            g_error_free(error);
            g_free(dbg_info);
            break;
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(message, &error, &dbg_info);
            user_data->logger->crit(
                Logger::Tag::engine,
                QObject::tr("Error from element %1: %2.").arg(GST_OBJECT_NAME(message->src), error->message));
            user_data->logger->info(
                Logger::Tag::engine,
                QObject::tr("Debugging info: %1.").arg(dbg_info ? dbg_info : QObject::tr("None")));
            g_error_free(error);
            g_free(dbg_info);
            break;
        case GST_MESSAGE_EOS:
            user_data->logger->info(Logger::Tag::engine, QObject::tr("End of stream."));
            emit user_data->pipeline->end_of_stream();
            break;
        case GST_MESSAGE_STATE_CHANGED:
            gst_message_parse_state_changed(message, &old_state, &new_state, NULL);
            user_data->logger->info(
                Logger::Tag::engine,
                QObject::tr("Element %1 changed state from %2 to %3.").arg(
                    GST_OBJECT_NAME(message->src),
                    gst_element_state_get_name(old_state),
                    gst_element_state_get_name(new_state)));
            emit user_data->pipeline->state_changed(
                    translate_state(old_state),
                    translate_state(new_state));
            break;
        }
        return GST_BUS_PASS;
    }

    void pad_added_handler(GstElement* src, GstPad* new_pad, GStreamer_pipeline::User_data* user_data)
    {
        GstPad* sink_pad = gst_element_get_static_pad(user_data->convert, "sink");

        if (gst_pad_is_linked(sink_pad)) {
            gst_object_unref(sink_pad);
            user_data->logger->info(
                Logger::Tag::engine,
                QObject::tr("Attempted to link new pad \"%1\" from \"%2\", but already linked.").arg(
                    GST_PAD_NAME(new_pad),
                    GST_ELEMENT_NAME(src)));
            return;
        }

        GstCaps* new_pad_caps = gst_pad_get_current_caps(new_pad);
        GstStructure* new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
        const gchar* new_pad_type = gst_structure_get_name(new_pad_struct);
        if (!g_str_has_prefix(new_pad_type, "audio/x-raw")) {
            user_data->logger->info(
                Logger::Tag::engine,
                QObject::tr("New pad \"%1\" from \"%2\" is not raw audio.").arg(
                    GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src)));
            gst_caps_unref(new_pad_caps);
            gst_object_unref(sink_pad);
            return;
        }

        GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
        if (GST_PAD_LINK_FAILED(ret)) {
            user_data->logger->crit(
                Logger::Tag::engine,
                QObject::tr("Failed to link new pad \"%1\" from \"%2\" with type \"%3\".").arg(
                    GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src), new_pad_type));
        } else {
            user_data->logger->info(
                Logger::Tag::engine,
                QObject::tr("Successfully linked new pad \"%1\" from \"%2\" with type \"%3\".").arg(
                    GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src), new_pad_type));
        }
    }

}

std::unique_ptr<GStreamer_pipeline> GStreamer_pipeline::make(std::shared_ptr<Logger> logger)
{
    if (!gst_is_initialized()) {
        logger->crit(Logger::Tag::engine, tr("Unable to create pipeline, GStreamer is not initialized."));
        return nullptr;
    }

    const auto elem_crit = tr("Unable to create GStreamer element \"%1\".");

    Gst_data data;
    data.source = gst_element_factory_make("uridecodebin", "source");
    if (!data.source) {
        logger->crit(Logger::Tag::engine, elem_crit.arg("uridecodebin"));
        return nullptr;
    }
    data.convert = gst_element_factory_make("audioconvert", "convert");
    if (!data.convert) {
        logger->crit(Logger::Tag::engine, elem_crit.arg("audioconvert"));
        return nullptr;
    }
    data.volume = gst_element_factory_make("volume", "volume");
    if (!data.volume) {
        logger->crit(Logger::Tag::engine, elem_crit.arg("volume"));
        return nullptr;
    }
    data.level = gst_element_factory_make("level", "level");
    if (!data.level) {
        logger->crit(Logger::Tag::engine, elem_crit.arg("level"));
        return nullptr;
    }
    data.sink = gst_element_factory_make("autoaudiosink", "sink");
    if (!data.sink) {
        logger->crit(Logger::Tag::engine, elem_crit.arg("autoaudiosink"));
        return nullptr;
    }
    data.pipeline = gst_pipeline_new("pipeline");
    if (!data.pipeline) {
        logger->crit(Logger::Tag::engine, tr("Unable to create new GStreamer pipeline."));
        return nullptr;
    }
    gst_bin_add_many(
        GST_BIN(data.pipeline),
        data.source,
        data.convert,
        data.volume,
        data.level,
        data.sink,
        NULL);

    if (!gst_element_link_many(data.convert, data.volume, data.level, data.sink, NULL)) {
        logger->crit(Logger::Tag::engine, tr("Unable to link elements in GStreamer pipeline."));
        gst_object_unref(data.pipeline);
        return nullptr;
    }

    data.bus = gst_element_get_bus(data.pipeline);
    data.state = GST_STATE_NULL;
    return std::make_unique<GStreamer_pipeline>(data, logger);
}

GStreamer_pipeline::GStreamer_pipeline(const Gst_data& data, std::shared_ptr<Logger> logger)
    : data(data),
      logger(logger)

{
    user_data = {this, logger, data.convert};
    g_signal_connect(this->data.source, "pad-added", G_CALLBACK(pad_added_handler), &user_data);
    gst_bus_set_sync_handler(this->data.bus, bus_sync_handler, &user_data, NULL);
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
        logger->crit(Logger::Tag::engine, tr("Unable to set GStreamer pipeline state."));
    else
        data.state = new_state;
}

void GStreamer_pipeline::set_volume(const Volume& volume)
{
    g_object_set(data.volume, "volume", volume.get_level(), NULL);
}
