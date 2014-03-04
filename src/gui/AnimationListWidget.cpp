#include "AnimationListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>
//#include "animations/Lift.hpp"
//#include <QtWidgets/QAction>

AnimationListWidget::AnimationListWidget(QWidget *parent):
    QListWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setDragDropMode(QAbstractItemView::DragOnly);
}

void AnimationListWidget::keyPressEvent(QKeyEvent *event)
{
    //    qDebug() << "Event key is: " << QString::number(event->key(),16)
    //             << "Qt::Key_Enter is: " << QString::number(Qt::Key_Enter,16);
    int cRow = -1;

    switch (event->key()) {
    case Qt::Key_Enter:
        Q_EMIT itemsSelected(selectedItems());
        break;
    case Qt::Key_Return:
        Q_EMIT itemsSelected(selectedItems());
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
            setItemSelected(item(i),false);
        }
        break;
    case Qt::Key_Space:
        item(currentIndex().row())->setSelected(true);
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

void AnimationListWidget::insertAnimation(const QString &animation)
{
    addItem(animation);
}


