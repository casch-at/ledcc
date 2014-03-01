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


void AnimationOptions::enableProperty(const uint8_t &animation)
{
    switch (animation)
    {
    case 1:

        break;
    default:
        break;
    }
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
    Q_EMIT optionsReady(&m_options);
}
