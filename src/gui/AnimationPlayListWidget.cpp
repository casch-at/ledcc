/**
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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "AnimationPlayListWidget.hpp"
#include <QKeyEvent>
#ifdef _DEBUG_
#include <QDebug>
#endif
//TODO:: Scroll viewport when draging item
AnimationPlayListWidget::AnimationPlayListWidget(QWidget *parent) :
    ListWidget(parent),
    m_lastPlayedAnimationRow(0)
{
    setDefaultDropAction(Qt::MoveAction);

    // Create connection
    connect( this, &QListWidget::itemDoubleClicked, this, &AnimationPlayListWidget::on_itemDoubleClicked);
}

AnimationPlayListWidget::~AnimationPlayListWidget()
{

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

void AnimationPlayListWidget::keyPressEvent(QKeyEvent *e)
{
    int cRow = -1;

    switch (e->key()) {
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
    if (this->dropIndicatorPosition() == QAbstractItemView::AboveItem )
    {
        event->ignore();
        return;
    }

    qDebug("dragMoveEvent");
    event->accept();
}

void AnimationPlayListWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
}

void AnimationPlayListWidget::dropEvent(QDropEvent *e)
{
    QList<QListWidgetItem*> items;

    if( e->source() == this )
    {
        foreach (QListWidgetItem *i, selectedItems())
        {
            items.append( i->clone() );
            delete i;
        }
        insertItemsAt (items, indexAt( e->pos() ).row() );
    }else if( e->source()->objectName().compare("availableAnimationsLW") == 0 ) //TODO:: Thats nasty :-), read trough the documentation of Qt 5
    {
        foreach (QListWidgetItem *i, dynamic_cast<QListWidget*>(e->source())->selectedItems())
        {
            items.append( i->clone() );
        }
        insertItemsAt( items, indexAt(e->pos()).row() );
        e->accept();
    }
    e->acceptProposedAction();
}


AnimationItem *AnimationPlayListWidget::getNextAnimation()
{
    int rows = count();

    if( rows )
    {
        if( m_lastPlayedAnimationRow >= rows )
            m_lastPlayedAnimationRow = 0;

        return dynamic_cast<AnimationItem*>( item( m_lastPlayedAnimationRow++ ) );
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
