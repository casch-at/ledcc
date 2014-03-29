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
#include <QKeyEvent>
#include <QTimer>
#ifdef _DEBUG_
#include <QDebug>
#endif
//#include "animations/Lift.hpp"
//#include <QtWidgets/QAction>

AnimationListWidget::AnimationListWidget(QWidget *parent):
    ListWidget(parent)
{

}

AnimationListWidget::~AnimationListWidget()
{

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
        Q_EMIT addToPlaylist(this->selectedItems());
        break;
    case Qt::Key_Return:
        Q_EMIT addToPlaylist(this->selectedItems());
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


void AnimationListWidget::insertAnimation(const QString &animation)
{
    addItem(animation);
}


