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
    connect(ui->m_nextPB, &QAction::triggered, this, &AnimationOptions::optionsNextAnimation);
    connect(ui->m_prevPB, &QAction::triggered, this, &AnimationOptions::optionsPrevAnimation);
    connect(ui->m_applyPB, &QAction::triggered, this, &AnimationOptions::applyAnimationOptions);
    connect(ui->m_cancelPB, &QAction::triggered, this, &AnimationOptions::cancel);
    connect(ui->m_okPB, &QAction::triggered, this, &AnimationOptions::ok);
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

void AnimationOptions::onApplyPressed()
{
    if(m_animationOptionsModefied)
        Q_EMIT applyNewAnimationArguments();
}

void AnimationOptions::cancel()
{

}

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

void AnimationOptions::optionsNextAnimation()
{
    AnimationItem *animation;
    if (m_itemList.count() == m_animationAt )
        ui->m_nextPB->setDisabled(true);

    animation = m_itemList.at(m_animationAt);

    hideShowWidgetsDisplayOptions(animation->getAvailableAnimationOptions(), animation->getOptions());


    m_animationAt++;
}

void AnimationOptions::optionsPrevAnimation()
{
    m_animationAt--;
}

void AnimationOptions::hideShowWidgetsDisplayOptions(const int &hasOption, const Options *options)
{
    if ( hasOption & Speed) {
        ui->m_speedLabel->setVisible(!ui->m_speedLabel->isVisible());
        ui->m_speedSpinB->setVisible(!ui->m_speedSpinB->isVisible());
        ui->m_speedSpinB->setValue(options->speed);
    }

    if ( hasOption & Direction){

    }

    if ( hasOption & Axis) {

    }

    if ( hasOption & Leds) {

    }

    if ( hasOption & Particls) {

    }

    if ( hasOption & Delay) {

    }

    if ( hasOption & Iterations) {

    }

    if ( hasOption & Invert) {

    }

    if ( hasOption & CenterStart) {

    }

    if ( hasOption & Text) {

    }

    if ( hasOption & LedState) {

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
