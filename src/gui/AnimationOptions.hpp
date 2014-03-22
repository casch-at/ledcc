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
#ifndef ANIMATIONOPTIONS_HPP
#define ANIMATIONOPTIONS_HPP

#include <QtWidgets/QGroupBox>
#include "Global.hpp"
#include "animations/Draw.hpp"
//#include "AnimationItem.hpp"

namespace Ui {
    class AnimationOptions;
    }

class AnimationOptions : public QGroupBox
{
    Q_OBJECT

public:
   struct  Options{
        QString text;
        u_int16_t speed;
        u_int16_t delay;
        u_int16_t leds;
        u_int16_t iteration;
        Draw::Direction direction;
        Draw::Axis axis;
        bool invert;
        Draw::BixelState state;
    };

    explicit AnimationOptions(QWidget *parent = 0);
    ~AnimationOptions();
    Options *getAnimationSettings(void);
Q_SIGNALS:
    void optionsReady(const Options &animationOptions);
public Q_SLOTS:
    void displayAnimationOptions(const Options options);
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_applyPushB_clicked();

private:
    Ui::AnimationOptions *ui;
    Options m_options;
    Q_DISABLE_COPY(AnimationOptions)
};

#endif // ANIMATIONOPTIONSGROUPBOX_HPP
