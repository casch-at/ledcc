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
#include "Draw.hpp"
#include <string.h>
#include <QTime>
#include <QtGlobal>

Draw::Draw(QObject *parent):
    QObject(parent)
{
    QTime time = QTime::currentTime();
    cubeFrame =  CubeArray(8,QVector<u_int8_t>(8)) ;
    cubeFrameTemp =  CubeArray(8,QVector<u_int8_t>(8)) ;
    qsrand(static_cast<uint>(time.msecsSinceStartOfDay()));
    qRegisterMetaType<CubeArray>("CubeArray");
}

Draw::~Draw()
{
    cubeFrame.clear();
    cubeFrameTemp.clear();
}

void Draw::setBixel(int x, int y, int z)
{
    if (inRange(x, y, z))
        cubeFrame[z][y] |= ( 0x01 << x );
//    qDebug("|Z: 0x%02x| Y: 0x%02x| X: 0x%02x|",z,y,x);
}

void Draw::setTempBixel(u_int8_t x, u_int8_t y, u_int8_t z)
{
    if (inRange(x, y, z))
        cubeFrameTemp[z][y] |= ( 0x01 << x );
}

void Draw::clearBixel(u_int8_t x, u_int8_t y, u_int8_t z)
{
    if (inRange(x, y, z))
        cubeFrame[z][y] &= ~( 0x01 << x );
}

void Draw::clearTempBixel(u_int8_t x, u_int8_t y, u_int8_t z)
{
    if (inRange(x, y, z))
        cubeFrameTemp[z][y] &= ~( 0x01 << x );
}

Draw::BixelState Draw::getBixelState(u_int8_t x, u_int8_t y, u_int8_t z)
{
    if (inRange(x, y, z)) {
        if (cubeFrame[z][y] & ( 1 << x ))
            return ON;
        else
            return OFF;
    } else {
        return OFF;
    }
}

void Draw::flipBixels(u_int8_t x, u_int8_t y, u_int8_t z)
{
    if (inRange(x, y, z))
        cubeFrame[z][y] ^= ( 1 << x );
}

void Draw::alterBixel(u_int8_t x, u_int8_t y, u_int8_t z, BixelState state)
{
    if (state)
        setBixel(x, y, z);
    else
        clearBixel(x, y, z);
}

bool Draw::inRange(u_int8_t x, u_int8_t y, u_int8_t z)
{
    if (x < CUBE_SIZE && y < CUBE_SIZE && z < CUBE_SIZE)
        return true;
    return false;
}

void Draw::shift(Axis axis, Direction direction)
{
    u_int8_t i, x, y, ii, iii;
    for (i = 0; i < CUBE_SIZE; i++) {
        if (direction)
            ii = 7 - i;
        else
            ii = i;

        for (x = 0; x < CUBE_SIZE; x++) {
            for (y = 0; y < CUBE_SIZE; ++y) {
                if (direction)
                    iii = ii - 1;
                else
                    iii = ii + 1;

                switch (axis) {
                case X_AXIS:
                    alterBixel(ii, y, x, getBixelState(iii, y, x));
                    break;
                case Y_AXIS:
                    alterBixel(x, ii, y, getBixelState(x, iii, y));
                    break;
                case Z_AXIS:
                    alterBixel(x, y, ii, getBixelState(x, y, iii));
                    break;
                default:
                    break;
                }
            }
        }
    }
    if (direction) {
        i = 0;
    } else {
        i = 7;
    }

    for (x = 0; x < CUBE_SIZE; x++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            switch (axis) {
            case X_AXIS:
                clearBixel(i, y, x);
                break;
            case Y_AXIS:
                clearBixel(x, i, y);
                break;
            case Z_AXIS:
                clearBixel(x, y, i);
                break;
            default:
                break;
            }
        }
    }
}

void Draw::checkArgumentOrder(u_int8_t from, u_int8_t to, u_int8_t *newStartPoint, u_int8_t *newEndPoint)
{
    if (from > to) {
        u_int8_t tmp;
        tmp = from;
        from = to;
        to = tmp;
    }
    *newStartPoint = from;
    *newEndPoint = to;
}

void Draw::drawPositionAxis(Axis axis, QVector<u_int8_t> &position, Direction direction)
{
    u_int8_t k = 0;

    fillCubeArray(0x00);
    for (u_int8_t x = 0; x < CUBE_SIZE; x++)
    {
        for (u_int8_t y = 0; y < CUBE_SIZE; y++)
        {
            if (direction) /* Forward */
                k = position[x * CUBE_SIZE + y];
            else
                k = CUBE_SIZE - 1 - position[x * CUBE_SIZE + y];

            switch (axis)
            {
            case X_AXIS:
                setBixel(k, y, x);
                break;
            case Y_AXIS:
                setBixel(x, k, y);
                break;
            case Z_AXIS:
                setBixel(x, y, k);
                break;
            default:
                break;
            }
        }
    }
}

u_int8_t Draw::flipByte(u_int8_t byte)
{
    u_int8_t b = byte;
    b = ( ( ( b * 0x0802LU ) & 0x22110LU ) | ( ( b * 0x8020LU ) & 0x88440LU ) ) * 0x10101LU >> 16;
    return b;
}

void Draw::setPlaneZ(u_int8_t z)
{
    u_int8_t i;
    if (z < CUBE_SIZE) {
        for (i = 0; i < CUBE_SIZE; i++)
            cubeFrame[z][i] = 0xff;
    }
}

void Draw::clearPlaneZ(u_int8_t z)
{
    u_int8_t i;
    if (z < CUBE_SIZE) {
        for (i = 0; i < CUBE_SIZE; i++)
             cubeFrame[z][i]  = 0x00;
    }
}

void Draw::setPlaneX(u_int8_t x)
{
    u_int8_t z, y;
    if (x < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++) {
            for (y = 0; y < CUBE_SIZE; y++)
                cubeFrame[z][y] |= ( 1 << x );
        }
    }
}

void Draw::clearPlaneX(u_int8_t x)
{
    u_int8_t z, y;
    if (x < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++) {
            for (y = 0; y < CUBE_SIZE; y++)
                cubeFrame[z][y] &= ~( 1 << x );
        }
    }
}

void Draw::setPlaneY(u_int8_t y)
{
    u_int8_t z;
    if (y < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++)
            cubeFrame[z][y] = 0xFF;
    }
}

void Draw::clearPlaneY(u_int8_t y)
{
    u_int8_t z;
    if (y < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++)
            cubeFrame[z][y] = 0x00;
    }
}

void Draw::setPlane(Axis axis, u_int8_t i)
{
    switch (axis) {
    case X_AXIS:
        setPlaneX(i);
        break;
    case Y_AXIS:
        setPlaneY(i);
        break;
    case Z_AXIS:
        setPlaneZ(i);
        break;
    default:
        break;
    }
}

void Draw::clearPlane(Axis axis, u_int8_t i)
{
    switch (axis) {
    case X_AXIS:
        clearPlaneX(i);
        break;
    case Y_AXIS:
        clearPlaneY(i);
        break;
    case Z_AXIS:
        clearPlaneZ(i);
        break;
    default:
        break;
    }
}

void Draw::boxWireframe(u_int8_t x1, u_int8_t y1, u_int8_t z1, u_int8_t x2, u_int8_t y2, u_int8_t z2)
{
    u_int8_t iy = 0;
    u_int8_t iz = 0;

    checkArgumentOrder(x1, x2, &x1, &x2);
    checkArgumentOrder(y1, y2, &y1, &y2);
    checkArgumentOrder(z1, z2, &z1, &z2);

    // Lines along X axis
    cubeFrame[z1][y1] = byteline(x1, x2);
    cubeFrame[z1][y2] = byteline(x1, x2);
    cubeFrame[z2][y1] = byteline(x1, x2);
    cubeFrame[z2][y2] = byteline(x1, x2);

    // Lines along Y axis
    for (iy = y1; iy <= y2; iy++) {
        setBixel(x1, iy, z1);
        setBixel(x1, iy, z2);
        setBixel(x2, iy, z1);
        setBixel(x2, iy, z2);
    }

    // Lines along Z axis
    for (iz = z1; iz <= z2; iz++) {
        setBixel(x1, y1, iz);
        setBixel(x1, y2, iz);
        setBixel(x2, y1, iz);
        setBixel(x2, y2, iz);
    }
}

void Draw::boxFilled(u_int8_t x1, u_int8_t y1, u_int8_t z1, u_int8_t x2, u_int8_t y2, u_int8_t z2)
{
    u_int8_t iy;
    u_int8_t iz;

    checkArgumentOrder(x1, x2, &x1, &x2);
    checkArgumentOrder(y1, y2, &y1, &y2);
    checkArgumentOrder(z1, z2, &z1, &z2);

    for (iz = z1; iz <= z2; iz++) {
        for (iy = y1; iy <= y2; iy++) {
            cubeFrame[iz][iy] |= byteline(x1, x2);
        }
    }
}

void Draw::boxWalls(u_int8_t x1, u_int8_t y1, u_int8_t z1, u_int8_t x2, u_int8_t y2, u_int8_t z2)
{
    u_int8_t iy = 0;
    u_int8_t iz = 0;

    checkArgumentOrder(x1, x2, &x1, &x2);
    checkArgumentOrder(y1, y2, &y1, &y2);
    checkArgumentOrder(z1, z2, &z1, &z2);

    for (iz = z1; iz <= z2; iz++) {
        for (iy = y1; iy <= y2; iy++) {
            if (iy == y1 || iy == y2 || iz == z1 || iz == z2) {
                cubeFrame[iz][iy] = byteline(x1, x2);
            } else {
                cubeFrame[iz][iy] |= ( ( 0x01 << x1 ) | ( 0x01 << x2 ) );
            }
        }
    }
}

void Draw::mirrorX()
{
    u_int8_t y = 0;
    u_int8_t z = 0;
    for (z = 0; z < CUBE_SIZE; z++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            cubeFrameTemp[z][y] = flipByte(cubeFrame[z][y]);
        }
    }
    cubeFrame = cubeFrameTemp;
}

void Draw::mirrorY()
{
    u_int8_t z = 0;
    u_int8_t y = 0;
    for (z = 0; z < CUBE_SIZE; z++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            cubeFrameTemp[z][CUBE_SIZE - y - 1] = cubeFrame[z][y];
        }
    }
    cubeFrame = cubeFrameTemp;
}

void Draw::mirrorZ()
{
    u_int8_t y = 0;
    u_int8_t z = 0;
    for (z = 0; z < CUBE_SIZE; z++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            cubeFrameTemp[CUBE_SIZE - z - 1][y] = cubeFrame[z][y];
        }
    }
    cubeFrame = cubeFrameTemp;
}

void Draw::fillTempCubeArray(const u_int8_t &pattern)
{
    for(u_int8_t i = 0; i < CUBE_SIZE; i++)
        cubeFrameTemp[i].fill(pattern);
}

void Draw::fillCubeArray(const u_int8_t &pattern)
{
    for(u_int8_t i = 0; i < CUBE_SIZE; i++)
        cubeFrame[i].fill(pattern);
}

u_int8_t Draw::byteline(const u_int8_t &start, const u_int8_t &end)
{
    return ( ( 0xff << start ) & ~( 0xff << ( end + 1 ) ) );
}

void Draw::tmpCubeToCube()
{
    cubeFrame = cubeFrameTemp;
}

void Draw::fontGetChar(u_int8_t chr, u_int8_t dst[5])
{
    u_int8_t i=0;
    chr -= 32; // bitmap starts at ascii 32 (space)

    for (i = 0; i < 5; i++)
        dst[i] = lookUpTable[(chr*5)+i];
}


