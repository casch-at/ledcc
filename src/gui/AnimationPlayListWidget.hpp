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
namespace animations {
    class Animation;
    class AnimationOptions;
    class AnimationItem;
    class Options;
//    class Draw;
//    class Lift;
//    class Firework;
//    class AxisNailWall;
//    class WireBoxCenterShrinkGrow;
//    class WireBoxCornerShrinkGrow;
//    class Loadbar;
//    class RandomFiller;
//    class RandomSpark;
//    class RandomSparkFlash;
//    class RandomZLift;
//    class Wall;
//    class Rain;
//    class StringFly;
//    class AnimationOptions;
    }
using namespace animations;
/*!
 \brief Holds the animation to play

*/
class AnimationPlayListWidget : public ListWidget
{
    Q_OBJECT
    Q_PROPERTY(Animation* currentAnimation READ currentAnimation WRITE setCurrentAnimation NOTIFY currentAnimationChanged)

public:
    explicit AnimationPlayListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~AnimationPlayListWidget();
    inline animations::Animation* currentAnimation() const  { return m_currentAnimation; }

Q_SIGNALS:
    void currentAnimationChanged(animations::Animation* arg);

public Q_SLOTS:
    void clearList(void);
    void newItem(QList<QListWidgetItem *> item);
    void duplicateItems();
    void removeItems();
    void moveItemsUpDown();
    void editItem();
    void updateAnimation(const AnimationItem *item);
    void updateItemToolTip(const Options &aOptions);
    AnimationItem *getNextAnimation(void);
    inline void setCurrentAnimation(animations::Animation* arg) {
        if (m_currentAnimation != arg) {
            m_currentAnimation = arg;
            emit currentAnimationChanged(arg);
        }
    }
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
    int m_lastPlayedAnimation; /*! Holds the row of the current shown Animation */
    int m_mousePressRow;
    Animation *m_currentAnimation;
    AnimationOptions *m_adjustOptionDialog;
    Q_DISABLE_COPY(AnimationPlayListWidget)
};

#endif // ANIMATIONPLAYLISTWIDGET_HPP
