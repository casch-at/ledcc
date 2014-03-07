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
#ifndef RANDOMFILLER_HPP
#define RANDOMFILLER_HPP

#include "Animation.hpp"

class RandomFiller : public Animation
{
    Q_OBJECT
    Q_PROPERTY(BixelState state READ getState WRITE setState)
public:
    explicit RandomFiller(const u_int16_t &speed = 10,
                          const BixelState &state = ON,
                          const QString &name = "Random Filler",
                          QObject *parent = 0);
    BixelState getState() const
    {
        return m_state;
    }

signals:

public Q_SLOTS:
    virtual void createAnimation();
    void setState(const BixelState state)
    {
        if(m_state != state)
            m_state = state;
    }

private:
    BixelState m_state;
};

#endif // RANDOMFILLER_HPP
