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

#include "ConwaysGameOfLife.hpp"

ConwaysGameOfLife::ConwaysGameOfLife(const quint16 &speed, const QString &name, const quint16 &iterations, const bool invert, QObject *parent) :
    Animation(speed,name,parent),
    m_iterations(iterations),
    m_invert(invert)
{

}

void ConwaysGameOfLife::createAnimation()
{
    fillCubeArray(0x00);

    for (quint8 i = 0; i < 20; i++) {
        setBixel(rand() % 4, rand() % 4, rand() % 4);
    }

    for (quint16 i  = 0; i < m_iterations; i++) {
        nextgen();

        if (countChanges() == 0) {
            break;
        }

        tmpCubeToCube();

        waitMs(speed());

    }
    Q_EMIT done();
}

quint16 ConwaysGameOfLife::countChanges()
{
    quint16 counter = 0;

    for (quint8 x = 0; x < CUBE_SIZE; x++) {
        for (quint8 y = 0; y < CUBE_SIZE; y++) {
            if (cubeFrameTemp[x][y] != cubeFrame[x][y]) {
                counter++;
            }
        }
    }
    return counter;
}

quint8 ConwaysGameOfLife::countNeighbors(quint8 x, quint8 y, quint8 z)
{
    qint8 jx = 0;
    qint8 jy = 0;
    qint8 jz = 0;
    quint8 neighbore = 0;

    for (qint8 ix = -1; ix < 2; ix++) {
        for (qint8 iy = -1; iy < 2; iy++) {
            for (qint8 iz = -1; iz < 2; iz++) {
                if ( !(ix == 0 && iy == 0 && iz == 0) ) {
                    if (m_invert) {
                        jx = (x + ix) % CUBE_SIZE;
                        jy = (y + iy) % CUBE_SIZE;
                        jz = (z + iz) % CUBE_SIZE;
                    } else {
                        jx = x + ix;
                        jy = y + iy;
                        jz = z + iz;
                    }

                    if (getBixelState(jx, jy, jz)) {
                        neighbore++;
                    }
                }
            }
        }
    }
    return neighbore;
}

void ConwaysGameOfLife::nextgen()
{
    quint8 neighbors = 0;

    fillTempCubeArray(0x00);

    for (quint8 x = 0; x < CUBE_SIZE; x++) {
        for (quint8 y = 0; y < CUBE_SIZE; y++) {
            for (quint8 z = 0; z < CUBE_SIZE; z++) {

                neighbors = countNeighbors(x, y, z);

                if (getBixelState(x, y, z) == Draw::ON) {
                    if (neighbors <= TERMINATE_LONELY || neighbors >= TERMINATE_CROWDED) {
                        clearTempBixel(x, y, z);
                    } else {
                        setTempBixel(x, y, z);
                    }
                } else {
                    if (neighbors >= CREATE_MIN && neighbors <= CREATE_MAX) {
                        setTempBixel(x, y, z);
                    }
                }
            }
        }
    }
}
