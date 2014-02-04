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
    if(event->key() == Qt::Key_Delete)
    {
        delete currentItem();
        emit updateUi();
    }

}
