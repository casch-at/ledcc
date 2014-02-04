#ifndef ANIMATIONOPTIONSGROUPBOX_HPP
#define ANIMATIONOPTIONSGROUPBOX_HPP

#include <QGroupBox>

namespace Ui {
    class AnimationOptionsGroupBox;
    }

class AnimationOptionsGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit AnimationOptionsGroupBox(QWidget *parent = 0);
    ~AnimationOptionsGroupBox();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AnimationOptionsGroupBox *ui;
};

#endif // ANIMATIONOPTIONSGROUPBOX_HPP
