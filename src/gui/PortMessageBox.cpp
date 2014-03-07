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
#include "PortMessageBox.hpp"
#include <QDebug>
PortMessageBox::PortMessageBox(const QString &title, QWidget *parent) :
    QMessageBox(parent)
{
    setTextFormat(Qt::RichText);
    setIconPixmap(QPixmap("://images/dialog-error.png").scaled(QSize(48,48)));
    setWindowTitle(title);
}

PortMessageBox::PortMessageBox(const QString &title, const QString &text, const QString &infText, QWidget *parent) :
    QMessageBox(parent)
{
    setTextFormat(Qt::RichText);
    setIconPixmap(QPixmap("://images/dialog-error.png").scaled(QSize(48,48)));
    setWindowTitle(title);
    setText(text);
    setInformativeText(infText);
}

PortMessageBox::PortMessageBox(const QString &title, const QString &text, QWidget *parent) :
    QMessageBox(parent)
{
    setTextFormat(Qt::RichText);
    setIconPixmap(QPixmap("://images/dialog-error.png").scaled(QSize(48,48)));
    setWindowTitle(title);
    setText(text);
}

//PortMessageBox::~PortMessageBox()
//{
//    qDebug("PortMessageBox destructor");
//}

