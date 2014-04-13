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
#include "AnimationOptions.hpp"
#include "ui_AnimationOptions.h"
#include "alt_key.hpp"
#include "aqp.hpp"
#include "AnimationItem.hpp"

// Qt Includes
#ifdef _DEBUG_
#include <QDebug>
#endif

AnimationOptions::AnimationOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimationOptions),
    m_animationOptionsModefied(false),
    m_animationAt(0)
{
    // Setup the user interface
    ui->setupUi(this);

    // Create connections ( Signal & Slot )
    connect(ui->m_nextPB, &QPushButton::pressed, this, &AnimationOptions::optionsNextAnimation);
    connect(ui->m_prevPB, &QPushButton::pressed, this, &AnimationOptions::optionsPrevAnimation);
    connect(ui->m_applyPB, &QPushButton::pressed, this, &AnimationOptions::applyAnimationOptions);
    connect(ui->m_cancelPB, &QPushButton::pressed, this, &AnimationOptions::cancel);
    connect(ui->m_okPB, &QPushButton::pressed, this, &AnimationOptions::ok);
    AQP::accelerateWidget(this);


}

AnimationOptions::~AnimationOptions()
{
    delete ui;
}

/*!
 \brief Slot gets called when the user has pressed the edit button from \a AnimationPlayListWidget.
 The first animation options are displayed, and the window will be shown, all other animation options
 will be inserted either in \a optionsNextAnimation or \a optionsPrevAnimation slot function.

 \param itemsList
*/
void AnimationOptions::adjustAnimationOptions(QList<AnimationItem*> &itemsList)
{
    if(itemsList.isEmpty())
       return;
    m_itemList = itemsList;
    optionsNextAnimation();

    show();
}


/*!
 \brief Gets called if the ui changes (e.g. the language)

 \param e
*/
void AnimationOptions::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
 \brief

*/
void AnimationOptions::applyAnimationOptions()
{
    if(m_animationOptionsModefied)
        Q_EMIT applyNewAnimationArguments(m_animationToUpdate);
}

/*!
 \brief

*/
void AnimationOptions::cancel()
{

}

/*!
 \brief

*/
void AnimationOptions::ok()
{

}
//    if(options->axis == Draw::X_AXIS )
//        ui->axisComB->setCurrentIndex(0);
//    else if(options->axis == Draw::Y_AXIS )
//        ui->axisComB->setCurrentIndex(1);
//    else if(options->axis == Draw::Z_AXIS )
//        ui->axisComB->setCurrentIndex(2);
//    ui->delaySpinB->setValue(static_cast<int>(options->delay));
//    if( options->direction == Draw::BACKWARD )
//        ui->directionComB->setCurrentIndex(0);
//    else if(options->direction == Draw::FORWARD )
//        ui->directionComB->setCurrentIndex(1);
//    if(options->invert == true && options->state == Draw::ON){
//        ui->invertSpinB->setValue(1);
//    }else{
//        ui->invertSpinB->setValue(0);
//    }
//    ui->iterationsSpinB->setValue( static_cast<int>(options->iteration));
//    ui->ledsSpinB->setValue( static_cast<int>(options->leds));
//    ui->speedSpinB->setValue( static_cast<int>(options->speed));
//    ui->textLineE->setText(options->text.isEmpty() == true ? "" : options->text);


/*!
 \brief

*/
void AnimationOptions::optionsNextAnimation()
{
    AnimationItem *animation;
    if (m_itemList.count() == m_animationAt )
        ui->m_nextPB->setDisabled(true);

    animation = m_itemList.at(m_animationAt);

    hideShowWidgetsDisplayOptions(animation->getAvailableAnimationOptions(), animation->getOptions());


    m_animationAt++;
}


/*!
 \brief

*/
void AnimationOptions::optionsPrevAnimation()
{
    m_animationAt--;
}


/*!
 \brief

 \param hasOption
 \param options
*/
void AnimationOptions::hideShowWidgetsDisplayOptions(const int &hasOption, const Options *options)
{
    foreach (QObject *obj, ui->m_optionsGridL->children()) {
        dynamic_cast<QWidget*>(obj)->setVisible(false);
    }
    for (int i = 0; i < TOTAL_ARGUMENTS; i++) {
        switch ( (1<<i) & hasOption )
        {
        case Speed:
            ui->m_speedLabel->setVisible(!ui->m_speedLabel->isVisible());
            ui->m_speedSpinB->setVisible(!ui->m_speedSpinB->isVisible());
            ui->m_speedSpinB->setValue(options->speed);
            break;
        case Direction:
            ui->m_directionLabel->setVisible(!ui->m_directionLabel->isVisible());
            ui->m_directionComB->setVisible(!ui->m_directionComB->isVisible());
            ui->m_directionComB->setCurrentIndex(options->direction == Draw::Backward ? 0 : 1);
            break;
        case Axis:
            ui->m_axisLabel->setVisible(!ui->m_axisLabel->isVisible());
            ui->m_axisComB->setVisible(!ui->m_axisComB->isVisible());
            switch (options->axis)
            {
            case Draw::Y_AXIS:
                ui->m_axisComB->setCurrentIndex(1);
                break;
            case Draw::Z_AXIS:
                ui->m_axisComB->setCurrentIndex(2);
                break;
            default: // Default Draw::X_AXIS
                    ui->m_axisComB->setCurrentIndex(0);
                break;
            }
            break;
        case Leds:
            break;
        case Particls:
            break;
        case Delay:
            break;
        case Iterations:
            break;
        case Invert:
            break;
        case CenterStart:
            break;
        case Text:
            break;
        case LedState:
            break;
        default:
            qDebug("Hello default");
            break;
        }
    }
}

//void AnimationOptions::on_applyPushB_clicked()
//{
//    if(ui->axisComB->currentIndex() == 0)
//        m_options.axis = Draw::X_AXIS;
//    else if(ui->axisComB->currentIndex() == 1)
//        m_options.axis = Draw::Y_AXIS;
//    else if(ui->axisComB->currentIndex() == 2)
//        m_options.axis = Draw::Z_AXIS;

//    if(ui->directionComB->currentIndex() == 0)
//        m_options.direction = Draw::BACKWARD;
//    else if(ui->directionComB->currentIndex() == 1)
//        m_options.direction = Draw::FORWARD;

//    if(ui->invertSpinB->value()){
//        m_options.invert = true;
//        m_options.state = Draw::ON;
//    }else{
//        m_options.invert = false;
//        m_options.state = Draw::OFF;
//    }

//    m_options.delay = static_cast<u_int16_t>(ui->delaySpinB->value());
//    m_options.iteration = static_cast<u_int16_t>(ui->iterationsSpinB->value());
//    m_options.leds = static_cast<u_int16_t>(ui->ledsSpinB->value());
//    m_options.speed = static_cast<u_int16_t>(ui->speedSpinB->value());
//    m_options.text = ui->textLineE->text();

//    Q_EMIT optionsReady(m_options);
//}
