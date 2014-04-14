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
#ifndef RANDOMZLIFT_HPP
#define RANDOMZLIFT_HPP

#include "Animation.hpp"


namespace animations {
    class RandomZLift : public Animation
    {
        Q_OBJECT
        Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
    public:
        explicit RandomZLift(const u_int16_t &iterations = 10, const u_int16_t &speed = 100,
                             const QString &name = "Random Z Lift",QObject *parent = Q_NULLPTR);

        u_int16_t getIterations() const
        {
            return m_iterations;
        }

    public Q_SLOTS:
        virtual void createAnimation();
        virtual void createAnimationTooltipAsRichText(AnimationItem *item);
        virtual QStringList& getAnimationPropertiesAsPlainText( const AnimationItem *item );
        void setIterations(u_int16_t &iterations)
        {
            if (m_iterations != iterations)
                m_iterations = iterations;
        }

    private:
        u_int16_t m_iterations;
    };
    }// End namespace animations

#endif // RANDOMZLIFT_HPP
