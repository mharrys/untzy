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

#include "seeker_widget.h"
#include "ui_seeker_widget.h"

Seeker_widget::Seeker_widget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::seekerWidget),
      dragging(false)
{
    init();
}

Seeker_widget::~Seeker_widget()
{
    delete ui;
}

void Seeker_widget::set_position(long position)
{
    if (!dragging)
        ui->seekerSlider->setSliderPosition(position);
}

void Seeker_widget::set_length(long length)
{
    ui->seekerSlider->setRange(0, length);
}

void Seeker_widget::init()
{
    ui->setupUi(this);
    connect(ui->seekerSlider, &QSlider::sliderReleased, [=]() {
        dragging = false;
        emit changed_position(ui->seekerSlider->sliderPosition());
    });
    connect(ui->seekerSlider, &QSlider::sliderPressed, [=]() {
        dragging = true;
    });
}
