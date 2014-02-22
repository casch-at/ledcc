#ifndef STRINGFLY_HPP
#define STRINGFLY_HPP

#include "Animation.hpp"

class StringFly : public Animation
{
    Q_OBJECT
    Q_PROPERTY(QString sToDisplay READ getSToDisplay WRITE setSToDisplay)
public:
    explicit StringFly(const u_int16_t &speed,
                       const QString &string = "3D LED CUBE",
                       const QString &name = "String Fly",
                       QObject *parent = 0);

    virtual void createAnimation();
    QString getSToDisplay() const
    {
        return m_sToDisplay;
    }

signals:

public slots:

    void setSToDisplay(const QString &string)
    {
        if(m_sToDisplay.compare(string) != 0)
            m_sToDisplay = string;
    }
private:
    QString m_sToDisplay;
};

#endif // STRINGFLY_HPP
