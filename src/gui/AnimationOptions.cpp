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

AnimationOptions::AnimationOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimationOptions),
    m_animationOptionsModefied(false),
    m_animationAt(-1)
{
    // Setup the user interface
    ui->setupUi(this);
    // Create connections ( Signal & Slot )
    connect(ui->m_nextPB, &QPushButton::pressed, this, &AnimationOptions::optionsNextAnimation);
    connect(ui->m_prevPB, &QPushButton::pressed, this, &AnimationOptions::optionsPrevAnimation);
    connect(ui->m_applyPB, &QPushButton::pressed, this, &AnimationOptions::applyAnimationOptions);
    connect(ui->m_cancelPB, &QPushButton::pressed, this, &AnimationOptions::cancel);
    connect(ui->m_okPB, &QPushButton::pressed, this, &AnimationOptions::ok);
    connect(this, &QDialog::rejected, this, &AnimationOptions::cancel);
//    connect(ui->m_axisComB, &QComboBox::currentIndexChanged, this, &AnimationOptions::setWindowModified);
    setWindowModified(true);
//    connect(this, &QDialog::done, this, &AnimationOptions::cancel);
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
    // TODO:: If current animation has been changed ask if the settings should be applyed befor continue.
    if(isWindowModified())
        qDebug("WindowModefied");
    if( m_animationAt < m_itemList.count() -1)
        m_animationAt++;


    animation = m_itemList.at(m_animationAt);

    ui->groupBox->setTitle("Properties of " + animation->text());
    hideShowWidgetsDisplayOptions(animation->getAvailableAnimationOptions(), animation->getOptions());
}


/*!
 \brief

*/
void AnimationOptions::optionsPrevAnimation()
{
    AnimationItem *animation;

    // TODO:: If current animation has been changed ask if the settings should be applyed befor continue.
    if(m_animationAt)
        m_animationAt--;
    animation = m_itemList.at(m_animationAt);

    ui->groupBox->setTitle("Properties of " + animation->text());
    hideShowWidgetsDisplayOptions(animation->getAvailableAnimationOptions(), animation->getOptions());
}



/*!
 \brief

 \param hasOption
 \param options
*/
void AnimationOptions::hideShowWidgetsDisplayOptions(const int &hasOption, const Options *options)
{
    for (int i = 0; i < TOTAL_ARGUMENTS; i++) {
        switch ( (1<<i) & hasOption )
        {
        case Speed:
            ui->m_speedLabel->setVisible(true);
            ui->m_speedSpinB->setVisible(true);
            ui->m_speedSpinB->setValue(options->speed);
            break;
        case Direction:
            ui->m_directionLabel->setVisible(true);
            ui->m_directionComB->setVisible(true);
            ui->m_directionComB->setCurrentIndex(options->direction == Draw::Backward ? 0 : 1);
            break;
        case Axis:
            ui->m_axisLabel->setVisible(true);
            ui->m_axisComB->setVisible(true);
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
            ui->m_ledsLabel->setVisible(true);
            ui->m_ledsSpinB->setVisible(true);
            ui->m_ledsSpinB->setValue(options->leds);
            break;
        case Particls:
            ui->m_particlesLabel->setVisible(true);
            ui->m_particlesSpinB->setVisible(true);
            ui->m_particlesSpinB->setValue(options->leds);
            break;
        case Delay:
            ui->m_delayLabel->setVisible(true);
            ui->m_delaySpinB->setVisible(true);
            ui->m_delaySpinB->setValue(options->delay);
            break;
        case Iterations:
            ui->m_iterationsLabel->setVisible(true);
            ui->m_iterationsSpinB->setVisible(true);
            ui->m_iterationsSpinB->setValue(options->iteration);
            break;
        case Invert:
            ui->m_invertCheckB->setVisible(true);
            ui->m_invertCheckB->setChecked(options->invert);
            break;
        case CenterStart:
            ui->m_invertCheckB->setVisible(true);
            ui->m_invertCheckB->setChecked(options->invert);
            break;
        case Text:
            ui->m_textLabel->setVisible(true);
            ui->m_textLineE->setVisible(true);
            ui->m_textLineE->setText(options->text);
            break;
        case LedState:
            ui->m_ledStateCheckB->setVisible(true);
            ui->m_ledStateCheckB->setChecked(options->state == Draw::ON ? true : false);
            break;
        default:
            qDebug("Default Default 1");
            switch ((1<<i))
            {
            case Speed:
                ui->m_speedLabel->setVisible(false);
                ui->m_speedSpinB->setVisible(false);
                break;
            case Direction:
                ui->m_directionLabel->setVisible(false);
                ui->m_directionComB->setVisible(false);
                break;
            case Axis:
                ui->m_axisLabel->setVisible(false);
                ui->m_axisComB->setVisible(false);
                break;
            case Leds:
                ui->m_ledsLabel->setVisible(false);
                ui->m_ledsSpinB->setVisible(false);
                break;
            case Particls:
                ui->m_particlesLabel->setVisible(false);
                ui->m_particlesSpinB->setVisible(false);
                break;
            case Delay:
                ui->m_delayLabel->setVisible(false);
                ui->m_delaySpinB->setVisible(false);
                break;
            case Iterations:
                ui->m_iterationsLabel->setVisible(false);
                ui->m_iterationsSpinB->setVisible(false);
                break;
            case Invert:
                ui->m_invertCheckB->setVisible(false);
                break;
            case CenterStart:
                ui->m_invertCheckB->setVisible(false);
                break;
            case Text:
                ui->m_textLabel->setVisible(false);
                ui->m_textLineE->setVisible(false);
                break;
            case LedState:
                ui->m_ledStateCheckB->setVisible(false);
                break;
            default:
                qDebug("Default Default 2");
                break;
            }
            break;
        }
    }

    for (int var = 0; var < 10000; var++) {

    }

    resize(350,0);
    ui->groupBox->resize(0,0);

    updateGeometry();
    update();
    ui->groupBox->repaint();
    ui->groupBox->updateGeometry();
    Q_EMIT updateUi();
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
