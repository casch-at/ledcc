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
    Q_PROPERTY(Axis axis READ getAxis WRITE setAxis)
    Q_PROPERTY(Direction direction READ getDirection WRITE setDirection)
public:
    explicit Loadbar(const u_int16_t &speed = 50, const Axis &axis = X_AXIS,
                     const QString &name = "Loadbar", const Direction &direction = FORWARD, QObject *parent = 0);
    Axis getAxis() const
    {
        return m_axis;
    }

    Direction getDirection() const
    {
        return m_direction;
    }

signals:

public Q_SLOTS:
    virtual void createAnimation();
    virtual void createAnimationTooltip(AnimationItem *item);
    virtual QStringList& getAnimationProperties();
    void setAxis(const Axis axis)
    {
        if(m_axis != axis)
            m_axis = axis;
    }

    void setDirection(const Direction direction)
    {
        if(m_direction != direction)
            m_direction = direction;
    }

private:

    Axis m_axis;
    Direction m_direction;
};

#endif // LOADBAR_HPP
