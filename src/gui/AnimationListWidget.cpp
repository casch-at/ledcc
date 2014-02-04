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
    int currentRow = -1;
    switch (event->key()) {
    case Qt::Key_Enter:
        emit itemDoubleClicked(currentItem());
        break;
    case Qt::Key_Return:
        emit itemDoubleClicked(currentItem());
        break;
    case Qt::Key_Up:
        currentRow = currentRow();
        if(currentRow == 0)
            setCurrentRow(count()-1);
        else
            setCurrentRow(currentRow-1);
        break;
    case Qt::Key_Down:
        currentRow = currentRow();
        if(currentRow == count()-1)
            setCurrentRow(0);
        else
            setCurrentRow(currentRow+1);
        break;
//    case Qt::Key_Return:
//        break;
//    case Qt::Key_Return:
        break;
    default:
        break;
    }
}
