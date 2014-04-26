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
#include "AnimationItem.hpp"
#include "AnimationOptions.hpp"

#ifdef _DEBUG_
#include <QDebug>
#endif
#include <QObject>

AnimationItem::AnimationItem(QListWidget *view, int type):
    QListWidgetItem(view,type)
{
//    initOptions();
//    qRegisterMetaType<AnimationItem>("AnimationItem");
}

AnimationItem::AnimationItem(const QString &text, QListWidget *view, int type):
    QListWidgetItem(text,view,type)
{
    //    initOptions();
}

AnimationItem::~AnimationItem()
{
}

/**
 * @brief Reimplemented clone function to clone the settings of the item
 *
 * @return AnimationItem
 */
AnimationItem *AnimationItem::clone() const
{
    AnimationItem *item = new AnimationItem(text());
    item->setToolTip(toolTip());
    item->setOptions(const_cast<Options&>(m_options));
    item->setAvailableAnimationOptions(m_availableAnimationOptions);
    return item;
}

/*!
 \brief

*/
void AnimationItem:: createAnimationTooltipAsRichText()
{
    QString itemToolTip;

    // Append name of the animation.
    itemToolTip.append(QString("<p style='white-space:pre'><font color=#00FFFF><b>%1 Animation</b></font><br>").arg(text()));
    for (int i = 0; i < AnimationOptions::TOTAL_ARGUMENTS; i++) {
        switch ((1<<i) & m_availableAnimationOptions)
        {
        case AnimationOptions::Speed:
            itemToolTip.append(QString("Speed: %1<br>").arg(m_options.m_speed));
            break;
        case AnimationOptions::Direction:
            itemToolTip.append( m_options.m_direction == Draw::Forward ? "Direction: Forward<br>" : "Direction: Backward<br>");
            break;
        case AnimationOptions::Axis:
            if(m_options.m_axis == Draw::X_AXIS)
                itemToolTip.append("Axis: X-Axis<br>");
            else if(m_options.m_axis == Draw::Y_AXIS)
                itemToolTip.append("Axis: Y-Axis<br>");
            else
                itemToolTip.append("Axis: Z-Axis<br>");
            break;
        case AnimationOptions::Leds:
            itemToolTip.append(QString("Particles: %1<br>").arg( m_options.m_leds ));
            break;
        case AnimationOptions::Particls:
            itemToolTip.append(QString("Particles: %1<br>").arg( m_options.m_leds ));
            break;
        case AnimationOptions::Delay:
            itemToolTip.append(QString("Delay: %1<br>").arg(m_options.m_delay));
            break;
        case AnimationOptions::Iterations:
            itemToolTip.append( QString( "Iterations: %1<br>" ).arg( m_options.m_iteration ) );
            break;
        case AnimationOptions::Invert:
            itemToolTip.append(m_options.m_invert == true ? "Invert: YES<br>" : "Invert: NO<br>");
            break;
        case AnimationOptions::CenterStart:
            itemToolTip.append(m_options.m_invert == true ? "Start from center: YES<br>" : "Start from center: NO<br>");
            break;
        case AnimationOptions::Text:
            itemToolTip.append("Current Text: " + m_options.m_text.toLatin1() + "<br>");
            break;
        case AnimationOptions::LedState:
            itemToolTip.append( QString( "Initial LED State: %1<br>" ).arg( m_options.m_invert == Draw::ON ? "ON" : "OFF" ) );
            break;
        default:
            break;
        }
    }
    setToolTip(itemToolTip);
}


/*!
 \brief

 \return QStringList
*/
QStringList AnimationItem::getAnimationPropertiesAsPlainText()
{
    QStringList list;

    // Append name of the animation.
    list.append(text());
    for (int i = 0; i < AnimationOptions::TOTAL_ARGUMENTS; i++) {
        switch ((1<<i) & m_availableAnimationOptions)
        {
        case AnimationOptions::Speed:
            list.append(QString("Speed:%1").arg(m_options.m_speed));
            break;
        case AnimationOptions::Direction:
            list.append( m_options.m_direction == Draw::Forward ? "Direction:Forward" : "Direction:Backward");
            break;
        case AnimationOptions::Axis:
            if(m_options.m_axis == Draw::X_AXIS)
                list.append("Axis:X-Axis");
            else if(m_options.m_axis == Draw::Y_AXIS)
                list.append("Axis:Y-Axis");
            else
                list.append("Axis:Z-Axis");
            break;
        case AnimationOptions::Leds:
            list.append(QString("LEDs:%1").arg( m_options.m_leds ));
            break;
        case AnimationOptions::Particls:
            list.append(QString("Particles:%1").arg( m_options.m_leds ));
            break;
        case AnimationOptions::Delay:
            list.append(QString("Delay:%1").arg(m_options.m_delay));
            break;
        case AnimationOptions::Iterations:
            list.append( QString( "Iterations:%1" ).arg( m_options.m_iteration ) );
            break;
        case AnimationOptions::Invert:
            list.append(m_options.m_invert == true ? "Invert:YES" : "Invert:NO");
            break;
        case AnimationOptions::CenterStart:
            list.append(m_options.m_invert == true ? "Start from center:YES" : "Start from center:NO");
            break;
        case AnimationOptions::Text:
            list.append("Current Text: " + m_options.m_text.toLatin1());
            break;
        case AnimationOptions::LedState:
            list.append( QString( "Initial LED State:%1" ).arg( m_options.m_invert == Draw::ON ? "ON" : "OFF" ) );
            break;
        default:
            break;
        }
    }
    return list;
}


