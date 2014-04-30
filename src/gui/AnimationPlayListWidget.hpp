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

class Animation;
class AnimationOptions;
class AnimationItem;
class Options;


/*!
 \brief Holds the animation to play

*/
class AnimationPlayListWidget : public ListWidget
{
    Q_OBJECT
public:
    explicit AnimationPlayListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~AnimationPlayListWidget();
Q_SIGNALS:
    void contantChanged();
    void playAnimation(const AnimationItem *animation);
public Q_SLOTS:
    void clearList(void);
    void newItem(QList<QListWidgetItem *> item);
    void duplicateItems();
    void removeItems();
    void moveItemsUpDown();
    void editItem();
    void onItemDoubleClicked(QListWidgetItem *item);
    AnimationItem *getNextAnimation(void);
    void saveAnimationPlaylistItems();
    void saveAnimationPlaylistItemsTo();
    void openAnimationPlaylistFrom();
private Q_SLOTS:
    void setNewItemOptions(AnimationItem *itemForUpdate);
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void dropEvent(QDropEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void dragEnterEvent(QDragEnterEvent *e);
private:
    bool moveItem(const QObject *object, QModelIndexList *list, bool *up);
    bool dropOn(QDropEvent *event, int *dropRow, int *dropCol, QModelIndex *dropIndex);
    void insertItemsAt(const QList<QListWidgetItem *> &items, const int row);
    void sortIndexes(const bool ascending, QModelIndexList *list);
    void openAnimationPlaylist();
    void getAllItems(QList<AnimationItem*> *animationItems);
    int m_nextAnimationRow; /*! Holds the row of the current shown Animation */
    int m_mousePressRow;
    Animation *m_currentAnimation;

    Q_DISABLE_COPY(AnimationPlayListWidget)
};



#endif // ANIMATIONPLAYLISTWIDGET_HPP
