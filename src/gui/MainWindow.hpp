/*************************************************************
 * 	File Name       : mainwindow.hpp
 *************************************************************
 *  Created on	 : Jan 30, 2013
 *  Author 		 : Christian Schwarzgruber
 *  Version		 : 1.0
 *************************************************************
 *  Description	 : Class MainWindow
 *************************************************************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <SettingsDialog.hpp>
#include <QThread>
#include <Global.hpp>
#include "animations/Draw.hpp"
#include <QTimer>
//#define DEBUGWINDOW

/*Forward deceleration*/
class QTimer;
class DebugDockWidget;
class QShortcut;
class Lift;
class Animation;
class QListWidgetItem;
class Sender;
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
    void updateAnimation(const Draw::AnimationOptions *animationOptions);
    void updateAnimationItemToolTip(const QString &a, QListWidgetItem *item);
    void portOpen(const QString &message);
    void displayPortErrorMessage(const QString &message);
    void closePort(const QString &message);
    void portClose(const QString &message);
    void stopAnimation(void);
private:
    void playNextAnimation(const QString &a);
    void setupAnimationItems(void);
    void setupSenderThread(void);
private:
    Ui::MainWindow *ui;
    SettingsDialog *sdialog;
    SettingsDialog::SerialSettings m_port;
    DebugDockWidget *debugDockWidget;
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
