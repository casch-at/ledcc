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
#include "AnimationPropertiesPreview.hpp"
#include "ui_AnimationPropertiesPreview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QShortcut>

//FIXME:: Properties which are shown are always the last stored values from the animation.
//        Use the animation item struct properties to generate the preview
//FIXME:: Application crashes when last item is removed from the list because it tries to show the preview
AnimationPropertiesPreview::AnimationPropertiesPreview( QWidget *parent ) :
    QDockWidget(parent),
    ui(new Ui::animationPropertiesPreview),
    hideShowSC( new QShortcut(QKeySequence("F9"),parent)),
    frame(new QFrame)
{
    ui->setupUi(this);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    this->connect(hideShowSC,&QShortcut::activated,this,&AnimationPropertiesPreview::hideShow);
    this->setMaximumWidth(190);
    this->setMinimumWidth(100);
}

AnimationPropertiesPreview::~AnimationPropertiesPreview()
{
    if(!frame->children().isEmpty()){
        delete mlayout;
    }
    delete frame;
    delete ui;
}

void AnimationPropertiesPreview::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AnimationPropertiesPreview::clearFrame( void )
{
    foreach (QObject *o, frame->children()) {
        delete o;
    }
}


void AnimationPropertiesPreview::createPropertiePreview(QStringList &properties )
{
    if(!frame->children().isEmpty())
        clearFrame();

    bool firstColumn = true;
    QFont font;
    font.setBold(true);
    font.setPixelSize(PIXEL_SIZE_NAME);

    mlayout =  new QVBoxLayout;
    mlayout->setObjectName("animationPropertiesPreviewLay");
    mlayout->setAlignment( this, Qt::AlignCenter );

    QLabel *label = new QLabel( properties.first() );
    properties.removeAt(0);
    label->setWordWrap(true);
    label->setFont(font);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QFrame *f = new QFrame;
    f->setFrameStyle(QFrame::HLine | QFrame::Raised);
    f->setMidLineWidth(1);
    f->setLineWidth(0);

    mlayout->addWidget(label);
    mlayout->addWidget(f);

    font.setBold(false);
    font.setPixelSize(PIXEL_SIZE_PROPERTIES);

    foreach ( QString propertie, properties )
    {
        if(propertie.isEmpty())
            continue;
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setSpacing(10);

        foreach ( QString propertieValue, propertie.split(":") )
        {
            if( propertieValue.isEmpty() )
                continue;
            label = new QLabel;
            label->setWordWrap(true);
            /**
 * Property name is first column and is Right aligned,
 * second column is property value default aligned
 */
            if(firstColumn){
                propertieValue.append(":");
                label->setAlignment(Qt::AlignRight);
            }
            label->setText(propertieValue);
            label->setFont( font );
            hlayout->addWidget( label );
            firstColumn = !firstColumn;
        }
        mlayout->addItem(hlayout);
    }

    mlayout->addStretch(1);
    frame->setLayout(mlayout);
    this->setWidget(frame);
}
