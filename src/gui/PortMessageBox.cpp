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

PortMessageBox::~PortMessageBox()
{
    qDebug("PortMessageBox destructor");
}

