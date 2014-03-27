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
#ifndef ANIMATIONPROPERTIESPREVIEW_HPP
#define ANIMATIONPROPERTIESPREVIEW_HPP

#include <QDockWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QShortcut;
class QFrame;
QT_END_NAMESPACE

namespace Ui {
    class animationPropertiesPreview;
    }

class AnimationPropertiesPreview : public QDockWidget
{
    Q_OBJECT
public:
    explicit AnimationPropertiesPreview( QWidget *parent = 0 );
    ~AnimationPropertiesPreview();
Q_SIGNALS:

public Q_SLOTS:
    void createPropertiePreview(QStringList &properties );

private Q_SLOTS:
    inline void hideShow() { setVisible( !isVisible() ); }
protected:
    void changeEvent( QEvent *e );
private:
    void clearFrame( void );
    Ui::animationPropertiesPreview *ui;
    QVBoxLayout *mlayout;
    QVBoxLayout *mlabelLayout;
    QShortcut *hideShowSC;
    QFrame *frame;
    static const uint PIXEL_SIZE_NAME = 16;
    static const uint PIXEL_SIZE_PROPERTIES = 11;
    Q_DISABLE_COPY( AnimationPropertiesPreview )
};

#endif // ANIMATIONPROPERTIESPREVIEW_HPP
