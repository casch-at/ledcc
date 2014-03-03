#ifndef ANIMATIONOPTIONS_HPP
#define ANIMATIONOPTIONS_HPP

#include <QGroupBox>
#include "Global.hpp"
#include "animations/Draw.hpp"
//#include "AnimationItem.hpp"

namespace Ui {
    class AnimationOptions;
    }

class AnimationOptions : public QGroupBox
{
    Q_OBJECT

public:
   struct  Options{
        QString text;
        u_int16_t speed;
        u_int16_t delay;
        u_int16_t leds;
        u_int16_t iteration;
        Draw::Direction direction;
        Draw::Axis axis;
        bool invert;
        Draw::BixelState state;
    };

    explicit AnimationOptions(QWidget *parent = 0);
    ~AnimationOptions();
    Options *getAnimationSettings(void);
Q_SIGNALS:
    void optionsReady(const Options &animationOptions);
public Q_SLOTS:
    void displayAnimationOptions(const Options options);
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_applyPushB_clicked();

private:
    Ui::AnimationOptions *ui;
    Options m_options;
};

#endif // ANIMATIONOPTIONSGROUPBOX_HPP
