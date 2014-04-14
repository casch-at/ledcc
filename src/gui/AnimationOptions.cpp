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

#include <QTimer>
// Qt Includes
#ifdef _DEBUG_
#include <QDebug>
#endif
using namespace animations;

AnimationOptions::AnimationOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimationOptions),
    m_animationOptionsModefied(false),
    m_animationAt(-1)
{
    // Setup the user interface
    ui->setupUi(this);
//    m_updateUi = new QTimer(this); /* If widgets are set to visable false we need a timer to call all slot where the Parent gets resized/
//    m_updateUi->setSingleShot(true);
//    m_updateUi->setInterval(1);
    // Create connections ( Signal & Slot )
    connect(ui->m_nextPB, &QPushButton::pressed, this, &AnimationOptions::optionsNextAnimation);
    connect(ui->m_prevPB, &QPushButton::pressed, this, &AnimationOptions::optionsPrevAnimation);
    connect(ui->m_applyPB, &QPushButton::pressed, this, &AnimationOptions::applyAnimationOptions);
    connect(ui->m_cancelPB, &QPushButton::pressed, this, &AnimationOptions::cancel);
    connect(ui->m_okPB, &QPushButton::pressed, this, &AnimationOptions::ok);
    connect(this, &QDialog::rejected, this, &AnimationOptions::cancel);
//    connect(m_updateUi, &QTimer::timeout, this, &AnimationOptions::resizeUi);
    setWindowModified(true);
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
    if(compareOldNewAnimationOptions())
        Q_EMIT applyNewAnimationArguments(m_animationToUpdate);
}

/*!
 \brief

*/
void AnimationOptions::cancel()
{
    hide();
    foreach (AnimationItem *item, m_itemList) {
        delete item;
    }
    m_animationAt = -1;
}

/*!
 \brief

*/
void AnimationOptions::ok()
{
    // If current animation has ben modefied ask user if he wants to apply the settings
    // If nothing has changed hide application
}

void AnimationOptions::updateUi()
{
    if (m_animationAt){
        ui->m_prevPB->setEnabled(true);
    } else {
        ui->m_prevPB->setDisabled(true);
    }
    if(m_animationAt >= m_itemList.count() - 1 )
        ui->m_nextPB->setDisabled(true);
    else
        ui->m_nextPB->setEnabled(true);

}

//void AnimationOptions::resizeUi()
//{
//    resize(350,0);
//}

/*!
 \brief

*/
void AnimationOptions::optionsNextAnimation()
{
    if( m_animationAt < m_itemList.count() -1)
        m_animationAt++;
    m_animationToUpdate = m_itemList.at(m_animationAt);
    ui->m_propertiesGB->setTitle("Properties of " + m_animationToUpdate->text());
    hideShowWidgetsDisplayOptions();
}

/*!
 \brief

*/
void AnimationOptions::optionsPrevAnimation()
{
    if(m_animationAt)
        m_animationAt--;
    m_animationToUpdate = m_itemList.at(m_animationAt);
    ui->m_propertiesGB->setTitle("Properties of " + m_animationToUpdate->text());
    hideShowWidgetsDisplayOptions();
}

/*!
 \brief

 \param hasOption
 \param options
*/
void AnimationOptions::hideShowWidgetsDisplayOptions()
{
    const Options *options = m_animationToUpdate->getOptions();
    const int hasOption = m_animationToUpdate->getAvailableAnimationOptions();
    using namespace animations;
    for (int i = 0; i < TOTAL_ARGUMENTS; i++) {
        switch ( (1<<i) & hasOption )
        {
        case Speed:
            ui->m_speedLabel->setEnabled(true);
            ui->m_speedSpinB->setEnabled(true);
            ui->m_speedSpinB->setValue(options->m_speed);
            break;
        case Direction:
            ui->m_directionLabel->setEnabled(true);
            ui->m_directionComB->setEnabled(true);
            ui->m_directionComB->setCurrentIndex(options->m_direction == Draw::Backward ? 0 : 1);
            break;
        case Axis:
            ui->m_axisLabel->setEnabled(true);
            ui->m_axisComB->setEnabled(true);
            switch (options->m_axis)
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
            ui->m_ledsLabel->setEnabled(true);
            ui->m_ledsSpinB->setEnabled(true);
            ui->m_ledsSpinB->setValue(options->m_leds);
            break;
        case Particls:
            ui->m_particlesLabel->setEnabled(true);
            ui->m_particlesSpinB->setEnabled(true);
            ui->m_particlesSpinB->setValue(options->m_leds);
            break;
        case Delay:
            ui->m_delayLabel->setEnabled(true);
            ui->m_delaySpinB->setEnabled(true);
            ui->m_delaySpinB->setValue(options->m_delay);
            break;
        case Iterations:
            ui->m_iterationsLabel->setEnabled(true);
            ui->m_iterationsSpinB->setEnabled(true);
            ui->m_iterationsSpinB->setValue(options->m_iteration);
            break;
        case Invert:
            ui->m_invertCheckB->setEnabled(true);
            ui->m_invertCheckB->setChecked(options->m_invert);
            break;
        case CenterStart:
            ui->m_invertCheckB->setEnabled(true);
            ui->m_invertCheckB->setChecked(options->m_invert);
            break;
        case Text:
            ui->m_textLabel->setEnabled(true);
            ui->m_textLineE->setEnabled(true);
            ui->m_textLineE->setText(options->m_text);
            break;
        case LedState:
            ui->m_ledStateCheckB->setEnabled(true);
            ui->m_ledStateCheckB->setChecked(options->m_state == Draw::ON ? true : false);
            break;
        default:
            switch ((1<<i))
            {
            case Speed:
                ui->m_speedLabel->setEnabled(false);
                ui->m_speedSpinB->setEnabled(false);
                break;
            case Direction:
                ui->m_directionLabel->setEnabled(false);
                ui->m_directionComB->setEnabled(false);
                break;
            case Axis:
                ui->m_axisLabel->setEnabled(false);
                ui->m_axisComB->setEnabled(false);
                break;
            case Leds:
                ui->m_ledsLabel->setEnabled(false);
                ui->m_ledsSpinB->setEnabled(false);
                break;
            case Particls:
                ui->m_particlesLabel->setEnabled(false);
                ui->m_particlesSpinB->setEnabled(false);
                break;
            case Delay:
                ui->m_delayLabel->setEnabled(false);
                ui->m_delaySpinB->setEnabled(false);
                break;
            case Iterations:
                ui->m_iterationsLabel->setEnabled(false);
                ui->m_iterationsSpinB->setEnabled(false);
                break;
            case Invert:
                ui->m_invertCheckB->setEnabled(false);
                break;
            case CenterStart:
                ui->m_invertCheckB->setEnabled(false);
                break;
            case Text:
                ui->m_textLabel->setEnabled(false);
                ui->m_textLineE->setEnabled(false);
                break;
            case LedState:
                ui->m_ledStateCheckB->setEnabled(false);
                break;
            default: /* Should never come here */
                break;
            }
            break;
        }
    }
//    m_updateUi->start();

    Q_EMIT updateUi();
}


// TODO:: Create class for the Options better for intial values and comparies,
// in these case the value of the options are undefined and therfor the behavior will be undefined
u_int8_t AnimationOptions::compareOldNewAnimationOptions()
{
    const Options *options = m_animationToUpdate->getOptions();
//    if(options->axis != ui->m_axisComB)
//        return 1;
//    else if(options->delay != ui->)
}

