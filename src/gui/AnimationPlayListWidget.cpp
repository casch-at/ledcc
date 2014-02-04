#include "AnimationPlayListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>


AnimationPlayListWidget::AnimationPlayListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void AnimationPlayListWidget::clearList()
{
    clear();
    emit updateUi();
}

void AnimationPlayListWidget::newItem(QListWidgetItem *item)
{
    qDebug() << item->text();
    addItem(item->text());
    emit updateUi();
    //    addItem("Hello");
}

void AnimationPlayListWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Event key is: " << QString::number(event->key(),16)
             << "Qt::Key_Delete is: " << QString::number(Qt::Key_Delete,16);
    int currentRow = -1;
    switch (event->key()) {
    case Qt::Key_Delete:
        delete currentItem();
        emit updateUi();
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
        qDebug()<< "Rows: " << count();
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
