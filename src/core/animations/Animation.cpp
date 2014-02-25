#include "Animation.hpp"
#include <QTimer>
#include <QDebug>

void Animation::sendBixelZ(u_int8_t x, u_int8_t y, u_int8_t z, u_int16_t speed)
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
        waitMs(speed);
    }
}

void Animation::effectZUpDownMove(QVector<u_int8_t> &destination, QVector<u_int8_t> &position, Axis axe)
{
    for (u_int8_t px = 0; px < CUBE_ARRAY_SIZE; px++)
    {
        if (position[px] < destination[px])
            position[px]++;
        else
            position[px]--;

    }
    drawPositionAxis(Z_AXIS, position, true);
}

void Animation::waitMs(const u_int16_t &time)
{
//    qDebug()<< "Animation thread id: " << thread()->currentThread();
//    qDebug()<< "Animation timer id: " << m_timer->thread()->currentThread();
//    m_timer->setSingleShot(true);
//    m_timer->start(time);
//    qDebug() << cubeFrame;
    Q_EMIT sendData();
//    for (int i = 0; i < 10000000; ++i);
    thread()->msleep(time);
}
