#ifndef ANIMATIONPROPERTIESPREVIEW_HPP
#define ANIMATIONPROPERTIESPREVIEW_HPP

#include <QDockWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
QT_END_NAMESPACE

class AnimationPropertiesPreview : public QDockWidget
{
    Q_OBJECT
public:
    explicit AnimationPropertiesPreview(QWidget *parent = 0);
    ~AnimationPropertiesPreview();
signals:

public slots:
    void setProperties(const QString &name, const QString &properties);
private:
    QVBoxLayout *mlayout;
    QVBoxLayout *mlabelLayout;
    QWidget *mcentralWidget;
};

#endif // ANIMATIONPROPERTIESPREVIEW_HPP
