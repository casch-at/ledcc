#include "AnimationListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>

AnimationListWidget::AnimationListWidget(QWidget *parent):
    QListWidget(parent)
{

}

void AnimationListWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Event key is: " << QString::number(event->key(),16)
             << "Qt::Key_Enter is: " << QString::number(Qt::Key_Enter,16);
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
//        delete currentItem();
        emit itemDoubleClicked(currentItem());
    }
}
