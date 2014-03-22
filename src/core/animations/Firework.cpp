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
#include "Firework.hpp"

Firework::Firework(const u_int16_t &iterations, const u_int16_t &speed, const u_int8_t &particles, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations),
    m_particles(particles)
{
}

void Firework::createAnimation()
{
    u_int8_t origin_x = 3;
    u_int8_t origin_y = 3;
    u_int8_t origin_z = 3;

    qreal rand_y = 0;
    qreal rand_x = 0;
    qreal rand_z = 0;

    qreal slowrate = 0;
    qreal gravity = 0;

    // Particles and their position, x,y,z and their movement, dx, dy, dz
    qreal particlesA[m_particles][6];

    for (u_int16_t i = 0; i < m_iterations; i++)
    {

        origin_x = qrand() % 4 + 2;
        origin_y = qrand() % 4 + 2;
        origin_z = qrand() % 2 + 5;

        // shoot a particle up in the air
        for (u_int8_t e = 0; e < origin_z; e++)
        {
            setBixel(origin_x, origin_y, e);
            if(m_abort)
                return;
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
            particlesA[f][3] = 1 - rand_x / 100 ;
            particlesA[f][4] = 1 - rand_y / 100 ;
            particlesA[f][5] = 1 - rand_z / 100 ;
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
            if(m_abort)
                return;
            waitMs(getSpeed());
            fillCubeArray(0x00);
        }

    }
    fillCubeArray(0x00);
    Q_EMIT done();
}

const QString Firework::createAnimationTooltip()
{
    QString itemToolTip;
    Animation::createAnimationTooltip(&itemToolTip);

    itemToolTip.append(QString("Iterations: %1<br>"
                               "Particles: %2")
                       .arg(m_iterations)
                       .arg(m_particles));
    return itemToolTip;
}

void Firework::createAnimationTooltip(AnimationItem *item)
{
    QString itemToolTip;
    Animation::createAnimationTooltip(&itemToolTip, item);
    itemToolTip.append(QString("Iterations: %1<br>"
                               "Particles: %2")
                       .arg(item->getOptions().iteration)
                       .arg(item->getOptions().leds));
    item->setToolTip(itemToolTip);
}
