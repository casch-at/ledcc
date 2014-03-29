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
#ifndef ANIMATIONLISTWIDGET_HPP
#define ANIMATIONLISTWIDGET_HPP

#include <QListWidget>
#include "AnimationItem.hpp"
/**
 * @brief Class AnimationListWidget handles all available animation
 *
 *
 */
class AnimationListWidget : public QListWidget
{
    Q_OBJECT
public:
    AnimationListWidget(QWidget* parent = Q_NULLPTR);
Q_SIGNALS:
    void addToPlaylist(QList<QListWidgetItem *> selected);
    void showPropertiePreview(QListWidgetItem *item);
protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void dragMoveEvent(QDragMoveEvent *e);
public Q_SLOTS:
    void selectAllItems(void);
    void insertAnimation(const QString &animation);
    void on_itemSelectionChanged();
private:
};

#endif // ANIMATIONLISTWIDGET_HPP
