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
#include "AnimationListWidget.hpp"

// Third Party
#include "aqp.hpp"
#include "alt_key.hpp"

// Qt includes
#include <QKeyEvent>
#include <QAction>
#ifdef _DEBUG_
#include <QDebug>
#endif
//#include "animations/Lift.hpp"
//#include <QtWidgets/QAction>

// FIXME:: Edit signal not working in AnimationListWidget because connection between AnimationListWidge object and AnimationOptions object are missing
//         However there is no need anymore because the AnimationOptions class gets modified anyway and a object of the new class will be in the base class
AnimationListWidget::AnimationListWidget(QWidget *parent):
    ListWidget(parent)
{
    setDropIndicatorShown(false);
    setMovement(Static);
    setDefaultDropAction(Qt::IgnoreAction);
    setEditTriggers(DoubleClicked | EditKeyPressed);
    setDragDropMode(DragOnly);
    setDragEnabled(true);
    setAcceptDrops(false);
    setSortingEnabled(true);
    createActions();

    connect( m_addToPlaylistAction, &QAction::triggered, this, &AnimationListWidget::addSelectedItemsToPlaylist);
}

AnimationListWidget::~AnimationListWidget()
{

}

void AnimationListWidget::keyPressEvent(QKeyEvent *e)
{
    //TODO::Fix space selection!!!
    //      Actuall behaviour, item stays always selected ones item is selected because
    //      when moving item item gets selected ;-) Strange behaviour
    switch (e->key()) {
    case Qt::Key_Enter:
        addSelectedItemsToPlaylist();
        break;
    case Qt::Key_Return:
        addSelectedItemsToPlaylist();
        break;
    case Qt::Key_Up:
        ListWidget::keyPressEvent(e);
        break;
    case Qt::Key_Down:
        ListWidget::keyPressEvent(e);
        break;
    case Qt::Key_Escape:
        ListWidget::keyPressEvent(e);
        break;
    case Qt::Key_Space:
        break;
    default:
        e->ignore();
        break;
    }
    e->accept();
}



void AnimationListWidget::insertAnimation(const QString &animation)
{
    addItem(animation);
}

void AnimationListWidget::createActions()
{
    m_addToPlaylistAction = createAction(tr("Add to Playlist"));
    addAction(m_addToPlaylistAction);
    AQP::accelerateActions(actions());
}


void AnimationListWidget::addSelectedItemsToPlaylist()
{
    Q_EMIT addToPlaylist(selectedItems());
}
