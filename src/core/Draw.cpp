#include "Draw.hpp"
#include <string.h>
#include <QTime>
#include <QtGlobal>
#include <QByteArray>
#include <QBitArray>
#include <QDebug>
Draw::Draw()
{
    QTime time = QTime::currentTime();
    cubeF =  CubeArray(8,QVector<uint8_t>(8)) ;
    cubeFTemp =  CubeArray(8,QVector<uint8_t>(8)) ;
    qsrand(static_cast<uint>(time.msecsSinceStartOfDay()));
}

Draw::~Draw()
{

}

void Draw::setBixel(int x, int y, int z)
{
    if (inRange(x, y, z))
        cubeF[z][y] |= ( 0x01 << x );
//    qDebug("|Z: 0x%02x| Y: 0x%02x| X: 0x%02x|",z,y,x);
}

void Draw::setTempBixel(uint8_t x, uint8_t y, uint8_t z)
{
    if (inRange(x, y, z))
        cubeFrameTemp[z][y] |= ( 0x01 << x );
}

void Draw::clearBixel(uint8_t x, uint8_t y, uint8_t z)
{
    if (inRange(x, y, z))
        cubeFrame[z][y] &= ~( 0x01 << x );
}

void Draw::clearTempBixel(uint8_t x, uint8_t y, uint8_t z)
{
    if (inRange(x, y, z))
        cubeFrameTemp[z][y] &= ~( 0x01 << x );
}

BixelState Draw::getBixelState(uint8_t x, uint8_t y, uint8_t z)
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

void Draw::flipBixels(uint8_t x, uint8_t y, uint8_t z)
{
    if (inRange(x, y, z))
        cubeFrame[z][y] ^= ( 1 << x );
}

void Draw::alterBixel(uint8_t x, uint8_t y, uint8_t z, BixelState state)
{
    if (state)
        setBixel(x, y, z);
    else
        clearBixel(x, y, z);
}

bool Draw::inRange(uint8_t x, uint8_t y, uint8_t z)
{
    if (x < CUBE_SIZE && y < CUBE_SIZE && z < CUBE_SIZE)
        return true;
    return false;
}

void Draw::shift(Axis axis, Direction direction)
{
    uint8_t i, x, y, ii, iii;
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

void Draw::checkArgumentOrder(uint8_t from, uint8_t to, uint8_t *newStartPoint, uint8_t *newEndPoint)
{
    if (from > to) {
        uint8_t tmp;
        tmp = from;
        from = to;
        to = tmp;
    }
    *newStartPoint = from;
    *newEndPoint = to;
}

void Draw::drawPositionAxis(Axis axis, uint8_t position[], bool invert)
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t k = 0;

    fillCubeArray(0x00);

    for (x = 0; x < CUBE_SIZE; x++)
    {
        for (y = 0; y < CUBE_SIZE; y++)
        {
            if (invert)
                k = CUBE_SIZE - 1 - position[x * CUBE_SIZE + y];
            else
                k = position[x * CUBE_SIZE + y];

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

uint8_t Draw::flipByte(uint8_t byte)
{
    uint8_t b = byte;
    b = ( ( ( b * 0x0802LU ) & 0x22110LU ) | ( ( b * 0x8020LU ) & 0x88440LU ) ) * 0x10101LU >> 16;
    return b;
}

void Draw::setPlaneZ(uint8_t z)
{
    uint8_t i;
    if (z < CUBE_SIZE) {
        for (i = 0; i < CUBE_SIZE; i++)
            cubeF[z][i] = 0xff;
    }
}

//void Draw::setPlaneZ(uint8_t z)
//{
//    uint8_t i;
//    if (z < CUBE_SIZE) {
//        for (i = 0; i < CUBE_SIZE; i++)
//            cubeFrame[z][i] = 0xFF;
//    }
//}

//void Draw::clearPlaneZ(uint8_t z)
//{
//    uint8_t i;
//    if (z < CUBE_SIZE) {
//        for (i = 0; i < CUBE_SIZE; i++)
//            cubeFrame[z][i] = 0x00;
//    }
//}
void Draw::clearPlaneZ(uint8_t z)
{
    uint8_t i;
    if (z < CUBE_SIZE) {
        for (i = 0; i < CUBE_SIZE; i++)
             cubeF[z][i]  = 0x00;
    }
}

void Draw::setPlaneX(uint8_t x)
{
    uint8_t z, y;
    if (x < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++) {
            for (y = 0; y < CUBE_SIZE; y++)
                cubeFrame[z][y] |= ( 1 << x );
        }
    }
}

//void Draw::clearPlaneX(uint8_t x)
//{
//    uint8_t z, y;
//    if (x < CUBE_SIZE) {
//        for (z = 0; z < CUBE_SIZE; z++) {
//            for (y = 0; y < CUBE_SIZE; y++)
//                cubeFrame[z][y] &= ~( 1 << x );
//        }
//    }
//}

void Draw::clearPlaneX(uint8_t x)
{
    uint8_t z, y;
    if (x < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++) {
            for (y = 0; y < CUBE_SIZE; y++)
                cubeFrame[z][y] &= ~( 1 << x );
        }
    }
}

void Draw::setPlaneY(uint8_t y)
{
    uint8_t z;
    if (y < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++)
            cubeFrame[z][y] = 0xFF;
    }
}

void Draw::clearPlaneY(uint8_t y)
{
    uint8_t z;
    if (y < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++)
            cubeFrame[z][y] = 0x00;
    }
}

void Draw::setPlane(Axis axis, uint8_t i)
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

void Draw::clearPlane(Axis axis, uint8_t i)
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

void Draw::boxWireframe(uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2)
{
    uint8_t iy = 0;
    uint8_t iz = 0;

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

void Draw::boxFilled(uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2)
{
    uint8_t iy;
    uint8_t iz;

    checkArgumentOrder(x1, x2, &x1, &x2);
    checkArgumentOrder(y1, y2, &y1, &y2);
    checkArgumentOrder(z1, z2, &z1, &z2);

    for (iz = z1; iz <= z2; iz++) {
        for (iy = y1; iy <= y2; iy++) {
            cubeFrame[iz][iy] |= byteline(x1, x2);
        }
    }
}

void Draw::boxWalls(uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2)
{
    uint8_t iy = 0;
    uint8_t iz = 0;

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
    uint8_t y = 0;
    uint8_t z = 0;
    for (z = 0; z < CUBE_SIZE; z++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            cubeFrameTemp[z][y] = flipByte(cubeFrame[z][y]);
        }
    }
    memcpy(&cubeFrame[0], &cubeFrameTemp[0], CUBE_ARRAY_SIZE);
}

void Draw::mirrorY()
{
    uint8_t z = 0;
    uint8_t y = 0;
    for (z = 0; z < CUBE_SIZE; z++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            cubeFrameTemp[z][CUBE_SIZE - y - 1] = cubeFrame[z][y];
        }
    }
    memcpy(&cubeFrame[0], &cubeFrameTemp[0], CUBE_ARRAY_SIZE);
}

void Draw::mirrorZ()
{
    uint8_t y = 0;
    uint8_t z = 0;
    for (z = 0; z < CUBE_SIZE; z++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            cubeFrameTemp[CUBE_SIZE - z - 1][y] = cubeFrame[z][y];
        }
    }
    memcpy(cubeFrame, cubeFrameTemp, CUBE_ARRAY_SIZE);
}

void Draw::fillTempCubeArray(uint8_t pattern)
{
    memset(cubeFrameTemp,pattern,CUBE_ARRAY_SIZE);
}

void Draw::fillCubeArray(uint8_t pattern)
{
    for (int z = 0; z < CUBE_SIZE; z++) {
        for (int y = 0; y < CUBE_SIZE; y++) {
            cubeF[z][y] = pattern;
        }
    }
}

uint8_t Draw::byteline(uint8_t start, uint8_t end)
{
    return ( ( 0xff << start ) & ~( 0xff << ( end + 1 ) ) );
}

void Draw::tmpCubeToCube()
{
    memcpy(&cubeFrame[0], &cubeFrameTemp[0], 64);
}

void Draw::fontGetChar(uint8_t chr, uint8_t dst[5])
{
    uint8_t i=0;
    chr -= 32; // bitmap starts at ascii 32 (space)

    for (i = 0; i < 5; i++)
        dst[i] = lookUpTable[(chr*5)+i];
}


