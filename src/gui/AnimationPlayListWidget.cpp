
/*
 * Copyright (C) 2014  Christian Schwarzgruber <christiandev9@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "AnimationPlayListWidget.hpp"
#include <QScrollBar>
#include <QTimer>
#include <QKeyEvent>
#ifdef _DEBUG_
#include <QDebug>
#endif


/*!
 \brief AnimationPlayListWidget Constructor

 \todo Create custrom context menu

 \param[in,out] parent  Parent QWidget past over to \link ListWidget()
*/
AnimationPlayListWidget::AnimationPlayListWidget(QWidget *parent) :
    ListWidget(parent),
    m_lastPlayedAnimation(0),
    m_scrollThrough(0)
{
    setDropIndicatorShown(true);
    setMovement(Free);
    setDragDropMode(DragDrop);
    setDefaultDropAction(Qt::MoveAction);
//    setDragDropOverwriteMode(true);
    setAcceptDrops(true);

    // Create connection
    connect( this, &QListWidget::itemDoubleClicked, this, &AnimationPlayListWidget::on_itemDoubleClicked);
}


/*!
 \brief Virtual destructor

*/
AnimationPlayListWidget::~AnimationPlayListWidget()
{
}


/*!
 \brief Function to clear the QListWidget.

*/
void AnimationPlayListWidget::clearList()
{
    clear();
    emit updateUi();
}


/*!
 \brief Insert new items

 Function insert one or more new QListWidgetItems into the QListWidget

 \param[in] item The item(s) to insert
*/
void AnimationPlayListWidget::newItem(QList<QListWidgetItem *> item)
{
    foreach (QListWidgetItem *i, item) {
        addItem(i->clone());
    }
    emit updateUi();
}


/*!
 \brief Function gets called when item is double clicked and di

 \param item
*/
void AnimationPlayListWidget::on_itemDoubleClicked(QListWidgetItem *item)
{
    Q_EMIT displayAnimationOptions(dynamic_cast<AnimationItem*>(item)->getOptions());
}


void AnimationPlayListWidget::valueChanged()
{
    qDebug() << verticalScrollBar()->value();
}

bool AnimationPlayListWidget::dropOn(QDropEvent *event, int *dropRow, int *dropCol, QModelIndex *dropIndex)
{
    if (event->isAccepted())
        return false;

    QModelIndex index;
    //     rootIndex() (i.e. the viewport) might be a valid index
    if (viewport()->rect().contains(event->pos())) {
        index = indexAt(event->pos());
        if (!index.isValid() || visualRect(index).contains(event->pos()))
            index = rootIndex();
    }


    // If we are allowed to do the drop
    if (model()->supportedDropActions() & event->dropAction()) {
        int row = -1;
        int col = -1;
        //        if (index != root) {
        qDebug() << dropIndicatorPosition();
        switch (dropIndicatorPosition()) {
        case QAbstractItemView::AboveItem:
            row = index.row();
            col = index.column();
            qDebug() << "Parent index:" << index.parent();
            index = index.parent();
            break;
        case QAbstractItemView::BelowItem:
            row = index.row() + 1;
            col = index.column();
            index = index.parent();
            break;
        case QAbstractItemView::OnItem:
        case QAbstractItemView::OnViewport:
            break;
        }
        *dropIndex = index;
        *dropRow = row;
        *dropCol = col;
        return true;
        //        if (!droppingOnItself(event, index))
        //            return true;
    }
    return false;
}


/*!
 \brief Reimplement the keyPressEvent function to add some more functionality

 \param e
*/
void AnimationPlayListWidget::keyPressEvent(QKeyEvent *e)
{
    QModelIndex index;

    switch (e->key()) {
    case Qt::Key_Delete:
        foreach(QListWidgetItem *i,selectedItems())
            delete i;

        if(count())
            setCurrentRow(currentRow());

        emit updateUi();
        break;
    case Qt::Key_Up:
        index = moveCursor( QAbstractItemView::MoveUp, e->modifiers() );

        if( index.row() > 0)
            m_scrollThrough = 0;

        if ( index.row()  == 0 ){
            m_scrollThrough++;

            switch (m_scrollThrough)
            {
            case 1:
                setCurrentRow( 0, selectionCommand(index,dynamic_cast<QEvent*>(e)) );
                break;
            case 2:
                setCurrentRow( count() - 1, selectionCommand(index,dynamic_cast<QEvent*>(e)) );
                m_scrollThrough = 0;
                break;
            default:
                break;
            }
        }
        else{
            setCurrentRow( index.row(), selectionCommand(index,dynamic_cast<QEvent*>(e)) );
        }
        break;
    case Qt::Key_Down:
        index = moveCursor( QAbstractItemView::MoveDown, e->modifiers() );
        if (index.row() + 1 < count())
            m_scrollThrough = 0;

        if( index.row() + 1 == count() ){
            m_scrollThrough++;

            switch (m_scrollThrough)
            {
            case 1:
                setCurrentRow( count() -1, selectionCommand(index,dynamic_cast<QEvent*>(e)) );
                break;
            case 2:
                setCurrentRow( 0, selectionCommand(index,dynamic_cast<QEvent*>(e)) );
                m_scrollThrough = 0;
                break;
            default:
                break;
            }
        }
        else{
            setCurrentRow( index.row(), selectionCommand(index,dynamic_cast<QEvent*>(e)) );
        }

        break;
    case Qt::Key_Return:
        break;
    case Qt::Key_Escape:
        for(int i=0;i < count();i++){
            setItemSelected(item(i),false);
        }
        break;
    default:
        e->ignore();
        return;
        break;
    }
    e->accept();
}

void AnimationPlayListWidget::dragMoveEvent(QDragMoveEvent *event)
{

    ListWidget::dragMoveEvent(event);
}

void AnimationPlayListWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    ListWidget::dragLeaveEvent(e);
}


/*!
 \brief Reimplemented virtual proteced \a dropEvent function.
 Reimplemented dropEvent function to add some more functionality.
 \param[in] e \link QDropEvent
*/
void AnimationPlayListWidget::dropEvent(QDropEvent *e)
{
    QList<QListWidgetItem*> items;

    if ( e->source() == this  )
    {
        QModelIndex index = indexAt(e->pos());

        if (index.isValid())
        {
            foreach (QListWidgetItem *i, selectedItems())
            {
                items.append( i->clone() );
                delete i;
            }
            int row = -1;
            int col = -1;
            dropOn(e,&row,&col,&index);
            row = index.row();
            //            index = index.parent();
            insertItemsAt (items, row );

            qDebug() << "Viewport rect contains:" << viewport()->rect().contains(e->pos());
            qDebug() << "Index row:" << row << "Index column:" << col;
            if (dragDropMode() == InternalMove)
                e->setDropAction(Qt::MoveAction);
            e->accept();
        }
        else
        {
            e->ignore();
            return;
        }
    }
    else //TODO:: Thats nasty :-), read trough the documentation of Qt 5
    {
        foreach (QListWidgetItem *i, dynamic_cast<QListWidget*>(e->source())->selectedItems())
        {
            items.append( i->clone() );
        }
        insertItemsAt( items, indexAt( e->pos() ).row() );
        e->accept();
    }
    e->acceptProposedAction();
    stopAutoScroll();
    setState(NoState);
    viewport()->update();
    //    ListWidget::dropEvent(e);
}

void AnimationPlayListWidget::mousePressEvent(QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton )
    {
        m_dragStartPos = e->pos();
    }
    else if ( e->button() == Qt::RightButton )
    {

    }
    ListWidget::mousePressEvent(e);
}

void AnimationPlayListWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton )
    {
        m_dragStopPos = e->pos();
    }
    else if ( e->button() == Qt::RightButton )
    {

    }
    ListWidget::mouseReleaseEvent(e);
}

void AnimationPlayListWidget::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData())
        e->acceptProposedAction();
}



AnimationItem *AnimationPlayListWidget::getNextAnimation()
{
    int rows = count();

    if( rows )
    {
        if( m_lastPlayedAnimation >= rows )
            m_lastPlayedAnimation = 0;

        return dynamic_cast<AnimationItem*>( item( m_lastPlayedAnimation++ ) );
    }else
    {
        return Q_NULLPTR;
    }
}


void AnimationPlayListWidget::insertItemsAt(const QList<QListWidgetItem *> &items, const int row)
{
    foreach (QListWidgetItem *i, items)
    {
        insertItem(row,i);
    }
    Q_EMIT updateUi();
}
