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

#include "volume_slider.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>

Volume_slider::Volume_slider(QWidget* parent)
    : QWidget(parent),
      left_down(false),
      volume(0.5)
{
    init();
}

void Volume_slider::wheelEvent(QWheelEvent* event)
{
    QPoint num_degrees = event->angleDelta() / 8;
    if (!num_degrees.isNull()) {
        auto direction = (num_degrees / 15).y();
        auto step = 0.05 * direction;
        new_level(volume.get_level() + step);
    }
}

void Volume_slider::mouseMoveEvent(QMouseEvent* event)
{
    if (left_down) {
        event->accept();
        auto pos = event->pos();
        auto lvl = pos.x() / double(width());
        new_level(lvl);
    }
}

void Volume_slider::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        event->accept();
        left_down = true;
    } else
        left_down = false;
}

void Volume_slider::paintEvent(QPaintEvent* paint_event)
{
    auto w = static_cast<double>(width());
    auto h = static_cast<double>(height());

    QPainterPath outline;
    outline.moveTo(0.0, h);
    outline.lineTo(w,   h);
    outline.lineTo(w,   0.0);
    outline.closeSubpath();

    auto s = volume.get_level();
    QPainterPath filling;
    filling.moveTo(0.0,   h);
    filling.lineTo(w * s, h);
    filling.lineTo(w * s, h * (1.0 - s));
    filling.closeSubpath();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw triangle filling of current volume percentage
    painter.save();
    painter.setBrush(QColor(20, 170, 255));
    painter.drawPath(filling);
    painter.restore();

    // Draw lines around the filling
    painter.save();
    painter.setOpacity(0.5);
    painter.drawPath(outline);
    painter.restore();

    // Draw text showing volume percentage
    QFont font;
    auto pixel_size = 12;
    font.setPixelSize(pixel_size);
    painter.setFont(font);
    auto percent = QString::number(static_cast<int>(s * 100.0));
    painter.drawText(0, pixel_size, percent + "%");
}

void Volume_slider::init()
{
    setMinimumWidth(80);
    setMaximumWidth(80);
    setMinimumHeight(25);
    setMaximumHeight(25);
    new_level(volume.get_level());
}

void Volume_slider::new_level(double lvl)
{
    lvl = std::max(lvl, volume.get_minimum());
    lvl = std::min(lvl, volume.get_maximum());
    volume = Volume(lvl);
    emit changed_volume(volume);
    repaint();
}
