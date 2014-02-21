#include "AxisNailWall.hpp"

AxisNailWall::AxisNailWall(const u_int16_t &speed, const Draw::Axis &axis, const bool invert, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_axis(axis),
    m_invert(invert)
{
}

void AxisNailWall::createAnimation()
{
    QVector<u_int8_t> destination(CUBE_ARRAY_SIZE);
    QVector<u_int8_t> position(CUBE_ARRAY_SIZE);


    fillCubeArray(0x00);

    if (m_invert)
        setPlane(m_axis, 7);
    else
        setPlane(m_axis, 0);

    waitMs(getSpeed() * 3);

    for (u_int8_t i = 0; i < CUBE_ARRAY_SIZE; i++)
        destination[i] = rand() % CUBE_SIZE;

    for (u_int8_t i = 0; i < CUBE_SIZE; i++)
    {
        for (u_int8_t px = 0; px < CUBE_ARRAY_SIZE; px++)
        {
            if (position[px] < destination[px])
                position[px]++;
        }
        drawPositionAxis(m_axis, position, m_invert);
        waitMs(getSpeed());
    }

    for (u_int8_t i = 0; i < CUBE_ARRAY_SIZE; i++)
    {
        destination[i] = CUBE_SIZE - 1;
    }

    waitMs(getSpeed() * 10);

    for (u_int8_t i = 0; i < CUBE_SIZE; i++)
    {
        for (u_int8_t px = 0; px < CUBE_ARRAY_SIZE; px++)
        {
            if (position[px] < destination[px])
                position[px]++;
            else if (position[px] > destination[px])
                position[px]--;
        }
        drawPositionAxis(m_axis, position, m_invert);
        waitMs(getSpeed());
    }

}
