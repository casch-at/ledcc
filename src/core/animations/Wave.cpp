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

#include "Wave.hpp"

Wave::Wave(const quint16 &speed, const quint16 &iterations, const QString &name, QObject *parent) :
    Animation(speed, name, parent),
    m_iterations(iterations)
{
}

void Wave::createAnimation()
{
    float /*origin_x, origin_y,*/ distance, height, ripple_interval = 1.3;

    fillCubeArray(0x00);
//    QVector<QVector<QVector<quint8> > > tripleVector(iterations(),QVector<QVector<quint8> >(8, QVector<quint8>(8)));

    for (quint16 i=0; i<iterations(); i++)
    {
        if(m_abort)
            return;
        for (quint8 x=0; x<8; x++)
        {
            for (quint8 y=0; y<8; y++)
            {
                distance = distance2d(3.5,3.5,x,y)/9.899495*8;
                height = 4+sin(distance/ripple_interval+static_cast<float>(i)/50)*4;
                setBixel(x,y,static_cast<quint8>(height));
            }
        }
//        tripleVector[i] = cubeFrame;
        fillCubeArray(0x00);
    }
//    for (int i = 0; i < iterations(); i++) {
//        sendData(tripleVector[i]);
////        if(i/10 == 0)
////            waitMs(speed());
//    }
    waitMs(200);
    Q_EMIT done();
}

//void Wave::createAnimation()
//{
//    int i,x;

//    float left, right, sine_base, x_dividor,ripple_height;

//    for (i=0; i<iterations(); i++)
//    {
//        for (x=0; x<8 ;x++)
//        {
//            x_dividor = 2 + sin(static_cast<float>(i)/100)+1;
//            ripple_height = 3 + (sin(static_cast<float>(i)/200)+1)*6;

//            sine_base = static_cast<float>(i)/40 + static_cast<float>(x)/x_dividor;

//            left = 4 + sin(sine_base)*ripple_height;
//            right = 4 + cos(sine_base)*ripple_height;
//            right = 7-left;

//            //printf("%i %i \n", (int) left, (int) right);

//            line3d(0-3, x, static_cast<int>(left), 7+3, x, static_cast<int>(right));
//            //line_3d((int) right, 7, x);
//        }

//        waitMs(speed());
//        fillCubeArray(0x00);
//    }
//    Q_EMIT done();
//}
