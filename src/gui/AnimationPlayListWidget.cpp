
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

// Third Party
#include "aqp.hpp"
#include "alt_key.hpp"
// Qt includes
#include <QScrollBar>
#include <QKeyEvent>
#include <QAction>
#ifdef _DEBUG_
#include <QDebug>
#endif


/*!
 \brief AnimationPlayListWidget Constructor

 \param[in,out] parent  Parent QWidget past over to \link ListWidget()
*/
AnimationPlayListWidget::AnimationPlayListWidget(QWidget *parent) :
    ListWidget(parent),
    m_lastPlayedAnimation(0)
{
    // QListWidget settings
    setDropIndicatorShown(true);
    setMovement(Free);
    setDragDropMode(DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setAcceptDrops(true);

    // Internal
    createActions();

    // Create connection
    connect( m_clearAction, &QAction::triggered, this, &AnimationPlayListWidget::clearList);
    connect( m_duplicateAction, &QAction::triggered, this, &AnimationPlayListWidget::duplicateItems);
    connect( m_removeAction, &QAction::triggered, this, &AnimationPlayListWidget::removeItems);
    connect( m_moveUpAction, &QAction::triggered, this, &AnimationPlayListWidget::moveItemsUpDown);
    connect( m_moveDownAction, &QAction::triggered, this, &AnimationPlayListWidget::moveItemsUpDown);
}


/*!
 \brief Virtual destructor

*/
AnimationPlayListWidget::~AnimationPlayListWidget()
{
    //    delete m_clearAction;
    //    delete m_stopAction;
    //    delete m_playAction;
    //    delete m_duplicateAction;
    //    delete m_moveDownAction;
    //    delete m_moveUpAction;
    //    delete m_removeAction;
}


/*!
 \brief Function to clear the QListWidget.

*/
void AnimationPlayListWidget::clearList()
{
    clear();
    m_clearAction->setDisabled(true);
    Q_EMIT updateUi();
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

    if (count()) {
        m_clearAction->setEnabled(true);
    }
    Q_EMIT updateUi();


}

/*!
 \brief

*/
void AnimationPlayListWidget::duplicateItems()
{
    foreach (QListWidgetItem *item, selectedItems()) {
        insertItem(indexFromItem(item).row(),dynamic_cast<AnimationItem *>(item)->clone());
    }
}

/*!
 \brief

*/
void AnimationPlayListWidget::removeItems()
{
    QList<QListWidgetItem *> items = selectedItems();
    foreach (QListWidgetItem *item, items) {
        delete item;
    }
    if (!count()){
        m_clearAction->setDisabled(true);
        Q_EMIT updateUi();
    }

}


//TODO:: What is when items gets mixed selected, e.g., second and last row and the user wants move them up
// Create a function where item for item gets
/*!
 \brief

 \return bool
*/
bool AnimationPlayListWidget::moveItemsUpDown()
{
    QObject *emitedObject = QObject::sender();
    QModelIndexList selectedIndx;
    QList<QListWidgetItem *> items;
    int firstItemRowList = -1;
    int lastItemRowList = -1;
    int inListWidget = count();
    bool reverse = true;
    bool up;

    if (emitedObject == NULL)
        return false;

    if (emitedObject->objectName().compare("UP") == 0)
        up = true;
    else if (emitedObject->objectName().compare("DOWN") == 0)
        up = false;
    else
        return false;

    /* If all alle items are selected moveing them makes no sense also return if no item is selected*/
    if( !(selectedIndx = selectedIndexes()).count() || selectedIndx.count() == inListWidget)
        return false;


    firstItemRowList = selectedIndx.first().row();
    lastItemRowList = selectedIndx.last().row();

    /* Swap rows if needed */
    if (firstItemRowList > lastItemRowList) {
        int tmp = firstItemRowList;
        firstItemRowList = lastItemRowList;
        lastItemRowList = tmp;
        if(lastItemRowList + 1 == inListWidget && up) /* If we are already at the bottom of the list we don't need to move */
            return false;
        else if (!firstItemRowList) /* If we are already at the top of the list we don't need to move */
            return false;

        reverse = !reverse;
    }

    foreach (QModelIndex index, selectedIndx) {
        items.append(takeItem(index.row()));
    }
    qDebug() << "LastItemRow:" << lastItemRowList << "FirstItemRow:" << firstItemRowList << "Differnce:" << lastItemRowList - firstItemRowList;
    int insertAt = lastItemRowList - firstItemRowList;

    if (up)
        insertAt = firstItemRowList - 1;
    else
        insertAt = firstItemRowList + 1;

    qDebug() << "insertAt:" << insertAt;
    QListWidgetItem *itemToInsert;
    QListIterator<QListWidgetItem*> itemsIter(items);
    if (reverse){
        itemsIter.toBack();
        while (itemsIter.hasPrevious()){
            itemToInsert = itemsIter.previous();
            insertItem(insertAt, itemToInsert);
            setItemSelected(itemToInsert, true);
        }
    } else {
        while (itemsIter.hasNext()) {
            itemToInsert = itemsIter.next();
            insertItem(insertAt, itemToInsert);
            setItemSelected(itemToInsert, true);
        }
    }
    return true;
}

/*!
 \brief

*/
void AnimationPlayListWidget::createActions()
{
    m_moveDownAction = createAction(tr("Move Down"));
    m_moveDownAction->setObjectName("DOWN");

    m_moveUpAction = createAction(tr("Move Up"));
    m_moveUpAction->setObjectName("UP");

    m_duplicateAction = createAction(tr("Duplicate"));

    m_removeAction = createAction(tr("Remove"));
    m_removeAction->setToolTip(tr("Remove one or more selected Animation(s)"));
    m_removeAction->setStatusTip(tr("Remove one or more selected Animation(s)"));

    m_playAction = createAction(tr("Play"));
    m_playAction->setIcon(QIcon("://images/media-playback-start-9.png"));
    m_playAction->setShortcut(tr("R"));
    m_playAction->setToolTip(tr("Play Animation R"));

    m_stopAction = createAction(tr("Stop"));
    m_stopAction->setIcon(QIcon("://images/media-playback-pause-7.png"));
    m_stopAction->setShortcut(tr("P"));
    m_stopAction->setToolTip(tr("Pause Animations P"));


    m_clearAction = createAction(tr("Clear List"), tr("This can not be undone!"));
    m_clearAction->setIcon( QIcon( "://images/clear.png" ) );
    m_clearAction->setShortcut( QKeySequence::Refresh );

    actions().first()->setShortcut(tr("Ctrl+E"));
    if (!count())
        m_clearAction->setDisabled(true);
    addActions(QList<QAction *>() << m_playAction << m_stopAction << m_moveUpAction << m_moveDownAction << m_duplicateAction << m_removeAction << m_clearAction);
    AQP::accelerateActions(actions());
}

/*!
 \brief

 \param event
 \param dropRow
 \param dropCol
 \param dropIndex
 \return bool
*/
bool AnimationPlayListWidget::dropOn(QDropEvent *event, int *dropRow, int *dropCol, QModelIndex *dropIndex)
{
    if (event->isAccepted())
        return false;

    QModelIndex index;
    //     rootIndex() (i.e. the viewport) might be a valid index
    if (viewport()->rect().contains(event->pos())) {
        index = indexAt(event->pos());
        if (!index.isValid() || !visualRect(index).contains(event->pos()))
            index = rootIndex();
    }


    // If we are allowed to do the drop
    if (model()->supportedDropActions() & event->dropAction()) {
        int row = -1;
        int col = -1;
        switch (dropIndicatorPosition()) {
        case QAbstractItemView::AboveItem:
            row = index.row();
            col = index.column();
            //            index = index.parent();
            break;
        case QAbstractItemView::BelowItem:
            row = index.row();
            col = index.column();
            //            index = index.parent();
            break;
        case QAbstractItemView::OnItem:
        case QAbstractItemView::OnViewport:
            break;
        }
        *dropIndex = index;
        *dropRow = row;
        *dropCol = col;
        return true;
    }
    return false;
}


/*!
 \brief Reimplement the keyPressEvent function to add some more functionality

 \param e
*/
void AnimationPlayListWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Delete:
        foreach(QListWidgetItem *i,selectedItems())
            delete i;

        if(count())
            setCurrentRow(currentRow());

        emit updateUi();
        break;
    case Qt::Key_Up:
        ListWidget::keyPressEvent(e);
        break;
    case Qt::Key_Down:
        ListWidget::keyPressEvent(e);
        break;
    case Qt::Key_Return:
        break;
    case Qt::Key_Escape:
        ListWidget::keyPressEvent(e);
        break;
    default:
        e->ignore();
        return;
        break;
    }
    e->accept();
}

/*!
 \brief

 \param event
*/
void AnimationPlayListWidget::dragMoveEvent(QDragMoveEvent *event)
{

    ListWidget::dragMoveEvent(event);
}

/*!
 \brief

 \param e
*/
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
        QModelIndex index;
        int row = -1;
        int col = -1;
        if (dropOn(e,&row,&col,&index)) /* Evalute the row where to item(s) should be droped befor clone */
        {
            foreach (QListWidgetItem *i, selectedItems())
            {
                items.append( i->clone() );
                delete i;
            }
            e->setDropAction(Qt::MoveAction);
            insertItemsAt (items, row );
            scrollToItem(item(index.row()));
            e->accept();
        }
        else
        {
            e->ignore();
            return;
        }
    }
    else //FIXME::Dangerous!!! When source of the event is not a QListWidget application will crash
    {
        foreach (QListWidgetItem *i, dynamic_cast<QListWidget*>(e->source())->selectedItems())
        {
            items.append( i->clone() );
        }
        insertItemsAt( items, indexAt( e->pos() ).row() );
        e->accept();
    }
    stopAutoScroll();
    setState(NoState);
    viewport()->update();
}

/*!
 \brief

 \param e
*/
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

/*!
 \brief

 \param e
*/
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

/*!
 \brief

 \param e
*/
void AnimationPlayListWidget::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData())
        e->acceptProposedAction();
}

/*!
 \brief

 \return AnimationItem
*/
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

/*!
 \brief

 \param items
 \param row
*/
void AnimationPlayListWidget::insertItemsAt(const QList<QListWidgetItem *> &items, const int row)
{
    foreach (QListWidgetItem *i, items)
    {
        insertItem(row,i);
        setItemSelected(i,true);
    }
    Q_EMIT updateUi();
}
