#include "Wall.hpp"
#include <QDebug>
Wall::Wall(const u_int16_t &speed, const Axis &axis, const Direction &direction, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_direction(direction),
    m_axis(axis)
{
}

void Wall::createAnimation()
{
    u_int8_t i = 0;

    fillCubeArray(0x00);

    if (m_direction)
        setPlane(m_axis, 0);
    else
        setPlane(m_axis, 7);

    for (i = 0; i < CUBE_SIZE - 1; i++)
    {
        waitMs(getSpeed());
        shift(m_axis, m_direction);

        qDebug()<< "Wall thread it: " << thread()->currentThread();

    }
}
