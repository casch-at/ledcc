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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <SettingsDialog.hpp>
#include <QThread>
#include <Global.hpp>

#include "AnimationOptions.hpp"
#include <QTimer>
//#define DEBUGWINDOW

/*Forward deceleration*/
class QTimer;
class QShortcut;
class Lift;
class Animation;
class QListWidgetItem;
class Sender;
class AnimationItem;
namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent (QCloseEvent *);
public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();
Q_SIGNALS:
    void startAnimation();
    void openSerialInterface(const SettingsDialog::SerialSettings &port);
    void okClosePort();
private Q_SLOTS:
    bool okToContinue(void);
    void resizeEvent(QResizeEvent *e);
    void saveSettings(void);
    void readSettings(void);
    void openCloseSerialPort(void);
    void connectSignals(void);
    void createActions(void);
    void createToolbar(void);
    void about(void);
    void setDirty() { setWindowModified ( true ); }
    void updateUi(void);
    void playAnimations(void);
    void getNextAnimation(void);
    void animationDone(void);
    void updateAnimation(const AnimationItem *item);
    void updateItemToolTip(const AnimationOptions::Options &aOptions);
    void updateAnimationItemToolTip(QListWidgetItem *item, const AnimationOptions::Options *a);
    void portOpen(const QString &message);
    void displayPortErrorMessage(const QString &message);
    void closePort(const QString &message);
    void portClosed(const QString &message);
    void stopThreads(void);
private:
    void playNextAnimation(const AnimationItem *item);
    void setupAnimationItems(void);
    void setupSenderThread(void);
private:
    Ui::MainWindow *ui;
    SettingsDialog *sdialog;
    SettingsDialog::SerialSettings m_port;
    QToolBar *helpToolBar;
    QToolBar *mainToolBar;
    QAction *quitAction;
    QAction *clearAction;
    QAction *aboutAction;
    QAction *settingAction;
    QAction *openPortAction;
    QAction *playAction;
    QAction *pauseAction;
    QShortcut *shortCutSA;
    QThread *createThread;
    QThread *senderThread;
    Animation *currentAnimation;
    Sender *sender;
    QHash<QString,Animation*> animation;
    bool portOpened;
    bool stopPlay;
};

#endif // MAINWINDOW_H
