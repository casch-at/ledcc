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
    Q_PROPERTY(bool invert READ getInvert WRITE setInvert)
    Q_PROPERTY(Axis axis READ getAxis WRITE setAxis)

public:
    explicit AxisNailWall(const u_int16_t &speed = 70,
                          const Axis &axis = X_AXIS,
                          const bool invert = false,
                          const QString &name = "Axis Nail Wall",
                          QObject *parent = 0);

    bool getInvert() const
    {
        return m_invert;
    }

    Axis getAxis() const
    {
        return m_axis;
    }
signals:

public Q_SLOTS:
    virtual void createAnimation();
    virtual void createAnimationTooltipAsRichText(AnimationItem *item);
    virtual QStringList& getAnimationPropertiesAsPlainText( const AnimationItem *item );
    void setInvert(const bool invert)
    {
        if(m_invert != invert)
            m_invert = invert;
    }

    void setAxis(const Axis axis)
    {
        if(m_axis != axis)
        m_axis = axis;
    }
private:

    Axis m_axis;
    bool m_invert;

};

#endif // AXISNAILWALL_HPP
