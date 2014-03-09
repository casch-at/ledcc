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
#ifndef DEBUGDOCKWIDGET_H
#define DEBUGDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
    class DebugDockWidget;
    }

class DebugDockWidget : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(QString sendString READ sendString WRITE setSendString NOTIFY sendStringChanged)

public:
    explicit DebugDockWidget(QWidget *parent = Q_NULLPTR);
    ~DebugDockWidget();
    void addLine(const QString &line);
    QString sendString() const
    {
        return m_sendString;
    }

public Q_SLOTS:
    void setSendString(QString arg)
    {
        if(arg.length() > 0){
                m_sendString = arg;
                emit sendStringChanged(arg);
            }
    }

Q_SIGNALS:
    void sendStringChanged(QString arg);

protected:
    void changeEvent(QEvent *e);
private Q_SLOTS:

private:
    void newStringToSend(void);
    Ui::DebugDockWidget *ui;
    QString m_sendString;
};

#endif // DEBUGDOCKWIDGET_H
