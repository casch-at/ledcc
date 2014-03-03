#include "AnimationOptions.hpp"
#include "ui_AnimationOptions.h"
#include <QDebug>

AnimationOptions::AnimationOptions(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::AnimationOptions)
{
    ui->setupUi(this);
}

AnimationOptions::~AnimationOptions()
{
    delete ui;
}

AnimationOptions::Options *AnimationOptions::getAnimationSettings()
{
    return &m_options;
}


void AnimationOptions::displayAnimationOptions(const Options options)
{
//    Options options = options->getOptions();
    if(options.axis == Draw::X_AXIS )
        ui->axisComB->setCurrentIndex(0);
    else if(options.axis == Draw::Y_AXIS )
        ui->axisComB->setCurrentIndex(1);
    else if(options.axis == Draw::Z_AXIS )
        ui->axisComB->setCurrentIndex(0);
    ui->delaySpinB->setValue(static_cast<int>(options.delay));
    if( options.direction == Draw::BACKWARD )
        ui->directionComB->setCurrentIndex(0);
    else if(options.direction == Draw::FORWARD )
        ui->directionComB->setCurrentIndex(1);
    if(options.invert == true && options.state == Draw::ON){
        ui->invertSpinB->setValue(1);
    }else{
        ui->invertSpinB->setValue(0);
    }
    ui->iterationsSpinB->setValue( static_cast<int>(options.iteration));
    ui->ledsSpinB->setValue( static_cast<int>(options.leds));
    ui->speedSpinB->setValue( static_cast<int>(options.speed));
    ui->textLineE->setText(options.text.isEmpty() == true ? "" : options.text);
}

void AnimationOptions::changeEvent(QEvent *e)
{
    QGroupBox::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void AnimationOptions::on_applyPushB_clicked()
{
    if(ui->axisComB->currentIndex() == 0)
        m_options.axis = Draw::X_AXIS;
    else if(ui->axisComB->currentIndex() == 1)
        m_options.axis = Draw::Y_AXIS;
    else if(ui->axisComB->currentIndex() == 2)
        m_options.axis = Draw::Z_AXIS;
    m_options.delay = static_cast<u_int16_t>(ui->delaySpinB->value());
    if(ui->directionComB->currentIndex() == 0)
        m_options.direction = Draw::BACKWARD;
    else if(ui->directionComB->currentIndex() == 1)
        m_options.direction = Draw::FORWARD;
    if(ui->invertSpinB->value()){
        m_options.invert = true;
        m_options.state = Draw::ON;
    }else{
        m_options.invert = false;
        m_options.state = Draw::OFF;
    }
    m_options.iteration = static_cast<u_int16_t>(ui->iterationsSpinB->value());
    m_options.leds = static_cast<u_int16_t>(ui->ledsSpinB->value());
    m_options.speed = static_cast<u_int16_t>(ui->speedSpinB->value());
    m_options.text = ui->textLineE->text();
    Q_EMIT optionsReady(m_options);
}
