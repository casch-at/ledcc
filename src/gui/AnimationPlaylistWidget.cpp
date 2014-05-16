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

#include "AnimationPlaylistWidget.hpp"
//#include "ui_MainWindow.h"
#include "AnimationOptions.hpp"
#include "AnimationItem.hpp"
#include "Animations.hpp"
#include "AnimationHandler.hpp"
#include "XmlPlaylistWriter.hpp"
#include "XmlPlaylistReader.hpp"
#include "System.hpp"

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

// TODO:: Always check how many animations are already in the playlist
//        if there are to many already inside it will need a long long time to move them etc.
/*!
 \brief AnimationPlayListWidget Constructor

 \param[in,out] parent  Parent QWidget past over to \link ListWidget()
*/
AnimationPlaylistWidget::AnimationPlaylistWidget(QWidget *parent) :
    ListWidget(parent),
    m_nextAnimationRow(0)
{
    setDropIndicatorShown(true);
    setMovement(Free);
    setDragDropMode(DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setAcceptDrops(true);
    connect(this, &AnimationPlaylistWidget::itemDoubleClicked, this, &AnimationPlaylistWidget::onItemDoubleClicked);
    openAnimationPlaylist();
    m_animationHandler = Q_NULLPTR;
}

/*!
 \brief Virtual destructor

*/
AnimationPlaylistWidget::~AnimationPlaylistWidget()
{
    saveAnimationPlaylistItems();
    delete  m_animationHandler; /* AnimationPlaylistWidget have to delete the AnimationHandler class since this class holds it. */
}


/*!
 \brief Function to clear the QListWidget.

*/
void AnimationPlaylistWidget::clearList()
{
    QMessageBox msgb;
    msgb.setTextFormat(Qt::RichText);
    msgb.setWindowTitle(tr("Clear Animation Playlist"));
    msgb.setText(tr("<h3><b>Do you really want clear the animation playlist?</b></h3>"));
    msgb.setInformativeText(tr("This can not be undone!"));
    msgb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgb.setDefaultButton(QMessageBox::No);
    msgb.setIcon(QMessageBox::Warning);
    int ret = msgb.exec();

    if(ret & QMessageBox::Yes){
        clear();
        Q_EMIT contentChanged();
    }
}


/*!
 \brief Insert new item(s)

 Function to insert one or more new \a QListWidgetItems into the \a QListWidget

 \param[in] item The item(s) to insert
*/
void AnimationPlaylistWidget::newItem(QList<QListWidgetItem *> item)
{
    foreach (QListWidgetItem *i, item) {
        insertItem(count(), i->clone());
    }
    Q_EMIT contentChanged();


}

/*!
 \brief

*/
void AnimationPlaylistWidget::duplicateItems()
{
    foreach (QListWidgetItem *item, selectedItems()) {
        insertItem(indexFromItem(item).row(),dynamic_cast<AnimationItem *>(item)->clone());
    }
}

/*!
 \brief

*/
void AnimationPlaylistWidget::removeItems()
{
    if (selectedItems().isEmpty())
        return;

    foreach (QListWidgetItem *i, selectedItems())
        delete i;

    if (!count())
        Q_EMIT contentChanged();

    setCurrentRow(currentRow());

}

/*!
 \brief

 \return bool
*/
void AnimationPlaylistWidget::moveItemsUpDown()
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
bool AnimationPlaylistWidget::moveItem(const QObject *object, QModelIndexList *list,  bool *up)
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
bool AnimationPlaylistWidget::dropOn(QDropEvent *event, int *dropRow, int *dropCol, QModelIndex *dropIndex)
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
void AnimationPlaylistWidget::keyPressEvent(QKeyEvent *e)
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
void AnimationPlaylistWidget::dragMoveEvent(QDragMoveEvent *event)
{

    ListWidget::dragMoveEvent(event);
}

/*!
 \brief

 \param e
*/
void AnimationPlaylistWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    ListWidget::dragLeaveEvent(e);
}


/*!
 \brief Reimplemented virtual proteced \a dropEvent function.
 Reimplemented dropEvent function to add some more functionality.
 \param[in] e \link QDropEvent
*/
void AnimationPlaylistWidget::dropEvent(QDropEvent *e)
{
    QList<QListWidgetItem*> items;

    if ( e->source() == this  ) {
        QModelIndex index;
        int row = -1;
        int col = -1;
        if (dropOn(e,&row,&col,&index)) /* Evaluate the row where to item(s) should be droped befor clone */
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
    } else if (!e->source()->objectName().compare("m_animationList")) {
        foreach (QListWidgetItem *i, dynamic_cast<ListWidget*>(e->source())->selectedItems()) {
            items.insert(0, i->clone());
        }
        insertItemsAt( items, indexAt( e->pos() ).row() );
        e->accept();
    } else {
        e->ignore();
    }
    stopAutoScroll();
    setState(NoState);
    viewport()->update();
}

/*!
 \brief

 \param e
*/
void AnimationPlaylistWidget::mousePressEvent(QMouseEvent *e)
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
void AnimationPlaylistWidget::mouseReleaseEvent(QMouseEvent *e)
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
void AnimationPlaylistWidget::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData())
        e->acceptProposedAction();
}


/*!
 \brief Get the next animationn which should be shown next.

 \return AnimationItem
*/
AnimationItem *AnimationPlaylistWidget::getNextAnimation()
{
    int rows = count();

    AnimationItem *retItem = Q_NULLPTR;
    if ( rows ) {
        if ( m_nextAnimationRow >= rows )
            m_nextAnimationRow = 0;
        retItem = dynamic_cast<AnimationItem*>( item( m_nextAnimationRow++ ) );
    }
    return retItem;
}

void AnimationPlaylistWidget::saveAnimationPlaylistItems()
{
    QList<AnimationItem*> animationItems;
    XmlPlaylistWriter xmlWriter;
    getAllItems(&animationItems);
    xmlWriter.writeAnimationPlaylist(&animationItems, System::getConfigPath() + "animations.xml");
}

void AnimationPlaylistWidget::saveAnimationPlaylistItemsTo(const QString &location)
{
    QList<AnimationItem*> animationItems;
    getAllItems(&animationItems);
    XmlPlaylistWriter xmlWriter;
    xmlWriter.writeAnimationPlaylist(&animationItems, location);
}

void AnimationPlaylistWidget::openAnimationPlaylistFrom(const QString &file)
{
    XmlPlaylistReader reader;
    QList<AnimationItem*> animationItems = reader.readAnimationPlaylist(file);
    foreach (AnimationItem *item, animationItems) {
        insertItem(count() ,item);
    }
    Q_EMIT contentChanged();
}

void AnimationPlaylistWidget::setNewItemOptions(AnimationItem *itemForUpdate)
{
    if (indexFromItem(itemForUpdate).row() == m_nextAnimationRow - 1) {
        m_animationHandler->getAnimations()->updateAnimation(itemForUpdate);
    }
    itemForUpdate->createAnimationTooltipAsRichText();
    Q_EMIT showPropertiePreview( itemForUpdate->getAnimationPropertiesAsPlainText() );
}

/*!
 \brief Insert items at a given row.

 \param items
 \param row
*/
void AnimationPlaylistWidget::insertItemsAt(const QList<QListWidgetItem *> &items, const int row)
{
    foreach (QListWidgetItem *i, items)
    {
        insertItem(row,i);
        setItemSelected(i,true);
    }
    Q_EMIT contentChanged();
}

/*!
 \brief Edit the animation properties

*/
void AnimationPlaylistWidget::editItem()
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
    connect(adjAnimation, &AnimationOptions::applyNewAnimationArguments, this, &AnimationPlaylistWidget::setNewItemOptions);
    adjAnimation->exec();
    disconnect(adjAnimation, &AnimationOptions::applyNewAnimationArguments, this, &AnimationPlaylistWidget::setNewItemOptions);
    delete adjAnimation;
}

void AnimationPlaylistWidget::onItemDoubleClicked(QListWidgetItem *item)
{
    m_nextAnimationRow = indexFromItem(item).row() + 1 ;
    Q_EMIT playAnimation(dynamic_cast<AnimationItem*>(item));
}

/*!
 \brief Sort the indexes after the row order.
  A insertion algorithm is used for sorting the \a QModelIndexList.
 \param ascending Sort the indexes ascending if true otherwise descending.
 \param list Pointer to \a QModelIndexList to sort.
*/
void AnimationPlaylistWidget::sortIndexes(const bool ascending, QModelIndexList *list)
{
    QModelIndex index;
    int i = 0;
    int length = list->length();

    if (ascending){ // Sort indexes after there row ascending 0,1,2,...
        for (int j = 1; j < length; ++j) {
            index = list->at(j);
            i = j - 1;
            while (i >= 0 && list->at(i).row() > index.row()) {
                list->replace(i + 1, list->at(i));
                i -= 1;
            }
            list->replace(i + 1, index);
        }
    } else { // Sort indexes after there row descending  ...,3,2,1,0
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

/*!
 \brief Open the playlist which was saved on exit.
        The playlist is located in the user home directory,
        on Linux system under ~/.config/ledcc/animations.xml

*/
void AnimationPlaylistWidget::openAnimationPlaylist()
{
    XmlPlaylistReader reader;
    QList<AnimationItem*> animationItems = reader.readAnimationPlaylist(System::getConfigPath() + "animations.xml");
    foreach (AnimationItem *item, animationItems) {
        insertItem(count() ,item);
    }
    Q_EMIT contentChanged();
}



