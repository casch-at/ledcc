#include "AnimationPlayListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>

AnimationPlayListWidget::AnimationPlayListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    connect(this,&QListWidget::itemDoubleClicked,
            this,&AnimationPlayListWidget::on_itemDoubleClicked);
}

void AnimationPlayListWidget::clearList()
{
    clear();
    emit updateUi();
}

void AnimationPlayListWidget::newItem(QList<QListWidgetItem *> item)
{
    foreach (QListWidgetItem *i, item) {
        addItem(i->clone());
    }
    emit updateUi();
}

void AnimationPlayListWidget::on_itemDoubleClicked(QListWidgetItem *item)
{
    Q_EMIT displayAnimationOptions(dynamic_cast<AnimationItem*>(item)->getOptions());
}

void AnimationPlayListWidget::keyPressEvent(QKeyEvent *event)
{
    int cRow = -1;

    switch (event->key()) {
    case Qt::Key_Delete:
        foreach(QListWidgetItem *i,selectedItems())
            delete i;
        if(count())
            setCurrentRow(currentRow());
        emit updateUi();
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
        //    case Qt::Key_Return:
        //        break;
        //    case Qt::Key_Return:
        //        break;
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

void AnimationPlayListWidget::dragMoveEvent(QDragMoveEvent *e)
{
//    QListWidget::dragMoveEvent(e);
    e->acceptProposedAction();
}

void AnimationPlayListWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
}

void AnimationPlayListWidget::dropEvent(QDropEvent *event)
{
    foreach(QListWidgetItem *i,selectedItems())
       insertItem(indexAt(event->pos()).row(),i->clone());
    foreach(QListWidgetItem *i,selectedItems())
        delete i;
    event->acceptProposedAction();
}


void AnimationPlayListWidget::selectAllItems(void)
{
    if(hasFocus())
        selectAll();
}


AnimationItem *AnimationPlayListWidget::getNextAnimation()
{
    static int row;
    if(row >= count())
        row=0;
    return dynamic_cast<AnimationItem*>(item(row++));
}
