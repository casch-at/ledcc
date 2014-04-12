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
#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include "Draw.hpp"
//#include "AnimationOptions.hpp"
#include "AnimationItem.hpp"
#ifdef _DEBUG_
#include <QDebug>
#endif


class Animation : public Draw
{
    Q_OBJECT
public:
    explicit  Animation(const u_int16_t &speed, const QString &name,QObject *parent = Q_NULLPTR);

    QString getName(void) const{
        return m_name;
    }
    void setSpeed(const u_int16_t &speed)
    {
        if(m_speed != speed){
            m_speed = speed;
        }
    }
    u_int16_t getSpeed(void) const
    {
        return m_speed;
    }

    void sendBixelZ(u_int8_t x, u_int8_t y, u_int8_t z);
    void effectZUpDownMove(QVector<u_int8_t> &destination,
                           QVector<u_int8_t> &position, Draw::Axis axis);

    void waitMs(const u_int16_t &time);


    bool m_abort;
    void setItemToolTipNameSpeed(QString *itemToolTip, AnimationItem *item);
Q_SIGNALS:
    void done();
    void sendData(const CubeArray &cubeFrame);
public Q_SLOTS:
    virtual void createAnimationTooltipAsRichText(AnimationItem *item) = 0;
    virtual void createAnimation(void) = 0;
    virtual QStringList& getAnimationPropertiesAsPlainText(const AnimationItem *item ) = 0;
protected:
    QStringList list;
private:
    u_int16_t m_speed;
    QString m_name;
};

#endif // ANIMATION_HPP
