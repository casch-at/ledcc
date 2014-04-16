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





namespace animations {

     /*!
 \brief Base class of \a Animation class which provides drawing functions

*/
     class Draw: public QObject
     {
         Q_OBJECT
     public:
         explicit Draw(QObject *parent = Q_NULLPTR);
         /** @brief CubeArray holds the data of the cube! */
         typedef QVector<QVector<u_int8_t> > CubeArray; /*! Define alias for the CubeArray */

         /************************************************************************
     *                   ENUMERATION DEFENTIONS
     ************************************************************************/
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
         void setBixel(int x, int y, int z);
         void setTempBixel(u_int8_t x, u_int8_t y, u_int8_t z);

         void clearBixel(u_int8_t x, u_int8_t y, u_int8_t z);
         void clearTempBixel(u_int8_t x, u_int8_t y, u_int8_t z);

         BixelState getBixelState(u_int8_t x, u_int8_t y, u_int8_t z);

         void flipBixels(u_int8_t x, u_int8_t y, u_int8_t z);

         void alterBixel(u_int8_t x, u_int8_t y, u_int8_t z, BixelState state);

         bool inRange(u_int8_t x, u_int8_t y, u_int8_t z);

         void shift(Axis axis, Direction direction);

         void checkArgumentOrder(u_int8_t from, u_int8_t to, u_int8_t *newStartPoint, u_int8_t *newEndPoint);

         void drawPositionAxis(Axis axis, QVector<u_int8_t> &position, Direction direction);

         void setPlaneX(u_int8_t x);
         void setPlaneY(u_int8_t y);
         void setPlaneZ(u_int8_t z);
         void setPlane(Axis axis, u_int8_t i);

         void clearPlaneX(u_int8_t x);
         void clearPlaneY(u_int8_t y);
         void clearPlaneZ(u_int8_t z);
         void clearPlane(Axis axis, u_int8_t i);

         void boxWireframe(u_int8_t x1, u_int8_t y1, u_int8_t z1, u_int8_t x2, u_int8_t y2, u_int8_t z2);
         void boxFilled(u_int8_t x1, u_int8_t y1, u_int8_t z1, u_int8_t x2, u_int8_t y2, u_int8_t z2);
         void boxWalls(u_int8_t x1, u_int8_t y1, u_int8_t z1, u_int8_t x2, u_int8_t y2, u_int8_t z2);

         void mirrorX(void);
         void mirrorY(void);
         void mirrorZ(void);

         void fillTempCubeArray(const u_int8_t &pattern);
         void fillCubeArray(const u_int8_t &pattern);

         u_int8_t byteline(const u_int8_t &start, const u_int8_t &end);
         u_int8_t flipByte(u_int8_t byte);

         void tmpCubeToCube(void);

         void fontGetChar(u_int8_t chr, u_int8_t dst[5]);
         Q_DISABLE_COPY(Draw)
     };
     } // End namespace animations
 //Q_DECLARE_METATYPE(Draw::AnimationOptions)
#endif // DRAW_HPP
