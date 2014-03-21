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
#ifndef WALL_HPP
#define WALL_HPP
#include "Animation.hpp"

class Wall : public Animation
{
    Q_OBJECT
    Q_PROPERTY(Direction direction READ getDirection WRITE setDirection)
    Q_PROPERTY(Axis axis READ getAxis WRITE setAxis)
public:
    explicit Wall(const u_int16_t &speed = 50,
                  const Axis &axis = X_AXIS,
                  const Direction &direction = FORWARD,
                  const QString &name = "Wall",
                  QObject *parent = Q_NULLPTR);

    Direction getDirection(void) const{
        return m_direction;
    }
    Axis getAxis() const
    {
        return m_axis;
    }

public Q_SLOTS:
    virtual void createAnimation();
    virtual const QString createAnimationTooltip();
    void setDirection(Direction arg)
    {
        if(m_direction != arg)
            m_direction = arg;
    }

    void setAxis(Axis arg)
    {
        if(m_axis != arg)
            m_axis = arg;
    }
private:
    Direction m_direction;
    Axis m_axis;
};

#endif // WALL_HPP

