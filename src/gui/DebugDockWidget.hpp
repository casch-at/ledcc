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
