#include "Loadbar.hpp"

Loadbar::Loadbar(const u_int16_t &speed, const Draw::Axis &axis, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_axis(axis)
{
}

void Loadbar::createAnimation()
{
    fillCubeArray(0x00);
    for (u_int8_t i = 0; i < CUBE_SIZE; i++)
    {
        setPlane(m_axis, i);
        waitMs(getSpeed());
    }
    waitMs(getSpeed() * 2);
    for (u_int8_t i = 0; i < CUBE_SIZE; i++)
    {
        clearPlane(m_axis, i);
        waitMs(getSpeed());
    }
    Q_EMIT done();
}
