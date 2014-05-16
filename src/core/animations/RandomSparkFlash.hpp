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
#ifndef RANDOMSPARKFLASH_HPP
#define RANDOMSPARKFLASH_HPP

#include "Animation.hpp"


class RandomSparkFlash : public Animation
{
    Q_OBJECT
    Q_PROPERTY(quint16 iterations READ getIterations WRITE setIterations)
    Q_PROPERTY(quint16 leds READ getLeds WRITE setLeds)
public:
    explicit RandomSparkFlash(const quint16 &speed = 80,
                              const quint16 &iterations = 20,
                              const quint16 &leds = 40,
                              const QString &name = BIAS::RandomSparkFlash,
                              QObject *parent = 0);

    quint16 getIterations() const
    {
        return m_iterations;
    }

    quint16 getLeds() const
    {
        return m_leds;
    }

public Q_SLOTS:
    virtual void createAnimation();
    void setIterations(const quint16 iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }
    void setLeds(const quint16 leds)
    {
        if(m_leds != leds)
            m_leds = leds;
    }
private:
    quint16 m_iterations;
    quint16 m_leds;
};

#endif // RANDOMSPARKFLASH_HPP
