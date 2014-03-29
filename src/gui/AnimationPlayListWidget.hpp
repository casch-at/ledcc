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
#ifndef ANIMATIONPLAYLISTWIDGET_HPP
#define ANIMATIONPLAYLISTWIDGET_HPP

#include <ListWidget.hpp>
#include "AnimationItem.hpp"

class Timer;

class AnimationPlayListWidget : public ListWidget
{
    Q_OBJECT
public:
    explicit AnimationPlayListWidget(QWidget *parent = Q_NULLPTR);

Q_SIGNALS:
    void updateUi(void);
    void displayAnimationOptions(const AnimationOptions::Options *options);
public Q_SLOTS:
    void clearList(void);
    void newItem(QList<QListWidgetItem *> item);
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void dropEvent(QDropEvent *e);
public Q_SLOTS:
    AnimationItem *getNextAnimation(void);
private Q_SLOTS:
    void on_itemDoubleClicked(QListWidgetItem *item);
private:
    int m_lastPlayedAnimationRow;
    void insertItemsAt(const QList<QListWidgetItem *> &items, const int row);
};

#endif // ANIMATIONPLAYLISTWIDGET_HPP
