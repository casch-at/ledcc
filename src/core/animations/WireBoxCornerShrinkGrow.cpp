#include "WireBoxCornerShrinkGrow.hpp"

WireBoxCornerShrinkGrow::WireBoxCornerShrinkGrow(const u_int16_t &iterations, const u_int16_t &speed, const u_int8_t &rotate, const u_int8_t &flip, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations),
    m_rotate(rotate),
    m_flip(flip)
{
}

void WireBoxCornerShrinkGrow::createAnimation()
{
    u_int8_t xyz = 0;
    for (u_int16_t j = 0; j < m_iterations; j++)
    {
        for (u_int8_t i = 0; i < CUBE_SIZE * 2; i++)
        {
            xyz = CUBE_SIZE - 1 - i;
            if (i > CUBE_SIZE - 1)
                xyz = i - CUBE_SIZE;
            fillCubeArray(0x00);
            boxWireframe(0, 0, 0, xyz, xyz, xyz);

            if (m_flip > 0)
                mirrorZ();
            if (m_rotate == 1 || m_rotate == 3)
                mirrorY();
            if (m_rotate == 2 || m_rotate == 3)
                mirrorX();
            waitMs(getSpeed());
    //			fillCubeArray(0x00);
        }
    }
}

