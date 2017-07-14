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

#ifndef SEEKER_WIDGET_H_INCLUDED
#define SEEKER_WIDGET_H_INCLUDED

#include <QWidget>

namespace Ui {
class seekerWidget;
}

class Seeker_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Seeker_widget(QWidget* parent = 0);
    ~Seeker_widget();
public slots:
    // Set position in seconds unless user is currently dragging in seeker.
    void set_position(long position);
    // Set length in seconds.
    void set_length(long length);
signals:
    // Seek position moved by user. Provided is the seek position in seconds.
    void changed_position(long seek_pos);
private:
    void init();

    Ui::seekerWidget* ui;
    bool dragging;
};

#endif // SEEKER_WIDGET_H
