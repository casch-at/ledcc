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
//#include "ui_MainWindow.h"
#include "AnimationOptions.hpp"
#include "AnimationItem.hpp"
#include "Animations.hpp"

// Qt includes
#include <QScrollBar>
#include <QKeyEvent>
#include <QAction>
#include <QSpacerItem>
#include <QGridLayout>
#include <QMessageBox>
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
    setDropIndicatorShown(true);
    setMovement(Free);
    setDragDropMode(DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setAcceptDrops(true);
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
    QMessageBox msgb;
    msgb.setTextFormat(Qt::RichText);
    msgb.setWindowTitle("Clear Animation Playlist");
    msgb.setText(tr("<h3><b>Do you really want clear the animation playlist?</b></h3>"));
    msgb.setInformativeText(tr("This can not be undone!"));
    msgb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgb.setDefaultButton(QMessageBox::No);
    msgb.setIcon(QMessageBox::Warning);
    int ret = msgb.exec();

    if(ret & QMessageBox::Yes){
        clear();
        Q_EMIT contantChanged();
    }
}


/*!
 \brief Insert new item(s)

 Function insert one or more new QListWidgetItems into the QListWidget

 \param[in] item The item(s) to insert
*/
void AnimationPlayListWidget::newItem(QList<QListWidgetItem *> item)
{
    foreach (QListWidgetItem *i, item) {
        addItem(i->clone());
    }
    Q_EMIT contantChanged();


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
    foreach (QListWidgetItem *item, selectedItems()) {
        delete item;
    }
    if (!count()){
        Q_EMIT contantChanged();
    }
    setCurrentRow(currentRow());

}

/*!
 \brief

 \return bool
*/
void AnimationPlayListWidget::moveItemsUpDown()
{
    QObject *emitedObject = QObject::sender();
    QModelIndexList selectedIndx;
    bool up;
    int inListWidget = count();
    int addToRow;

    if(!moveItem(emitedObject, &selectedIndx, &up))
        return;

    sortIndexes(up,&selectedIndx);

    QModelIndex index = selectedIndx.first();
    QListWidgetItem *animation;
    /*
     * If items should be moved up and the top most is selected too insert
     * the item from row zero at the bottom and return, all other get moved anyway.
     */
    if (up){
        if ( index.row() == 0 ){
            animation = takeItem(index.row());
            insertItem(inListWidget - 1, animation);
            setItemSelected(animation, true);
            return;
        }
        addToRow = -1;
    } else {
        /*
     * If items should be moved down and the bottom most is selected too insert the
     * the item from the last row at the top and return, all other get moved anyway.
     */
        if ( index.row() == inListWidget - 1 ){
            animation = takeItem(index.row());
            insertItem(0, animation);
            setItemSelected(animation, true);
            return;
        }
        addToRow = 1;
    }

    foreach (index, selectedIndx) {
        animation = takeItem(index.row());
        insertItem(index.row() + addToRow, animation);
        setItemSelected(animation, true);
    }

}
/*!
 \brief Check if the items should be moved.
 Check if a object exists when yes check if it was either the moveUp or moveDown
 action.

 \param[in] object Caller object
 \param[out] list Pointer to the QModelIndexList where indexes should be stored.
 \param[out] up Pointer to boolean where the move direction gets stored, true if the item(s) should be moved up otherwise false.
 \return bool Return true if items should be moved otherwise false.
*/
bool AnimationPlayListWidget::moveItem(const QObject *object, QModelIndexList *list,  bool *up)
{

    if (object == NULL)
        return false;

    /* Check if caller was the moveUp(Down) action */
    if (object->objectName().compare("m_moveUpAction") == 0)
        *up = true;
    else if (object->objectName().compare("m_moveDownAction") == 0)
        *up = false;
    else
        return false;

    /* If all alle items are selected moveing them makes no sense? also return if no item is selected at all*/
    if( !( *list = selectedIndexes() ).count() /*|| list->count() == *itemsInListWidget*/)
        return false;

    return true;
}

/*!
 \brief Determine where the items should be droped.

 \param event  Pointer to the QDropEvent.
 \param dropRow Row the items should be droped.
 \param dropCol Column the items should be droped.
 \param dropIndex QModelIndex on which the items should be droped.
 \return bool Return true if the items should be droped, otherwise false.
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
        removeItems();
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
 \brief Get the next animationn which should be shown next.

 \return AnimationItem
*/
AnimationItem *AnimationPlayListWidget::getNextAnimation()
{
    int rows = count();

    AnimationItem *retItem = Q_NULLPTR;
    if ( rows ) {
        if ( m_lastPlayedAnimation >= rows )
            m_lastPlayedAnimation = 0;
        retItem = dynamic_cast<AnimationItem*>( item( m_lastPlayedAnimation++ ) );
    }
    return retItem;
}

void AnimationPlayListWidget::setNewItemOptions(AnimationItem *itemForUpdate)
{
    if (indexFromItem(itemForUpdate).row() == m_lastPlayedAnimation - 1) {
        animations()->updateAnimation(itemForUpdate);
    }
    itemForUpdate->createAnimationTooltipAsRichText();
    Q_EMIT showPropertiePreview( itemForUpdate->getAnimationPropertiesAsPlainText() );
}

/*!
 \brief Insert items at a given row.

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
    Q_EMIT contantChanged();
}

void AnimationPlayListWidget::editItem()
{
    QList<QListWidgetItem *> items = selectedItems();
    QList<AnimationItem*> itemList;

    if (!items.isEmpty())
        foreach (QListWidgetItem *i, items)
            itemList.append(dynamic_cast<AnimationItem*>(i));
    else
        for (int i = 0; i < count(); i++)
            itemList.append(dynamic_cast<AnimationItem*>(item(i)));

    AnimationOptions *adjAnimation = new AnimationOptions(itemList, this);
    connect(adjAnimation, &AnimationOptions::applyNewAnimationArguments, this, &AnimationPlayListWidget::setNewItemOptions);
    adjAnimation->exec();
    disconnect(adjAnimation, &AnimationOptions::applyNewAnimationArguments, this, &AnimationPlayListWidget::setNewItemOptions);
    delete adjAnimation;
}

/*!
 \brief Sort the indexes after the row order.
  A insertion algorithm is used for sorting the \a QModelIndexList.

 \param ascending Sort the indexes ascending if true otherwise descending.
 \param list Pointer to \a QModelIndexList to sort.
*/
void AnimationPlayListWidget::sortIndexes(const bool ascending, QModelIndexList *list)
{
    QModelIndex index;
    int i = 0;
    int length = list->length();

    if (ascending){ // Sort indexes ascending 0,1,2,...
        for (int j = 1; j < length; ++j) {
            index = list->at(j);
            i = j - 1;
            while (i >= 0 && list->at(i).row() > index.row()) {
                list->replace(i + 1, list->at(i));
                i -= 1;
            }
            list->replace(i + 1, index);
        }
    } else { // Sort indexes descending row ...,3,2,1,0
        for (int j =  1 ; j < length; ++j) {
            index = list->at(j);
            i = j - 1;
            while (i >= 0 && list->at(i).row() < index.row()) {
                list->replace(i + 1, list->at(i));
                i -= 1;
            }
            list->replace(i + 1, index);
        }
    }
}

