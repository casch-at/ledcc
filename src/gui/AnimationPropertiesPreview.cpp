#include "AnimationPropertiesPreview.hpp"
#include "QVBoxLayout"
#include "QHBoxLayout"

AnimationPropertiesPreview::AnimationPropertiesPreview( QWidget *parent ) :
    QDockWidget(parent),
    mlayout( new QVBoxLayout ),
    mcentralWidget( new QWidget )
{
    mlayout->setObjectName("animationPropertiesPreviewLay");
    mlayout->setAlignment( this, Qt::AlignCenter );
}

AnimationPropertiesPreview::~AnimationPropertiesPreview()
{
    delete mlayout;
}

void AnimationPropertiesPreview::setProperties( const QString &name, const QString &properties )
{
    QLabel *label = new QLabel( name );
    QFont font;

    font.setBold(true);
    font.setPixelSize(16);

    label->setFont(font);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    mlayout->addWidget(label);

    font.setBold(false);

    font.setPixelSize(11);
    foreach ( QString line, properties.split(":") )
    {
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setSpacing(40);

        foreach ( QString column, line.split(" ") )
        {
            if( !column.isEmpty() )
            {
                label = new QLabel( column );
                label->setFont( font );
                hlayout->addWidget( label );
            }
        }

        mlayout->addItem(hlayout);
    }

    mlayout->addStretch(1);
    mcentralWidget->setLayout(mlayout);

    this->setWidget(mcentralWidget);
}
