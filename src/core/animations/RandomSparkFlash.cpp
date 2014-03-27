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
#include "RandomSparkFlash.hpp"

RandomSparkFlash::RandomSparkFlash(const u_int16_t &speed, const u_int16_t &iterations, const u_int16_t &leds, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations),
    m_leds(leds)
{
}

void RandomSparkFlash::createAnimation()
{
    for (u_int16_t i = 0; i < m_iterations; i++)
    {
        for (u_int16_t b = 0; b < m_leds; b++)
        {
            setBixel(qrand() % CUBE_SIZE, qrand() % CUBE_SIZE,
                    qrand() % CUBE_SIZE);
        }
        if(m_abort)
            return;
        waitMs(getSpeed());
        fillCubeArray(0x00);
    }
    Q_EMIT done();
}

void RandomSparkFlash::createAnimationTooltip(AnimationItem *item)
{
    QString itemToolTip;

    setItemToolTipNameSpeed( &itemToolTip, item );

    itemToolTip.append(QString("LEDs: %1<br>"
                               "Iterations: %2")
                       .arg(item->getOptions().leds)
                       .arg(item->getOptions().iteration));

    item->setToolTip(itemToolTip);
}

QStringList& RandomSparkFlash::getAnimationProperties()
{
    list.clear();

    list.append( getName() );
    list.append( QString( "Speed:%1" ).arg( getSpeed() ) );
    list.append( QString( "Iterations:%1" ).arg( m_iterations ) );
    list.append( QString( "Leds:%1" ).arg( m_leds ) );

    return list;
}
