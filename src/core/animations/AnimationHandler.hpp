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
#include <QObject>
#include <QHash>

class AnimationPlaylistWidget;
class Animation;
class Animations;
class AnimationItem;

class Sender;
class SettingsDialog;

class QAction;

    /*!
     \brief Class \a AnimationHandler interface between all animations

    */
class AnimationHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPortOpen READ isPortOpen WRITE setIsPortOpen )
public:
    explicit AnimationHandler(QWidget *widget);
    virtual ~AnimationHandler();
    Sender * getSender() { return m_sender; }
    SettingsDialog *m_settingsDialog;
    Animations* getAnimations() const { return m_animations; }
    bool isPortOpen() const
    {
        return m_isPortOpen;
    }
    void setAction(QAction *action);
Q_SIGNALS:
    void startAnimation();
    void stopPlay();
    void updateUi(bool val);
    void okClosePort();
    void openSerialInterface();

public Q_SLOTS:
    void animationDone(void);
    void playAnimations(void);
    void playAnimation(const AnimationItem  *animation);
    void openCloseSerialPort(void);
    void stopThreads(void);
    void closePort(const QString &message);
    void portOpen(const QString &message);
    void portClosed(const QString &message);
    void setIsPortOpen(bool arg)
    {
        m_isPortOpen = arg;
    }

private Q_SLOTS:
    void displayPortErrorMessage(const QString &message);

private:
    void setupSenderThread(void);
    void startSenderThread(void);
    void stopCreaterThread(void);
    void playNextAnimation(const AnimationItem *item);
private:
    QAction *m_playAction; /* MainWindow will delete this action on exit */
    QAction *m_stopAction; /* MainWindow will delete this action on exit */
    QThread *m_createThread;
    QThread *m_senderThread;
    Sender *m_sender;
    bool m_play;
    AnimationItem *m_currentAnimationItem;
    Animations *m_animations;
    AnimationPlaylistWidget* m_animationPlaylist; /* MainWindow AnimationPlaylistWidget */
    bool m_isPortOpen;
    Q_DISABLE_COPY(AnimationHandler)
};
#endif // ANIMATIONHANDLER_HPP
