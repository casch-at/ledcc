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
#include <QSerialPort>
#include <QTimer>
#include <SettingsDialog.hpp>
#include <QThread>
#include <Global.hpp>

//#define DEBUGWINDOW

/*Forward deceleration*/
//class QString;
//class QWidget;
class QListWidgetItem;
class QTimer;
//class QMultiMap;

//class ADDDIALOG;
class DebugDockWidget;
class AnimationThread;
class SendThread;
class Animations;

namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(bool timeout READ timeout WRITE setTimeout)

protected:
    void closeEvent (QCloseEvent *);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    bool timeout() const
    {
        return m_timeout;
    }

public slots:
    void setTimeout(bool arg)
    {
        m_timeout = arg;
    }

signals:
    void startAnimation(const AnimationStruct &currentAnimation);

private slots:
    void animationThreadSleep(const uint &ms);
    bool okToContinue(void);
    void resizeEvent(QResizeEvent *e);
    void saveSettings(void);
    void readSettings(void);
    void processTimeOut(void);
    void openCloseSerialPort(void);
//    void portUnpluged(void);
    void connectSignals(void);
    void createActions(void);
    void createToolbar(void);
    void clearToolButtonClicked(void);
    void about(void);
    void dataReady();
    void readData(void);
    void deleteAnimationThread(void);
#ifdef DEBUGWINDOW
    void sendData(void);
#endif
    void setDirty() { setWindowModified ( true ); }
    void updateUi(bool state);
    void playAnimationFromList(void);

    void on_availableAnimationsLW_itemDoubleClicked(QListWidgetItem *item);

    void on_animationPlaylistLW_itemDoubleClicked(QListWidgetItem *item);

private:
    void setupAnimationList(void);
    void setupThreads(void);
    void deleteThread(void);
#ifdef DEBUGWINDOW
    void writeData(const char c);
#endif
    void addToList();
    bool checkPortSettings(void);
    void closeSerialPort(bool val);
    bool openSerialPort(void);
    Ui::MainWindow *ui;

//    ADDDIALOG *adddialog;
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

    QTimer timer;
    bool m_timeout;
    QByteArray *m_data;
    AnimationStruct currentAnimation;
//    QMultiMap<QString,AnimationStruct>  *animationMap;
    QHash<QString,AnimationStruct> *playList;
    QHash<QString,AnimationStruct> *alist;
    QThread *aThread;
    QThread *sThread;
    Animations *animations;
    QSerialPort *serial;
    SendThread *sendThread;
    AnimationThread *animationThread;

};

#endif // MAINWINDOW_H
