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

#include "AnimationItem.hpp"
#include "Animation.hpp"
#include "Draw.hpp"
#include "Lift.hpp"
#include "Firework.hpp"
#include "AxisNailWall.hpp"
#include "WireBoxCenterShrinkGrow.hpp"
#include "WireBoxCornerShrinkGrow.hpp"
#include "Loadbar.hpp"
#include "RandomFiller.hpp"
#include "RandomSpark.hpp"
#include "RandomSparkFlash.hpp"
#include "RandomZLift.hpp"
#include "Wall.hpp"
#include "Rain.hpp"
#include "StringFly.hpp"

// Qt Includes
#include <QMessageBox>
#include <QTimer>
#ifdef _DEBUG_
#include <QDebug>
#endif

/*!
 \brief Slot gets called when the user has pressed the edit button from \a AnimationPlayListWidget.
 The first animation options are displayed, and the window will be shown, all other animation options
 will be inserted either in \a optionsNextAnimation or \a optionsPrevAnimation slot function.

 \param itemsList
*/

AnimationOptions::AnimationOptions(QList<AnimationItem*> &itemsList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimationOptions),
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

    void (AnimationOptions:: *slot1)() = &AnimationOptions::compareOldNewAnimationOptions; /* Resolve the overloaded function compareOldNewAnimationOptions */
    connect(ui->m_speedSpinB, &QSpinBox::editingFinished, this, slot1);
    connect(ui->m_delaySpinB, &QSpinBox::editingFinished, this,slot1);
    connect(ui->m_ledsSpinB, &QSpinBox::editingFinished, this, slot1);
    connect(ui->m_particlesSpinB, &QSpinBox::editingFinished, this, slot1);
    connect(ui->m_iterationsSpinB, &QSpinBox::editingFinished, this, slot1);

    /* Resolve functions before creating the connection */
    void (QComboBox:: *signal2)(int) = &QComboBox::currentIndexChanged;
    void (AnimationOptions:: *slot2)(int) = &AnimationOptions::compareOldNewAnimationOptions;
    connect(ui->m_axisComB, signal2,this, slot2);
    connect(ui->m_directionComB, signal2,this, slot2);

    connect(ui->m_invertCheckB, &QCheckBox::stateChanged, this, slot2);
    connect(ui->m_ledStateCheckB, &QCheckBox::stateChanged, this, slot2);

    connect(ui->m_textLineE, &QLineEdit::editingFinished, this, slot1);
    if(itemsList.isEmpty())
        return;
    m_itemList = itemsList;
    optionsNextAnimation();
}

/*!
 \brief Destructor of class

*/
AnimationOptions::~AnimationOptions()
{
    delete ui;
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
    if(isWindowModified()){
        setWindowModified(false);
        Options options;
        options.m_speed = ui->m_speedSpinB->value();
        for (int i = 1; i < TOTAL_ARGUMENTS; i++) {
            switch ( (1 << i) &  m_animationToUpdate->getAvailableAnimationOptions() )
            {
            case Direction:
                options.m_direction = static_cast<Draw::Direction>(ui->m_directionComB->currentIndex());
                break;
            case Axis:
                options.m_axis = static_cast<Draw::Axis>(ui->m_axisComB->currentIndex());
                break;
            case Leds:
                options.m_leds = ui->m_ledsSpinB->value();
                break;
            case Particls:
                options.m_leds = ui->m_particlesSpinB->value();
                break;
            case Delay:
                options.m_delay = ui->m_delaySpinB->value();
                break;
            case Iterations:
                options.m_iteration = ui->m_iterationsSpinB->value();
                break;
            case Invert:
                options.m_invert = ui->m_invertCheckB->isChecked();
                break;
            case CenterStart:
                options.m_invert = ui->m_invertCheckB->isChecked();
                break;
            case Text:
                options.m_text = ui->m_textLineE->text();
                break;
            case LedState:
               options.m_state = static_cast<Draw::BixelState>(ui->m_ledStateCheckB->isChecked());
                break;
            default:
                break;
            }
        }
        m_animationToUpdate->setOptions(options);

        Q_EMIT applyNewAnimationArguments(m_animationToUpdate);
    }
}

/*!
 \brief

*/
void AnimationOptions::cancel()
{
    close();
}

/*!
 \brief

*/
void AnimationOptions::ok()
{
    shouldeApplyNewOptions();
    close();
}

/*!
 \brief

*/
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

/*!
 \brief

*/
void AnimationOptions::optionsNextAnimation()
{
    if( m_animationAt < m_itemList.count() -1)
        m_animationAt++;
    shouldeApplyNewOptions();
    m_animationToUpdate = m_itemList.at(m_animationAt);
    ui->m_propertiesGB->setTitle(tr("Properties of ") + m_animationToUpdate->text());
    hideShowWidgetsDisplayOptions();
}

/*!
 \brief

*/
void AnimationOptions::optionsPrevAnimation()
{
    if(m_animationAt)
        m_animationAt--;
    shouldeApplyNewOptions();
    m_animationToUpdate = m_itemList.at(m_animationAt);
    ui->m_propertiesGB->setTitle(tr("Properties of ") + m_animationToUpdate->text());
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

    for (int i = 0; i < TOTAL_ARGUMENTS; i++) {
        switch ( (1 << i) & m_animationToUpdate->getAvailableAnimationOptions() )
        {
        case Speed:
            ui->m_speedLabel->setEnabled(true);
            ui->m_speedSpinB->setEnabled(true);
            ui->m_speedSpinB->setValue(options->m_speed);
            break;
        case Direction:
            ui->m_directionLabel->setEnabled(true);
            ui->m_directionComB->setEnabled(true);
            ui->m_directionComB->setCurrentIndex(options->m_direction);
            break;
        case Axis:
            ui->m_axisLabel->setEnabled(true);
            ui->m_axisComB->setEnabled(true);
            ui->m_axisComB->setCurrentIndex(options->m_axis);
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
            ui->m_ledStateCheckB->setChecked(options->m_state);
            break;
        default:
            switch ( 1 << i )
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
    Q_EMIT updateUi();
}

/*!
 \brief

*/
void AnimationOptions::shouldeApplyNewOptions()
{
    if (isWindowModified()) {
        if (okToContinue() == QMessageBox::Ok)
            applyAnimationOptions();
        else
            setWindowModified(false);
    }
}

/*!
 \brief

 \return int
*/
int AnimationOptions::okToContinue()
{
    QMessageBox msgb;
    msgb.setText("Options have been modified!");
    msgb.setInformativeText("Do you want apply the new options?");
    msgb.setWindowTitle("Options Modified");
    msgb.setIcon(QMessageBox::Information);
    msgb.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgb.setDefaultButton(QMessageBox::Ok);
    return msgb.exec();
}


/*!
 \brief

 \return u_int8_t
*/
void AnimationOptions::compareOldNewAnimationOptions()
{
    const Options options = *m_animationToUpdate->getOptions();
    int ret = 0;
    ret = options.m_speed != ui->m_speedSpinB->value() ? 1 : 0;
    for (int i = 1; i < TOTAL_ARGUMENTS; i++) {
        switch ( (1 << i) & m_animationToUpdate->getAvailableAnimationOptions() )
        {
        case Direction:
            ret += options.m_direction != ui->m_directionComB->currentIndex() ? 1 : 0;
            break;
        case Axis:
            ret += options.m_axis != ui->m_axisComB->currentIndex() ? 1 : 0;
            break;
        case Leds:
            ret += options.m_leds != ui->m_ledsSpinB->value() ? 1 : 0;
            break;
        case Particls:
            ret += options.m_leds != ui->m_particlesSpinB->value() ? 1 : 0;
            break;
        case Delay:
            ret += options.m_delay != ui->m_delaySpinB->value() ? 1 : 0;
            break;
        case Iterations:
            ret += options.m_iteration != ui->m_iterationsSpinB->value() ? 1 : 0;
            break;
        case Invert:
            ret += options.m_invert != ui->m_invertCheckB->isChecked() ? 1 : 0;
            break;
        case CenterStart:
            ret += options.m_invert != ui->m_invertCheckB->isChecked() ? 1 : 0;
            break;
        case Text:
            ret += options.m_text.compare(ui->m_textLineE->text()) ? 1 : 0;
            break;
        case LedState:
            ret += options.m_state != ui->m_ledStateCheckB->isChecked() ? 1 : 0;
            break;
        default:
            break;
        }
    }
//    qDebug() << "Options Speed: " << options.m_speed << "Current Speed: " <<  ui->m_speedSpinB->value();
//    qDebug() << "Options Delay: " << options.m_delay << "Current Delay: " << ui->m_delaySpinB->value();
//    qDebug() << "Options Iteraions: " << options.m_iteration << "Current Iteraions: " << ui->m_iterationsSpinB->value();
//    qDebug() << "Options Leds: " << options.m_leds << "Current Leds: " << ui->m_ledsSpinB->value();
//    qDebug() << "Options Paritcles: " << options.m_leds << "Current Paritcles: " << ui->m_particlesSpinB->value();
//    qDebug() << "Options Axis: " << options.m_axis << "Current Axis: " << ui->m_axisComB->currentIndex();
//    qDebug() << "Options Direction: " << options.m_direction<< "Current Direction: " << ui->m_directionComB->currentIndex();
//    qDebug() << "Options Invert: " << options.m_invert << "Current Invert: " << ui->m_invertCheckB->isChecked();
//    qDebug() << "Options Text: " << options.m_text << "Current Text: " << ui->m_textLineE->text();
//    qDebug() << "Options Led State:" << options.m_state << "Current Led State: " <<  ui->m_ledStateCheckB->isChecked();
    if(ret) setWindowModified(true);
}

void AnimationOptions::compareOldNewAnimationOptions(int index)
{
    index = index;
    compareOldNewAnimationOptions();
}

