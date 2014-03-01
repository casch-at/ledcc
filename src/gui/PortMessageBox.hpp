#ifndef PORTMESSAGEBOX_HPP
#define PORTMESSAGEBOX_HPP

#include <QMessageBox>

class PortMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit PortMessageBox(const QString &title,QWidget *parent = Q_NULLPTR);
    explicit PortMessageBox(const QString &title, const QString &text, const QString &infText, QWidget *parent = Q_NULLPTR);
    explicit PortMessageBox(const QString &title, const QString &text, QWidget *parent = Q_NULLPTR);
//    ~PortMessageBox();
Q_SIGNALS:

public Q_SLOTS:
private:

};

#endif // PORTMESSAGEBOX_HPP
