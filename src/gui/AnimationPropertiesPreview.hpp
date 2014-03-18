#ifndef ANIMATIONPROPERTIESPREVIEW_HPP
#define ANIMATIONPROPERTIESPREVIEW_HPP

#include <QDockWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QShortcut;
QT_END_NAMESPACE

class AnimationPropertiesPreview : public QDockWidget
{
    Q_OBJECT
public:
    explicit AnimationPropertiesPreview(QWidget *parent = 0);
    ~AnimationPropertiesPreview();
Q_SIGNALS:

public Q_SLOTS:
    void setProperties(const QString &name, const QString &properties);
private Q_SLOTS:
    inline void hideShow(){
        setVisible(!isVisible());
    }

private:
    QVBoxLayout *mlayout;
    QVBoxLayout *mlabelLayout;
    QWidget *mcentralWidget;
    QShortcut *hideShowSC;
};

#endif // ANIMATIONPROPERTIESPREVIEW_HPP
