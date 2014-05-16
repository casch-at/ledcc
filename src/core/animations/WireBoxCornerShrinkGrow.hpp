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



class WireBoxCornerShrinkGrow : public Animation
{
    Q_OBJECT
    Q_PROPERTY(quint16 iterations READ getIterations WRITE setIterations)
    Q_PROPERTY(quint8 rotate READ getRotate WRITE setRotate)
    Q_PROPERTY(quint8 flip READ getFlip WRITE setFlip)
public:
    explicit WireBoxCornerShrinkGrow(const quint16 &iterations = 1,
                                     const quint16 &speed = 50,
                                     const quint8 &rotate = 1,
                                     const quint8 &flip=0,
                                     const QString &name = BIAS::WireBoxCornerShrinkGrow,
                                     QObject *parent = Q_NULLPTR);


    quint16 getIterations() const
    {
        return m_iterations;
    }

    quint8 getRotate() const
    {
        return m_rotate;
    }

    quint8 getFlip() const
    {
        return m_flip;
    }

public Q_SLOTS:
    virtual void createAnimation();
    virtual QStringList& getAnimationPropertiesAsPlainText( const AnimationItem *item );
    void setIterations(const quint16 &iterations)
    {
        if(m_iterations != iterations)
            m_iterations = iterations;
    }

    void setRotate(const quint8 &rotate)
    {
        if(m_rotate != rotate)
            m_rotate = rotate;
    }

    void setFlip(const quint8 &flip)
    {
        if(m_flip != flip)
            m_flip = flip;
    }

private:
    void createWireBoxCorner(const quint8 rotate, const quint8 flip);
    quint16 m_iterations;
    quint8 m_rotate;
    quint8 m_flip;
};
#endif // WIREBOXCORNERSHRINKGROW_HPP
