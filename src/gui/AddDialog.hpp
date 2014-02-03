/*************************************************************
 * 	File Name       : adddialog.hpp
 *************************************************************
 *  Created on	 : Jan 30, 2013
 *  Author 		 : Christian Schwarzgruber
 *  Version		 : 1.0
 *************************************************************
 *  Description	 : ADDDIALOG Class
 *************************************************************/
#ifndef ADDDIALOG_HPP
#define ADDDIALOG_HPP

#include <QDialog>

namespace Ui {
class ADDDIALOG;
}

class ADDDIALOG : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(bool cancel READ cancel WRITE setCancel)
public:
    explicit ADDDIALOG(QWidget *parent = 0);
    ~ADDDIALOG();
    
    QString text() const
    {
        return m_text;
    }

    bool cancel() const
    {
        return m_cancel;
    }

public slots:
    void setText(QString arg)
    {
        m_text = arg;
    }

    void setCancel(bool arg)
    {
        m_cancel = arg;
    }

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonCancel_clicked();

    void on_lineEditItem_returnPressed();

    void storeText();
private:
    Ui::ADDDIALOG *ui;
    QString m_text;
    bool m_cancel;
};

#endif // ADDDIALOG_HPP
