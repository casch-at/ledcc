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

Animation::Animation(const quint16 &speed, const QString &name, QObject *parent):
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
void Animation::sendBixelZ(quint8 x, quint8 y, quint8 z)
{
    quint8 ii = 0;
    for (quint8 i = 0; i < CUBE_SIZE; i++)
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
void Animation::effectZUpDownMove(QVector<quint8> &destination, QVector<quint8> &position, Draw::Axis axis)
{
    for (quint8 px = 0; px < CUBE_ARRAY_SIZE; px++)
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
void Animation::waitMs(const quint16 &time)
{
//    m_mutex.lock();
    cubeFrameTemp = cubeFrame;
//    m_mutex.unlock();
    Q_EMIT sendData(cubeFrameTemp);
//    thread()->usleep(time /** 1000*/);
    thread()->usleep(time * 1000);
}

