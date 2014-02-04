#include "AnimationOptionsGroupBox.hpp"
#include "ui_AnimationOptionsGroupBox.h"

AnimationOptionsGroupBox::AnimationOptionsGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::AnimationOptionsGroupBox)
{
    ui->setupUi(this);
    setDisabled(true);
}

AnimationOptionsGroupBox::~AnimationOptionsGroupBox()
{
    delete ui;
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
