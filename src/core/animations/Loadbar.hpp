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
#ifndef LOADBAR_HPP
#define LOADBAR_HPP

#include "Animation.hpp"

class Loadbar : public Animation
{
    Q_OBJECT
    Q_PROPERTY(Draw::Axis axis READ getAxis WRITE setAxis)
    Q_PROPERTY(Draw::Direction direction READ getDirection WRITE setDirection)
public:
    explicit Loadbar(const u_int16_t &speed = 50, const Draw::Axis &axis = X_AXIS,
                     const QString &name = "Loadbar", const Draw::Direction &direction = Forward, QObject *parent = 0);
    Draw::Axis getAxis() const
    {
        return m_axis;
    }

    Draw::Direction getDirection() const
    {
        return m_direction;
    }

public Q_SLOTS:
    virtual void createAnimation();
    virtual void createAnimationTooltipAsRichText(AnimationItem *item);
    virtual QStringList& getAnimationPropertiesAsPlainText( const AnimationItem *item );
    void setAxis(const Draw::Axis axis)
    {
        if(m_axis != axis)
            m_axis = axis;
    }

    void setDirection(const Draw::Direction direction)
    {
        if(m_direction != direction)
            m_direction = direction;
    }

private:

    Draw::Axis m_axis;
    Draw::Direction m_direction;
};
#endif // LOADBAR_HPP
