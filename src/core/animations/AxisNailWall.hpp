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
#ifndef AXISNAILWALL_HPP
#define AXISNAILWALL_HPP

#include "Animation.hpp"

class AxisNailWall : public Animation
{
    Q_OBJECT
    Q_PROPERTY(Draw::Direction direction READ getDirection WRITE setDirection)
    Q_PROPERTY(Draw::Axis axis READ getAxis WRITE setAxis)

public:
    explicit AxisNailWall(const quint16 &speed = 70,
                          const Draw::Axis &axis = X_AXIS,
                          const Draw::Direction &direction = Forward,
                          const QString &name = "Axis Nail Wall",
                          QObject *parent = 0);

    Draw::Direction getDirection() const
    {
        return m_direction;
    }

    Draw::Axis getAxis() const
    {
        return m_axis;
    }
public Q_SLOTS:
    virtual void createAnimation();
    void setDirection(const Draw::Direction direction)
    {
        if(m_direction != direction)
            m_direction = direction;
    }

    void setAxis(const Draw::Axis axis)
    {
        if(m_axis != axis)
            m_axis = axis;
    }
private:

    Draw::Axis m_axis;
    Draw::Direction m_direction;

};


#endif // AXISNAILWALL_HPP
