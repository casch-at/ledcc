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

//#define DEBUGWINDOW

/*Forward deceleration*/
class QListWidgetItem;
class QTimer;
class DebugDockWidget;
class AnimationThread;
class Animations;

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
    void startAnimation(const AnimationStruct &currentAnimation);
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
    void playAnimationFromList(void);
    void on_applyPB_clicked();
private Q_SLOTS:
#ifdef DEBUGWINDOW
    void sendData(void);
#endif
private:
#ifdef DEBUGWINDOW
    void writeData(const char c);
#endif
private:
    void setupAnimationList(void);
    void addToList();
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
    AnimationStruct currentAnimation;
    QHash<QString,AnimationStruct> *playList;
    QHash<QString,AnimationStruct> *alist;
    Animations *animations;
    QSerialPort serial;

};

#endif // MAINWINDOW_H
