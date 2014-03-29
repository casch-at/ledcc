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
    Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
    Q_PROPERTY(u_int16_t leds READ getLeds WRITE setLeds)
public:
    explicit RandomSparkFlash(const u_int16_t &speed = 80,
                              const u_int16_t &iterations = 20,
                              const u_int16_t &leds = 40,
                              const QString &name = "Random Spark Flash",
                              QObject *parent = 0);

    u_int16_t getIterations() const
    {
        return m_iterations;
    }

    u_int16_t getLeds() const
    {
        return m_leds;
    }

signals:

public Q_SLOTS:
    virtual void createAnimation();
    virtual void createAnimationTooltipAsRichText(AnimationItem *item);
    virtual QStringList& getAnimationPropertiesAsPlainText( const AnimationItem *item );
    void setIterations(const u_int16_t iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }
    void setLeds(const u_int16_t leds)
    {
        if(m_leds != leds)
            m_leds = leds;
    }
private:
    u_int16_t m_iterations;
    u_int16_t m_leds;
};

#endif // RANDOMSPARKFLASH_HPP
