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

#include "AboutDialog.hpp"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(const QString &title, const QString &version,
                         const QString &informativeText, const QPixmap &aboutPixmap,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->m_applicationName->setText(title);
    ui->m_version->setText(ui->m_version->text() + version);
    ui->m_informativeText->setText(informativeText);
    ui->m_aboutIcon->setPixmap(aboutPixmap);
    this->setWindowIcon(QIcon(aboutPixmap));
}

AboutDialog::AboutDialog(const QString &title, const QString &version, QWidget *parent):
    QDialog(parent)
{
    ui->setupUi(this);
    ui->m_applicationName->setText(title);
    ui->m_version->setText(ui->m_version->text() + version);
}

void AboutDialog::setInfromativeText(const QString &text)
{
    ui->m_informativeText->setText(text);
}

void AboutDialog::setAboutPixmap(const QPixmap &pix)
{
 ui->m_aboutIcon->setPixmap(pix);
}


AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
