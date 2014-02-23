#include "WireBoxCenterShrinkGrow.hpp"

WireBoxCenterShrinkGrow::WireBoxCenterShrinkGrow(const u_int16_t &speed, const bool centerStart, const QString &name, QObject *parent) :
    Animation(speed,name,parent),
    m_centerStart(centerStart)
{
}

void WireBoxCenterShrinkGrow::createAnimation()
{
    u_int8_t j = 0;
    u_int8_t cubeSizeHalf = CUBE_SIZE / 2;
    u_int8_t xyzStart = 0;
    u_int8_t xyzEnd = 0;
    for (u_int8_t i = 0; i < cubeSizeHalf; i++)
    {
        fillCubeArray(0x00);
        j = i;
        if (m_centerStart)
            j = cubeSizeHalf -1 - i;
        xyzStart = cubeSizeHalf + j;
        xyzEnd = cubeSizeHalf - 1 - j;
        boxWireframe(xyzStart, xyzStart, xyzStart, xyzEnd, xyzEnd, xyzEnd);
        waitMs(getSpeed());
    }
}


