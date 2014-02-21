#include "Animations.hpp"
#include <QtGlobal>
#include <QTimer>
#include <QDebug>

Animations::Animations()
{

}

Animations::~Animations()
{

}


void Animations::effectWall(Axis axis, Direction direction, uint16_t speed)
{
    uint8_t i = 0;

    fillCubeArray(0x00);

    if (direction)
        setPlane(axis, 0);
    else
        setPlane(axis, 7);

    for (i = 0; i < CUBE_SIZE - 1; i++)
    {
        animationWait(speed);
        shift(axis, direction);
    }
}

void Animations::effectTest()
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t i = 0;
    uint8_t z = 0;

    for (i = 0; i < 100; i++)
    {
        for (x = 0; x < CUBE_SIZE; x++)
        {
            for (y = 0; y < CUBE_SIZE; y++)
            {

                if (z == 7)
                {
                    z = 0;
                }

            }
        }
        x = sin(i / 8) * 2 + 3.5;
        y = cos(i / 8) * 2 + 3.5;
        animationWait(100);
        setBixel(x, y, z + 1);
    }
    fillCubeArray(0x00);
}

void Animations::effectRain(uint16_t iterations, uint16_t speed)
{
    uint8_t i = 0;
    uint16_t ii = 0;

    for (ii = 0; ii < iterations; ii++)
    {
        for (i = 0; i < qrand() % (CUBE_SIZE / 2); i++)
        {
            setBixel(qrand() % CUBE_SIZE, qrand() % CUBE_SIZE, 7);
        }

        animationWait(speed);
        shift(Z_AXIS, BACKWARD);
    }
}

//void Animations::effectLift(uint16_t iterations, uint16_t speed, uint16_t delay)
//{
//    uint8_t x = 0;
//    uint8_t y = 0;
//    uint16_t i = 0;
//    uint8_t lastX = 0;
//    uint8_t lastY = 0;

//    for (x = 0; x < CUBE_SIZE; x++)
//    {
//        for (y = 0; y < CUBE_SIZE; y++)
//        {
//            setBixel(x, y, (qrand() % 2) * 7);
//        }
//    }

//    for (i = 0; i < iterations; i++)
//    {
//        x = qrand() % CUBE_SIZE;
//        y = qrand() % CUBE_SIZE;

//        if (y != lastY && x != lastX)
//        {
//            if (getBixelState(x, y, 0))
//                sendBixelZ(x, y, 0, speed);
//            else
//                sendBixelZ(x, y, 7, speed);
//            animationWait(delay);
//            lastX = x;
//            lastY = y;
//        }
//    }
//}

//void Animations::effectRandomZLift(uint16_t iterations, uint16_t speed)
//{
//    uint8_t j = 0;
//    uint16_t i = 0;

//    uint8_t destination[CUBE_ARRAY_SIZE] =
//    { 0 };
//    uint8_t position[CUBE_ARRAY_SIZE] =
//    { 0 };
//    for (i = 0; i < CUBE_ARRAY_SIZE; i++)
//    {
//        position[i] = 4;
//        destination[i] = qrand() % 8;
//    }

//    for (i = 0; i < iterations; i++)
//    {
//        for (j = 0; j < CUBE_SIZE; j++)
//        {
//            effectZUpDownMove(destination, position, Z_AXIS);
//            animationWait(speed);
//        }

//        animationWait(speed * 4);

//        for (j = 0; j < CUBE_ARRAY_SIZE / 2; j++)
//        {
//            destination[qrand() % CUBE_ARRAY_SIZE] = qrand() % CUBE_SIZE;
//        }
//    }
//}

//void Animations::effectZUpDownMove(uint8_t destination[], uint8_t position[], Axis axe)
//{
//    uint8_t px = 0;
//    for (px = 0; px < CUBE_ARRAY_SIZE; px++)
//    {
//        if (position[px] < destination[px])
//        {
//            position[px]++;
//        }
//        if (position[px] > destination[px])
//        {
//            position[px]--;
//        }

//    }
//    drawPositionAxis(Z_AXIS, position, true);
//}

void Animations::effectFirework(uint16_t iterations, uint16_t speed, uint8_t particles)
{
    uint16_t i = 0;
    uint8_t f = 0;
    uint8_t e = 0;

    float origin_x = 3;
    float origin_y = 3;
    float origin_z = 3;

    uint8_t rand_y = 0;
    uint8_t rand_x = 0;
    uint8_t rand_z = 0;

    float slowrate = 0;
    float gravity = 0;

    // Particles and their position, x,y,z and their movement, dx, dy, dz
    float particlesA[particles][6];

    for (i = 0; i < iterations; i++)
    {

        origin_x = qrand() % 4 + 2;
        origin_y = qrand() % 4 + 2;
        origin_z = qrand() % 2 + 5;

        // shoot a particle up in the air
        for (e = 0; e < origin_z; e++)
        {
            setBixel(origin_x, origin_y, e);
            animationWait(speed * 2);
            fillCubeArray(0x00);
        }
        // Fill particle array
        for (f = 0; f < particles; f++)
        {
            // Position
            particlesA[f][0] = origin_x;
            particlesA[f][1] = origin_y;
            particlesA[f][2] = origin_z;

            rand_x = qrand() % 200;
            rand_y = qrand() % 200;
            rand_z = qrand() % 200;

            // Movement
            particlesA[f][3] = 1 - static_cast<float>(rand_x / 100); // dx
            particlesA[f][4] = 1 - static_cast<float>(rand_y / 100); // dy
            particlesA[f][5] = 1 - static_cast<float>(rand_z / 100); // dz
        }
        // explode
        for (e = 0; e < 25; e++)
        {
            slowrate = 1 + tan((e + 0.1) / 20) * 10;
            gravity = tan((e + 0.1) / 20) / 2;

            for (f = 0; f < particles; f++)
            {
                particlesA[f][0] += particlesA[f][3] / slowrate;
                particlesA[f][1] += particlesA[f][4] / slowrate;
                particlesA[f][2] += particlesA[f][5] / slowrate;
                particlesA[f][2] -= gravity;

                setBixel(particlesA[f][0], particlesA[f][1], particlesA[f][2]);
            }
            animationWait(speed);
            fillCubeArray(0x00);
        }
    }
}

//void Animations::sendBixelZ(uint8_t x, uint8_t y, uint8_t z, uint16_t speed)
//{
//    uint8_t i = 0;
//    uint8_t ii = 0;
//    for (i = 0; i < CUBE_SIZE; i++)
//    {
//        if (z == ( CUBE_SIZE - 1))
//        {
//            ii = ( CUBE_SIZE - 1) - i;
//            clearBixel(x, y, ii + 1);
//        }
//        else
//        {
//            ii = i;
//            clearBixel(x, y, ii - 1);
//        }
//        setBixel(x, y, ii);
//        animationWait(speed);
//    }
//}

void Animations::effectWireBoxCornerShrinkGrow(uint16_t iterations, uint16_t speed, uint8_t rotate, uint8_t flip)
{
    uint16_t j = 0;
    uint8_t i = 0;
    uint8_t xyz = 0;
    for (j = 0; j < iterations; j++)
    {
        for (i = 0; i < CUBE_SIZE * 2; i++)
        {
            xyz = CUBE_SIZE - 1 - i;
            if (i > CUBE_SIZE - 1)
                xyz = i - CUBE_SIZE;
            fillCubeArray(0x00);

            animationWait(1);

            boxWireframe(0, 0, 0, xyz, xyz, xyz);

            if (flip > 0)
                mirrorZ();
            if (rotate == 1 || rotate == 3)
                mirrorY();
            if (rotate == 2 || rotate == 3)
                mirrorX();
            animationWait(speed);
        }
    }

}

void Animations::effectWireBoxCenterShrinkGrow(uint16_t speed, bool centerStart)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t cubeSizeHalf = CUBE_SIZE / 2;
    uint8_t xyzStart = 0;
    uint8_t xyzEnd = 0;
    for (i = 0; i < cubeSizeHalf; i++)
    {
        fillCubeArray(0x00);
        j = i;
        if (centerStart)
            j = cubeSizeHalf -1 - i;
        xyzStart = cubeSizeHalf + j;
        xyzEnd = cubeSizeHalf - 1 - j;
        boxWireframe(xyzStart, xyzStart, xyzStart, xyzEnd, xyzEnd, xyzEnd);
        animationWait(speed);
    }
}

void Animations::effectAxisNailWall(uint16_t speed, Axis axis, bool invert)
{
    QVector<u_int8_t> destination(CUBE_ARRAY_SIZE);
    QVector<u_int8_t> position(CUBE_ARRAY_SIZE);

    uint8_t i = 0;
    uint8_t px = 0;

    fillCubeArray(0x00);

    if (invert)
        setPlane(axis, 7);
    else
        setPlane(axis, 0);

    animationWait(speed*3);

    for (i = 0; i < CUBE_ARRAY_SIZE; i++)
        destination[i] = qrand() % CUBE_SIZE;

    for (i = 0; i < CUBE_SIZE; i++)
    {
        for (px = 0; px < CUBE_ARRAY_SIZE; px++)
        {
            if (position[px] < destination[px])
                position[px]++;
        }
        drawPositionAxis(axis, position, invert);
        animationWait(speed);
    }

    for (i = 0; i < CUBE_ARRAY_SIZE; i++)
    {
        destination[i] = CUBE_SIZE - 1;
    }

    animationWait(speed * 10);

    for (i = 0; i < CUBE_SIZE; i++)
    {
        for (px = 0; px < CUBE_ARRAY_SIZE; px++)
        {
            if (position[px] < destination[px])
                position[px]++;
            else if (position[px] > destination[px])
                position[px]--;
        }
        drawPositionAxis(axis, position, invert);
        animationWait(speed);
    }
}

void Animations::effectLoadbar(uint16_t speed, Axis axis)
{
    uint8_t i = 0;
    fillCubeArray(0x00);
    for (i = 0; i < CUBE_SIZE; i++)
    {
        setPlane(axis, i);
        animationWait(speed);
    }
    animationWait(speed * 2 );
    for (i = 0; i < CUBE_SIZE; i++)
    {
        clearPlane(axis, i);
        animationWait(speed);
    }
}

void Animations::effectRandomSparkFlash(uint16_t iterations, uint16_t speed, uint16_t bixels)
{
    uint16_t b = 0;
    uint32_t i = 0;
    for (i = 0; i < iterations; i++)
    {
        for (b = 0; b < bixels; b++)
        {
            setBixel(qrand() % CUBE_SIZE, qrand() % CUBE_SIZE,
                     qrand() % CUBE_SIZE);
        }
        animationWait(speed);
        fillCubeArray(0x00);
    }
}

void Animations::effectRandomSpark(uint16_t sparks, uint16_t speed)
{
    uint16_t i = 0;

    for (i = 0; i < sparks; i++)
    {
        effectRandomSparkFlash(5, speed, i);
    }
    for (i = sparks; i >= 1; i--)
    {
        effectRandomSparkFlash(5, speed, i);
    }
}

void Animations::effectRandomFiller(uint16_t speed, BixelState state)
{
    uint32_t leds = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t z = 0;

    if (state)
        fillCubeArray(0x00);
    else
        fillCubeArray(0xFF);

    while ( leds <  CUBE_LED_COUNT - 1) {

    }
    while (leds < CUBE_LED_COUNT - 1)
    {
        x = qrand() % CUBE_SIZE;
        y = qrand() % CUBE_SIZE;
        z = qrand() % CUBE_SIZE;
        if ((state == OFF && getBixelState(x, y, z) == ON)
                || (state == ON && getBixelState(x, y, z) == OFF))
        {
            alterBixel(x, y, z, state);
            animationWait(speed);
            leds++;
        }
    }
}

void Animations::effectString(uint8_t *str, uint16_t speed)
{
    uint8_t x = 0;
    uint8_t z = 0;
    uint8_t i = 0;
    uint8_t chr[5];

    while (*str)
    {
        fontGetChar(*str++, chr);

        // Put a character on the back of the cube
        for (x = 0; x < 5; x++)
        {
            for (z = 0; z < 8; z++)
            {
                if ((chr[x] & (0x80 >> z)))
                {
                    setBixel(x + 2, 7, z);
                }
            }
        }

        // Shift the entire contents of the cube forward by 6 steps
        // before placing the next character
        for (i = 0; i < 6; i++)
        {
            animationWait(speed);
            shift(Y_AXIS, BACKWARD);
        }
    }
    // Shift the last character out of the cube.
    for (i = 0; i < 8; i++)
    {

        shift(Y_AXIS, BACKWARD);
    }
}

void Animations::animationWait(const uint speed)
{

}
