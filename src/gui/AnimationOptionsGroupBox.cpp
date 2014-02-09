#include "AnimationOptionsGroupBox.hpp"
#include "ui_AnimationOptionsGroupBox.h"
#include <QDebug>

AnimationOptionsGroupBox::AnimationOptionsGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::AnimationOptionsGroupBox)
{
    ui->setupUi(this);
    setDisabled(true);
    foreach (const QObject *o, children()) {
        qDebug() << o->objectName();
    }
}

AnimationOptionsGroupBox::~AnimationOptionsGroupBox()
{
    delete ui;
}

//AnimationStruct &AnimationOptionsGroupBox::getAnimationSettings()
//{
//    if(ui->axisComB->currentIndex() == 0)
//        animationStruct.axis = X_AXIS;
//    else if(ui->axisComB->currentIndex() == 1)
//        animationStruct.axis = Y_AXIS;
//    else if(ui->axisComB->currentIndex() == 2)
//        animationStruct.axis = Z_AXIS;
//    animationStruct.delay = static_cast<uint16_t>(ui->delaySpinB->value());
//    if(ui->directionComB->currentIndex() == 0)
//        animationStruct.direction = BACKWARD;
//    else if(ui->directionComB->currentIndex() == 1)
//        animationStruct.direction = FORWARD;
////    animationStruct.
//    return animationStruct;
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


