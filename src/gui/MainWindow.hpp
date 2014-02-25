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
class SendThread;
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
private Q_SLOTS:
    bool okToContinue(void);
    void resizeEvent(QResizeEvent *e);
    void saveSettings(void);
    void readSettings(void);
    void openCloseSerialPort(void);
//    void portUnpluged(void);
    void connectSignals(void);
    void createActions(void);
    void createToolbar(void);
    void about(void);
    void readData(void);
    void setDirty() { setWindowModified ( true ); }
    void updateUi(void);
    void playAnimations(void);
    void animationDone(void);
    void sendAnimation(void);
private Q_SLOTS:
    void updateAnimation(const Draw::AnimationOptions *animationOptions);
    void updateAnimationItemToolTip(const QString &a, QListWidgetItem *item);
#ifdef DEBUGWINDOW
    void sendData(void);
#endif
private:
#ifdef DEBUGWINDOW
    void writeData(const char c);
#endif
private:
    void playNextAnimation(const QString &a);
    void setupAnimationItems(void);
    bool checkPortSettings(void);
    void closeSerialPort(void);
    bool openSerialPort(void);
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
    QSerialPort serial;
    QShortcut *shortCutSA;
    QThread *createThread;
    Animation *currentAnimation;
    QTimer timer;
    SendThread *sendThread;
    QHash<QString,Animation*> animation;
};

#endif // MAINWINDOW_H
