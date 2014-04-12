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
#include "Loadbar.hpp"

Loadbar::Loadbar(const u_int16_t &speed, const Draw::Axis &axis, const QString &name, const Draw::Direction &direction, QObject *parent):
    Animation(speed,name,parent),
    m_axis(axis),
    m_direction(direction)
{
}

void Loadbar::createAnimation()
{
    fillCubeArray(0x00);
    for (u_int8_t i = 0; i < CUBE_SIZE; i++)
    {
        setPlane(m_axis, i);
        if(m_abort)
            return;
        waitMs(getSpeed());
    }
    waitMs(getSpeed() * 2);
    for (u_int8_t i = 0; i < CUBE_SIZE; i++)
    {
        clearPlane(m_axis, i);
        if(m_abort)
            return;
        waitMs(getSpeed());
    }
    Q_EMIT done();
}


void Loadbar::createAnimationTooltipAsRichText(AnimationItem *item)
{
    QString itemToolTip;

    setItemToolTipNameSpeed( &itemToolTip, item );

    if( item->getOptions()->axis == X_AXIS )
        itemToolTip.append("Axis: X-Axis<br>");
    else if( item->getOptions()->axis == Y_AXIS )
        itemToolTip.append("Axis: Y-Axis<br>");
    else
        itemToolTip.append("Axis: Z-Axis<br>");

    itemToolTip.append( item->getOptions()->direction == FORWARD ? "Direction: Forward" : "Direction: Backward");
    item->setToolTip(itemToolTip);
}

QStringList& Loadbar::getAnimationPropertiesAsPlainText( const AnimationItem *item )
{
    list.clear();

    list.append(getName());
    list.append(QString("Speed:%1").arg( item->getOptions()->speed));
    if( item->getOptions()->axis == X_AXIS )
        list.append( QString("Axis:X_AXIS") );
    else if( item->getOptions()->axis == Y_AXIS )
        list.append( QString("Axis:Y_AXIS") );
    else
        list.append( QString("Axis:Z_AXIS") );
    list.append( QString("Direction:%1").arg( item->getOptions()->direction == FORWARD ? "Forward" : "Backward"));

    return list;
}
