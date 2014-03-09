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
#include "DebugDockWidget.hpp"
#include "ui_DebugDockWidget.h"

DebugDockWidget::DebugDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DebugDockWidget)
{
    ui->setupUi(this);
    connect(ui->sendDebugLE,&QLineEdit::returnPressed,this,&DebugDockWidget::newStringToSend);

}

DebugDockWidget::~DebugDockWidget()
{
    delete ui;
}

void DebugDockWidget::addLine(const QString &line)
{
    ui->debugPTE->appendPlainText(line);
}

void DebugDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
        }
}

void DebugDockWidget::newStringToSend()
{
    setSendString(ui->sendDebugLE->text());
//    ui->sendDebugLE->clear();
}

