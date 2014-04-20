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
#include "RandomZLift.hpp"


RandomZLift::RandomZLift(const u_int16_t &iterations, const u_int16_t &speed, const QString &name, QObject *parent):
    Animation(speed,name,parent),
    m_iterations(iterations)
{
}

void RandomZLift::createAnimation()
{
    QVector<u_int8_t> destination(CUBE_ARRAY_SIZE);
    QVector<u_int8_t> position(CUBE_ARRAY_SIZE);

    for (u_int16_t i = 0; i < CUBE_ARRAY_SIZE; i++)
    {
        position[i] = 4;
        destination[i] = qrand() % 8;
    }

//    for (u_int8_t i = 0; i < CUBE_SIZE; i++)
//    {
//        effectZUpDownMove(destination, position, Z_AXIS);
//        waitMs(getSpeed());
//    }

    for (u_int16_t i = 0; i < m_iterations; i++)
    {
        for (u_int8_t j = 0; j < CUBE_SIZE; j++)
        {
            effectZUpDownMove(destination, position, Z_AXIS);

            if(m_abort)
                return;

            waitMs(getSpeed());
        }

        if(m_abort)
            return;

        waitMs(getSpeed() * 4);

        for (u_int8_t j = 0; j < CUBE_ARRAY_SIZE / 4; j++)
        {
            destination[qrand() % CUBE_ARRAY_SIZE] = qrand() % CUBE_SIZE;
        }
    }
    Q_EMIT done();
}

void RandomZLift::createAnimationTooltipAsRichText(AnimationItem *item)
{
    QString itemToolTip;

    setItemToolTipNameSpeed( &itemToolTip, item );

    itemToolTip.append(QString("Iterations: %1")
                       .arg(item->getOptions()->m_iteration));

    item->setToolTip(itemToolTip);
}

QStringList& RandomZLift::getAnimationPropertiesAsPlainText(const AnimationItem *item)
{
    list.clear();

    list.append(getName());
    list.append(QString("Speed:%1").arg( item->getOptions()->m_speed ));
    list.append( QString( "Iterations:%1" ).arg( item->getOptions()->m_iteration ) );

    return list;
}
