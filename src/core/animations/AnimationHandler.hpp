/**
 * Copyright (C) 2014  Christian Schwarzgruber <christiandev9@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ANIMATIONHANDLER_HPP
#define ANIMATIONHANDLER_HPP
#include "Options.hpp"
#include <SettingsDialog.hpp>
#include <QObject>
#include <QHash>
//#include "AnimationListWidget.hpp"
//#include "AnimationListWidget.hpp"
class AnimationPlayListWidget;
class Animation;
class AnimationItem;
class Sender;
    /*!
     \brief Class \a AnimationHandler interface between all animations

    */
class AnimationHandler : public QObject
{
    Q_OBJECT
public:
    AnimationHandler(QObject *object = Q_NULLPTR);
    ~AnimationHandler();

Q_SIGNALS:
    void startAnimation();
    void stopPlay();
    void updateUi();
    void okClosePort();
    void openSerialInterface(const SettingsDialog::SerialSettings &port);

public Q_SLOTS:
    void animationDone(void);
    void playAnimations(void);
    void openCloseSerialPort(void);
private Q_SLOTS:
    void portOpen(const QString &message);
    void displayPortErrorMessage(const QString &message);
    void closePort(const QString &message);
    void portClosed(const QString &message);
    void stopThreads(void);

private:
    void setupSenderThread(void);
    void playNextAnimation(const AnimationItem *item);
private:
    QThread *m_createThread;
    QThread *m_senderThread;
    Sender *m_sender;
    SettingsDialog::SerialSettings m_port;
    bool m_portOpened;
    bool m_stopPlay;
    AnimationItem *m_currentAnimationItem;
    Animation *m_currentAnimation;
    AnimationPlayListWidget *m_animationPlaylist;
    Q_DISABLE_COPY(AnimationHandler)
};
#endif // ANIMATIONHANDLER_HPP
