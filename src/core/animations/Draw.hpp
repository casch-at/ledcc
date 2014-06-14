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
#ifndef DRAW_HPP
#define DRAW_HPP
#include "Global.hpp"

// System includes
#include <math.h>

// Qt Includes
#include <QThread>
#include <QBitArray>
#include <QByteArray>
#include <QVector>
#include <QObject>

/*!
 \brief Base class of \a Animation class.
        Provides drawing functions.

*/
class Draw: public QObject
{
    Q_OBJECT
public:
    explicit Draw(QObject *parent = Q_NULLPTR);
    /** @brief CubeArray holds the data of the cube! */
    typedef QVector<QVector<quint8> > CubeArray;

    /** @brief: AXIS TYPE */
    typedef enum Axis
        {
        X_AXIS = 0,
        Y_AXIS,
        Z_AXIS
        } Axis;

    /** @brief: DIRECTION TYPE */
    typedef enum Direction
        {
        Backward = 0,
        Forward = !Backward
        } Direction;

    /** @brief: BIXEL STATE */
    typedef enum BixelState
        {
        OFF = 0,
        ON = !OFF
        } BixelState;

    CubeArray cubeFrame;
    CubeArray cubeFrameTemp;
protected:
    virtual ~Draw();
    void setBixel(const quint8 x, const quint8 y, const quint8 z);
    void setTempBixel(const quint8 x, const quint8 y, const quint8 z);

    void clearBixel(const quint8 x, const quint8 y, const quint8 z);
    void clearTempBixel(const quint8 x, const quint8 y, const quint8 z);

    BixelState getBixelState(const quint8 x, const quint8 y, const quint8 z);

    void flipBixels(const quint8 x, const quint8 y, const quint8 z);

    void alterBixel(const quint8 x, const quint8 y, const quint8 z, const BixelState state);

    bool inRange(const quint8 x, const quint8 y, const quint8 z);

    void shift(const Axis axis, const Direction direction);

    void checkArgumentOrder(quint8 from, quint8 to, quint8 *newStartPoint, quint8 *newEndPoint);

    void drawPositionAxis(const Axis axis, const QVector<quint8> &position, const Direction direction);

    void setPlaneX(const quint8 x);
    void setPlaneY(const quint8 y);
    void setPlaneZ(const quint8 z);
    void setPlane(const Axis axis, const quint8 i);

    void clearPlaneX(const quint8 x);
    void clearPlaneY(const quint8 y);
    void clearPlaneZ(const quint8 z);
    void clearPlane(const Axis axis, const quint8 i);

    void boxWireframe(quint8 x1, quint8 y1, quint8 z1, quint8 x2, quint8 y2, quint8 z2);
    void boxFilled(quint8 x1, quint8 y1, quint8 z1, quint8 x2, quint8 y2, quint8 z2);
    void boxWalls(quint8 x1, quint8 y1, quint8 z1, quint8 x2, quint8 y2, quint8 z2);

    void mirrorX(void);
    void mirrorY(void);
    void mirrorZ(void);

    void fillTempCubeArray(const quint8 &pattern);
    void fillCubeArray(const quint8 &pattern);

    quint8 byteline(const quint8 &start, const quint8 &end);
    quint8 flipByte(const quint8 byte);

    void tmpCubeToCube(void);

    void fontGetChar(quint8 chr, quint8 dst[]);

    float distance2d(float x1, float y1, float x2, float y2);
    // TODO:: Put 3D drawing functions into a Draw3D class some day or another
    float distance3d(float x1, float y1, float z1, float x2, float y2, float z2);

    void line3d(int x1, int y1, int z1, int x2, int y2, int z2);
    Q_DISABLE_COPY(Draw)
};
//Q_DECLARE_METATYPE(Draw::AnimationOptions)
#endif // DRAW_HPP
