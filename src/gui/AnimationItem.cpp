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
#ifdef _DEBUG_
#include <QDebug>
#endif

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
    item->setOptions(m_options);
    item->setAvailableAnimationOptions(m_availableAnimationOptions);
    return item;
}


