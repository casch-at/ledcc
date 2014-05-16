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
#ifndef ANIMATIONITEM_HPP
#define ANIMATIONITEM_HPP

#include <QListWidgetItem>
#include "Options.hpp"
#include <QObject>

//FIXME:: Translation does not work for this object because the text is dynamicly

class AnimationItem : public QObject, public QListWidgetItem
{
    Q_OBJECT
public:
    class Options
    {
    public:
        Options(const QString &text = "", const quint16 &speed = 1, const quint16 &delay = 0,
                const quint16 &leds = 0, const quint16 &iteration = 0, const Draw::Direction &direction = Draw::Backward,
                const Draw::Axis &axis = Draw::X_AXIS, const bool invert = false, const Draw::BixelState state= Draw::OFF)
            : m_text(text),
              m_speed(speed),
              m_delay(delay),
              m_leds(leds),
              m_iteration(iteration),
              m_direction(direction),
              m_axis(axis),
              m_invert(invert),
              m_state(state) { }

        QString m_text;
        quint16 m_speed;
        quint16 m_delay;
        quint16 m_leds;
        quint16 m_iteration;
        Draw::Direction m_direction;
        Draw::Axis m_axis;
        bool m_invert;
        Draw::BixelState m_state;
    };
    enum Arguments{
        Speed       = (1 << 0),
        Direction   = (1 << 1),
        Axis        = (1 << 2),
        Leds        = (1 << 3),
        Particls    = (1 << 4),
        Delay       = (1 << 5),
        Iterations  = (1 << 6),
        Invert      = (1 << 7),
        CenterStart = (1 << 8),
        Text        = (1 << 9),
        LedState    = (1 << 10)
        };
    static const int TOTAL_ARGUMENTS = 11;
    explicit AnimationItem(QListWidget *view = 0, int type = UserType);
    explicit AnimationItem(const QString &text, QListWidget *view = 0, int type = UserType);

    virtual ~AnimationItem();
    virtual AnimationItem *clone() const;

    inline const Options * getOptions(void) const{
        return &m_options;
    }

    inline void setOptions(Options &options){
        m_options = options;
    }

    inline int getAvailableAnimationOptions() const {
        return m_availableAnimationOptions;
    }

    inline void setAvailableAnimationOptions(const int &options){
        if(m_availableAnimationOptions != options)
            m_availableAnimationOptions = options;
    }
    void createAnimationTooltipAsRichText();
    QStringList getAnimationPropertiesAsPlainText();
private:
    //    void initOptions(void);
    int m_availableAnimationOptions;
    Options m_options;
};

#endif // ANIMATIONITEM_HPP
