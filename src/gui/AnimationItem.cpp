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
//    QObject(view->parent()),
    QListWidgetItem(view,type)
{
//    initOptions();
//    qRegisterMetaType<AnimationItem>("AnimationItem");
}

AnimationItem::AnimationItem(const QString &text, QListWidget *view, int type):
//    QObject(view->parent()),
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
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    itemToolTip.append(QString(tr("<p style='white-space:pre'><font color=#FFFFFF><b>%1 Animation</b></font><br>")).arg(text()));
#else // Windows
    itemToolTip.append(QString(tr("<p style='white-space:pre'><font color=#000000><b>%1 Animation</b></font><br>")).arg(text()));
#endif
    for (int i = 0; i < AnimationItem::TOTAL_ARGUMENTS; i++) {
        switch ((1<<i) & m_availableAnimationOptions)
        {
        case AnimationItem::Speed:
            itemToolTip.append(QString(tr("Speed: %1<br>")).arg(m_options.m_speed));
            break;
        case AnimationItem::Direction:
            itemToolTip.append( m_options.m_direction == Draw::Forward ? tr("Direction: Forward<br>") : tr("Direction: Backward<br>"));
            break;
        case AnimationItem::Axis:
            if(m_options.m_axis == Draw::X_AXIS)
                itemToolTip.append(tr("Axis: X-Axis<br>"));
            else if(m_options.m_axis == Draw::Y_AXIS)
                itemToolTip.append(tr("Axis: Y-Axis<br>"));
            else
                itemToolTip.append(tr("Axis: Z-Axis<br>"));
            break;
        case AnimationItem::Leds:
            itemToolTip.append(QString(tr("Particles: %1<br>")).arg( m_options.m_leds ));
            break;
        case AnimationItem::Particls:
            itemToolTip.append(QString(tr("Particles: %1<br>")).arg( m_options.m_leds ));
            break;
        case AnimationItem::Delay:
            itemToolTip.append(QString(tr("Delay: %1<br>")).arg(m_options.m_delay));
            break;
        case AnimationItem::Iterations:
            itemToolTip.append( QString(tr("Iterations: %1<br>")).arg( m_options.m_iteration ) );
            break;
        case AnimationItem::Invert:
            itemToolTip.append(m_options.m_invert == true ? tr("Invert: YES<br>") : tr("Invert: NO<br>"));
            break;
        case AnimationItem::CenterStart:
            itemToolTip.append(m_options.m_invert == true ? tr("Start from center: YES<br>") : tr("Start from center: NO<br>"));
            break;
        case AnimationItem::Text:
            itemToolTip.append(tr("Current Text: ") + m_options.m_text.toLatin1() + "<br>");
            break;
        case AnimationItem::LedState:
            itemToolTip.append( QString( tr("Initial LED State: %1<br>") ).arg( m_options.m_invert == Draw::ON ? tr("ON") : tr("OFF") ) );
            break;
        default:
            break;
        }
    }
    itemToolTip.remove(itemToolTip.length() - 4, 4); // Remove the last line break (<br>)
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
    for (int i = 0; i < AnimationItem::TOTAL_ARGUMENTS; i++) {
        switch ((1<<i) & m_availableAnimationOptions)
        {
        case AnimationItem::Speed:
            list.append(QString(tr("Speed:%1")).arg(m_options.m_speed));
            break;
        case AnimationItem::Direction:
            list.append( m_options.m_direction == Draw::Forward ? tr("Direction:Forward") : tr("Direction:Backward"));
            break;
        case AnimationItem::Axis:
            if(m_options.m_axis == Draw::X_AXIS)
                list.append(tr("Axis:X-Axis"));
            else if(m_options.m_axis == Draw::Y_AXIS)
                list.append(tr("Axis:Y-Axis"));
            else
                list.append(tr("Axis:Z-Axis"));
            break;
        case AnimationItem::Leds:
            list.append(QString(tr("LEDs:%1")).arg( m_options.m_leds ));
            break;
        case AnimationItem::Particls:
            list.append(QString(tr("Particles:%1")).arg( m_options.m_leds ));
            break;
        case AnimationItem::Delay:
            list.append(QString(tr("Delay:%1")).arg(m_options.m_delay));
            break;
        case AnimationItem::Iterations:
            list.append( QString( tr("Iterations:%1") ).arg( m_options.m_iteration ) );
            break;
        case AnimationItem::Invert:
            list.append(m_options.m_invert == true ? tr("Invert:YES") : tr("Invert:NO"));
            break;
        case AnimationItem::CenterStart:
            list.append(m_options.m_invert == true ? tr("Start from center:YES") : tr("Start from center:NO"));
            break;
        case AnimationItem::Text:
            list.append(tr("Current Text: ") + m_options.m_text.toLatin1());
            break;
        case AnimationItem::LedState:
            list.append( QString( tr("Initial LED State:%1") ).arg( m_options.m_invert == Draw::ON ? tr("ON") : tr("OFF") ) );
            break;
        default:
            break;
        }
    }
    return list;
}


