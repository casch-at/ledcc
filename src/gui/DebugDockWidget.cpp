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

