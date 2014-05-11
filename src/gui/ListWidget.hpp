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

#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include <QListWidget>




class QTimer;
class QShortcut;
class AnimationItem;

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~ListWidget();
    void getAllItems(QList<AnimationItem*> *animationItems);
    QList<AnimationItem*> getAllItems();
Q_SIGNALS:
    void showPropertiePreview(QStringList itemProperties);
public Q_SLOTS:
    void selectAllItems();
    void on_itemSelectionChanged();
    void on_showPropertiesPreviewTimerTimeout();
    void on_customContextMenuRequest(const QPoint &pos);
    inline void focus() { setFocus(); }
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void leaveEvent(QEvent *e);
    AnimationItem *m_itemToShowProperties;
    QPoint m_dragStartPos;
    QPoint m_dragStopPos;

    QAction *createAction(const QString &text, const QString &tooltip = "");

private:
    quint8 m_scrollThrough;
    QTimer *m_showPropertiesPreview;
};

#endif // LISTWIDGET_HPP
