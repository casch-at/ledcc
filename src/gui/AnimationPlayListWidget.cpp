#include "AnimationPlayListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>

AnimationPlayListWidget::AnimationPlayListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setDropIndicatorShown(true);
    setMovement(QListView::Free);

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
    qDebug("Playlist");

    event->accept();
}

void AnimationPlayListWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
}

void AnimationPlayListWidget::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
    QModelIndex index = indexAt(event->pos());
//    DropIndicatorPosition dropPos = dropIndicatorPosition();
    QList<int> altrows;
    if(event->source() == this){
//        insertItem(index.row(),sele);
        foreach(QListWidgetItem *i,selectedItems()){
            altrows.append(indexFromItem(i).row());
        }
        int row = 0;
        foreach (QListWidgetItem *i, selectedItems()) {
            insertItem(index.row()+1,i->clone());
            qDebug() << "Item text:" << item(altrows.at(row))->text() << "Row:" << altrows.at(row);
            if(item(altrows.at(row))->text().isEmpty()){

                delete item(altrows.at(row++));
            }
        }
        qDebug() << "Row:" << indexFromItem(item(2)).row() << "Item" << item(2)->text() ;
    }else{

        qDebug() << "Source event:" << event->source();
        event->accept();
    }
    qDebug("Playlist drop");

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
