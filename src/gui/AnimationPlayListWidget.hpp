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



class AnimationPlayListWidget : public ListWidget
{
    Q_OBJECT
public:
    explicit AnimationPlayListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~AnimationPlayListWidget();
    QAction *m_playAction;
    QAction *m_stopAction;
    QAction *m_clearAction;
Q_SIGNALS:
    void updateUi(void);

public Q_SLOTS:
    void clearList(void);
    void newItem(QList<QListWidgetItem *> item);
    void duplicateItems();
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void dropEvent(QDropEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

    virtual void dragEnterEvent(QDragEnterEvent *e);

public Q_SLOTS:
    AnimationItem *getNextAnimation(void);
private Q_SLOTS:
    void valueChanged();

private:
    void createActions();
    int m_lastPlayedAnimation; /*! Holds the row of the current shown Animation */
    int m_mousePressRow;
    bool dropOn(QDropEvent *event, int *dropRow, int *dropCol, QModelIndex *dropIndex);
    void insertItemsAt(const QList<QListWidgetItem *> &items, const int row);

    QAction *m_moveDownAction;
    QAction *m_moveUpAction;
    QAction *m_removeAction;
    QAction *m_duplicateAction;
};

#endif // ANIMATIONPLAYLISTWIDGET_HPP
