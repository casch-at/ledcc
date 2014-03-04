#include "AnimationListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>
//#include "animations/Lift.hpp"
//#include <QtWidgets/QAction>

AnimationListWidget::AnimationListWidget(QWidget *parent):
    QListWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMovement(QListView::Free);
    setDragDropMode(QAbstractItemView::DragDrop);
}

void AnimationListWidget::keyPressEvent(QKeyEvent *event)
{
    int cRow = -1;
//    QListWidgetItem *listItem;
    //TODO::Fix space selection!!!
    //      Actuall behaviour, item stays always selected ones item is selected because
    //      when moving item item gets selected ;-) Strange behaviour
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
//        listItem = item(currentIndex().row());
//        setItemSelected(listItem,!listItem->isSelected());
//        if(listItem->isSelected())
//            setItemSelected(listItem,true);
//        else
//            setItemSelected(listItem,false);
        break;
    default:
        break;
    }
}

void AnimationListWidget::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->source() != this) {
        e->accept();
    } else {
        e->ignore();
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


