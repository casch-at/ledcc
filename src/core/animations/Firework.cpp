#include "Firework.hpp"

Firework::Firework(const u_int16_t &iterations, const u_int16_t &speed, const u_int8_t &particles, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations),
    m_particles(particles)
{
}

void Firework::createAnimation()
{
    uint8_t origin_x = 3;
    uint8_t origin_y = 3;
    uint8_t origin_z = 3;

    uint8_t rand_y = 0;
    uint8_t rand_x = 0;
    uint8_t rand_z = 0;

    float slowrate = 0;
    float gravity = 0;

    // Particles and their position, x,y,z and their movement, dx, dy, dz
    float particlesA[m_particles][6];

    for (u_int16_t i = 0; i < m_iterations; i++)
    {

        origin_x = qrand() % 4 + 2;
        origin_y = qrand() % 4 + 2;
        origin_z = qrand() % 2 + 5;

        // shoot a particle up in the air
        for (u_int8_t e = 0; e < origin_z; e++)
        {
            setBixel(origin_x, origin_y, e);
            waitMs(getSpeed() * 2);
            fillCubeArray(0x00);
        }

        // Fill particle array
        for (u_int8_t f = 0; f < m_particles; f++)
        {
            // Position
            particlesA[f][0] = origin_x;
            particlesA[f][1] = origin_y;
            particlesA[f][2] = origin_z;

            rand_x = qrand() % 200;
            rand_y = qrand() % 200;
            rand_z = qrand() % 200;

            // Movement
            particlesA[f][3] = 1 - static_cast<float>( rand_x / 100 ); // dx
            particlesA[f][4] = 1 - static_cast<float>( rand_y / 100 ); // dy
            particlesA[f][5] = 1 - static_cast<float>( rand_z / 100 ); // dz
        }

        // explode
        for (u_int8_t e = 0; e < m_particles; e++)
        {
            slowrate = 1 + tan((e + 0.1) / 20) * 10;

            gravity = tan((e + 0.1) / 20) / 2;

            for (u_int8_t f = 0; f < m_particles; f++)
            {
                particlesA[f][0] += particlesA[f][3] / slowrate;
                particlesA[f][1] += particlesA[f][4] / slowrate;
                particlesA[f][2] += particlesA[f][5] / slowrate;
                particlesA[f][2] -= gravity;

                setBixel(particlesA[f][0], particlesA[f][1], particlesA[f][2]);

            }

            waitMs(getSpeed());
            fillCubeArray(0x00);
        }

    }
}
