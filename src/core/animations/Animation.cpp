/**
 * Copyright (C) 2014  Christian Schwarzgruber <christiandev9@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Animation.hpp"
#include <QTimer>

using namespace animations;
Animation::Animation(const u_int16_t &speed, const QString &name, QObject *parent):
    Draw(parent),
    m_speed(speed),
    m_name(name)
{
    m_abort = false;
}

/**
 * @brief The sendBixelZ function sends a led either up or down at position
 *        @param x y.
 *
 * @param x
 * @param y
 * @param z
 */
void Animation::sendBixelZ(u_int8_t x, u_int8_t y, u_int8_t z)
{
    u_int8_t ii = 0;
    for (u_int8_t i = 0; i < CUBE_SIZE; i++)
    {
        if (z == ( CUBE_SIZE - 1))
        {
            ii = ( CUBE_SIZE - 1) - i;
            clearBixel(x, y, ii + 1);
        }
        else
        {
            ii = i;
            clearBixel(x, y, ii - 1);
        }
        setBixel(x, y, ii);
        waitMs(m_speed);
    }
}


/**
 * @brief If the number at QVector position "x" is greater or lower
 *        than the number in the destination QVector, the new position
 *        gets either decrimented or incremented by one.
 *
 * @param destination
 * @param position
 * @param axe
 */
void Animation::effectZUpDownMove(QVector<u_int8_t> &destination, QVector<u_int8_t> &position, Draw::Axis axis)
{
    for (u_int8_t px = 0; px < CUBE_ARRAY_SIZE; px++)
    {
        if (position[px] < destination[px])
            position[px]++;
        else if(position[px] > destination[px])
            position[px]--;

    }
    drawPositionAxis(axis, position, Draw::Backward);
}


/**
 * @brief Function simulates the animation speed. This thread where the animation
 *        gets actually created are put to sleep for @param milliseconds.
 *
 * @param time
 */
void Animation::waitMs(const u_int16_t &time)
{
    Q_EMIT sendData(cubeFrame);
    thread()->usleep(time *1000);
}


void Animation::setItemToolTipNameSpeed(QString *itemToolTip, AnimationItem *item)
{
    itemToolTip->append(QString(tr("<p style='white-space:pre'><font color=#00FFFF><b>%1 Animation</b></font><br>"
                                   "Speed: %2<br>"))
                        .arg(item->text())
                        .arg(item->getOptions()->m_speed));
}
