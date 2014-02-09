#include "AnimationOptionsGroupBox.hpp"
#include "ui_AnimationOptionsGroupBox.h"
#include <QDebug>

AnimationOptionsGroupBox::AnimationOptionsGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::AnimationOptionsGroupBox)
{
    ui->setupUi(this);
//    setDisabled(true);
}

AnimationOptionsGroupBox::~AnimationOptionsGroupBox()
{
    delete ui;
}

Draw::AnimationOptions *AnimationOptionsGroupBox::getAnimationSettings()
{
    return &animationOptions;
}

//AnimationStruct &AnimationOptionsGroupBox::getAnimationSettings()
//{

//}

void AnimationOptionsGroupBox::enableProperty(const uint8_t &animation)
{
    switch (animation) {
    case 1:

        break;
    default:
        break;
    }
}

void AnimationOptionsGroupBox::changeEvent(QEvent *e)
{
    QGroupBox::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void AnimationOptionsGroupBox::on_applyPushB_clicked()
{
    if(ui->axisComB->currentIndex() == 0)
        animationOptions.axis = Draw::X_AXIS;
    else if(ui->axisComB->currentIndex() == 1)
        animationOptions.axis = Draw::Y_AXIS;
    else if(ui->axisComB->currentIndex() == 2)
        animationOptions.axis = Draw::Z_AXIS;
    animationOptions.delay = static_cast<u_int16_t>(ui->delaySpinB->value());
    if(ui->directionComB->currentIndex() == 0)
        animationOptions.direction = Draw::BACKWARD;
    else if(ui->directionComB->currentIndex() == 1)
        animationOptions.direction = Draw::FORWARD;
    if(ui->invertSpinB->value())
        animationOptions.invert = true;
    else
        animationOptions.invert = false;
    animationOptions.iteration = static_cast<u_int16_t>(ui->iterationsSpinB->value());
    animationOptions.leds = static_cast<u_int16_t>(ui->ledsSpinB->value());
    animationOptions.particle = static_cast<u_int16_t>(ui->fireworkPartSpinB->value());
    animationOptions.speed = static_cast<u_int16_t>(ui->speedSpinB->value());
    animationOptions.text = ui->textLineE->text();
    Q_EMIT optionsReady(&animationOptions);
}
