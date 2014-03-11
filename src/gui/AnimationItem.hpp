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
#ifndef ANIMATIONITEM_HPP
#define ANIMATIONITEM_HPP

#include <QListWidgetItem>
#include "AnimationOptions.hpp"

class AnimationItem : public QListWidgetItem
{
public:
    explicit AnimationItem(QListWidget *view = 0, int type = Type);
    explicit AnimationItem(const QString &text, QListWidget *view = 0, int type = Type);

    virtual AnimationItem *clone() const;
//    AnimationItem(const AnimationItem &other);
//    ~AnimationItem();
    AnimationOptions::Options getOptions(void) const{
        return m_options;
    }
    void setOptions(const AnimationOptions::Options &options){
        m_options = options;
    }

private:
//    void initOptions(void);

    AnimationOptions::Options m_options;

};
#endif // ANIMATIONITEM_HPP
