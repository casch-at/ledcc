#include "AnimationOptionsGroupBox.hpp"
#include "ui_AnimationOptionsGroupBox.h"
#include <QDebug>
#include <Global.hpp>
AnimationOptionsGroupBox::AnimationOptionsGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::AnimationOptionsGroupBox)
{
    ui->setupUi(this);
    qDebug() << "Current index of ceckbox: " << ui->achsesCB->currentIndex();
    setDisabled(true);
}

AnimationOptionsGroupBox::~AnimationOptionsGroupBox()
{
    delete ui;
}

AnimationStruct AnimationOptionsGroupBox::getAnimationSettings()
{
    if(ui->achsesCB->currentIndex() == 0)
        animationStruct.axis = X_AXIS;
    else if(ui->achsesCB->currentIndex() == 1)
        animationStruct.axis = Y_AXIS;
    else if(ui->achsesCB->currentIndex() == 2)
        animationStruct.axis = Z_AXIS;
    animationStruct.delay = static_cast<uint16_t>(ui->delaySB->value());
    if(ui->directionCB->currentIndex() == 0)
        animationStruct.direction = BACKWARD;
    else if(ui->directionCB->currentIndex() == 1)
        animationStruct.direction = FORWARD;
    animationStruct.
    return animationStruct;
}

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


