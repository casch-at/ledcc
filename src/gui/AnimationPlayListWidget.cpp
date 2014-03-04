#include "AnimationPlayListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>

AnimationPlayListWidget::AnimationPlayListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setDropIndicatorShown(true);
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

void AnimationPlayListWidget::dragMoveEvent(QDragMoveEvent *event)
{
//    this->setDropIndicatorShown(true);
    if ( this->dropIndicatorPosition() == QAbstractItemView::AboveItem )
    {
        event->ignore();
        return;
    }

    event->accept();
}
//void AnimationPlayListWidget::dragMoveEvent(QDragMoveEvent *e)
//{
////    QListWidget::dragMoveEvent(e);
//    e->acceptProposedAction();
//}

void AnimationPlayListWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
}

void AnimationPlayListWidget::dropEvent(QDropEvent *event)
{
    foreach(QListWidgetItem *i,selectedItems()){
        insertItem(indexAt(event->pos()).row()+1,i->clone());
    }
//    foreach(QListWidgetItem *i,selectedItems())
//        delete i;
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
    int rows = count();
    if(row >= count())
        row=0;
    if(rows)
        return dynamic_cast<AnimationItem*>(item(row++));
    else
        return Q_NULLPTR;
}
