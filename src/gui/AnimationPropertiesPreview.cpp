#include "AnimationPropertiesPreview.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QShortcut>

AnimationPropertiesPreview::AnimationPropertiesPreview( QWidget *parent ) :
    QDockWidget(parent),
    mlayout( new QVBoxLayout ),
    mcentralWidget( new QWidget ),
    hideShowSC( new QShortcut(QKeySequence("F9"),parent))
{
    mlayout->setObjectName("animationPropertiesPreviewLay");
    mlayout->setAlignment( this, Qt::AlignCenter );
    this->connect(hideShowSC,&QShortcut::activated,this,&AnimationPropertiesPreview::hideShow);
}

AnimationPropertiesPreview::~AnimationPropertiesPreview()
{
    delete mlayout;
}

void AnimationPropertiesPreview::setProperties( const QString &name, const QString &properties )
{
    QLabel *label = new QLabel( name );
    QFont font;
    bool firstColumn = true;
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
        hlayout->setSpacing(5);

        foreach ( QString textColumn, line.split(" ") )
        {
            if( !textColumn.isEmpty() )
            {                
                label = new QLabel;
                if(firstColumn){
                    textColumn.append(":");
                    label->setAlignment(Qt::AlignRight);
                }
                label->setText(textColumn);
                label->setFont( font );
                hlayout->addWidget( label );
                firstColumn = !firstColumn;
            }
        }

        mlayout->addItem(hlayout);
    }

    mlayout->addStretch(1);
    mcentralWidget->setLayout(mlayout);

    this->setWidget(mcentralWidget);
}
