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
#ifndef WIREBOXCORNERSHRINKGROW_HPP
#define WIREBOXCORNERSHRINKGROW_HPP

#include "Animation.hpp"

namespace animations {

    class WireBoxCornerShrinkGrow : public Animation
    {
        Q_OBJECT
        Q_PROPERTY(u_int16_t iterations READ getIterations WRITE setIterations)
        Q_PROPERTY(u_int8_t rotate READ getRotate WRITE setRotate)
        Q_PROPERTY(u_int8_t flip READ getFlip WRITE setFlip)
    public:
        explicit WireBoxCornerShrinkGrow(const u_int16_t &iterations = 1,
                                         const u_int16_t &speed = 50,
                                         const u_int8_t &rotate = 1,
                                         const u_int8_t &flip=0,
                                         const QString &name = "Wire Box Corner Shrink Grow",
                                         QObject *parent = Q_NULLPTR);


        u_int16_t getIterations() const
        {
            return m_iterations;
        }

        u_int8_t getRotate() const
        {
            return m_rotate;
        }

        u_int8_t getFlip() const
        {
            return m_flip;
        }

    public Q_SLOTS:
        virtual void createAnimation();
        virtual void createAnimationTooltipAsRichText(AnimationItem *item);
        virtual QStringList& getAnimationPropertiesAsPlainText( const AnimationItem *item );
        void setIterations(const u_int16_t &iterations)
        {
            if(m_iterations != iterations)
                m_iterations = iterations;
        }

        void setRotate(const u_int8_t &rotate)
        {
            if(m_rotate != rotate)
                m_rotate = rotate;
        }

        void setFlip(const u_int8_t &flip)
        {
            if(m_flip != flip)
                m_flip = flip;
        }

    private:
        void createWireBoxCorner(const u_int8_t rotate, const u_int8_t flip);
        u_int16_t m_iterations;
        u_int8_t m_rotate;
        u_int8_t m_flip;
    };
    }// End namespace animations
#endif // WIREBOXCORNERSHRINKGROW_HPP
