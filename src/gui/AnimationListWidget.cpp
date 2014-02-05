#include "AnimationListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>

AnimationListWidget::AnimationListWidget(QWidget *parent):
    QListWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void AnimationListWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Event key is: " << QString::number(event->key(),16)
             << "Qt::Key_Enter is: " << QString::number(Qt::Key_Enter,16);
    int cRow = -1;

    switch (event->key()) {
    case Qt::Key_Enter:
        emit itemsSelected(selectedItems());
        break;
    case Qt::Key_Return:
        emit itemsSelected(selectedItems());
        break;
    case Qt::Key_Up:
        cRow = currentRow();
        if(cRow == 0)
            setCurrentRow(count()-1);
        else
            setCurrentRow(cRow-1);
        break;
    case Qt::Key_Down:
        cRow = currentRow();
        if(cRow == count()-1)
            setCurrentRow(0);
        else
            setCurrentRow(cRow+1);
        break;
    case Qt::Key_Escape:
        for(int i=0;i < count();i++){
            if(item(i)->isSelected())
                setItemSelected(item(i),false);
        }
        break;
    default:
        break;
    }
}

void AnimationListWidget::selectAllItems()
{
    if(hasFocus())
        selectAll();
}

QHash<QString, AnimationStruct> *AnimationListWidget::getAnimationList()
{
    return &alist;
}

void AnimationListWidget::setAnimationList(QHash<QString, AnimationStruct> &animations)
{
    alist = animations;
}


