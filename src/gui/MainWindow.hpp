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
#include <QTimer>


/*Forward deceleration*/
class QTimer;
class QShortcut;
class Lift;
namespace animations {
    class Animation;
    }

class Sender;
class AnimationListWidget;
class AnimationPlayListWidget;

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
    virtual ~MainWindow();
Q_SIGNALS:
    void startAnimation();
    void openSerialInterface(const SettingsDialog::SerialSettings &port);
    void okClosePort();
private Q_SLOTS:
    void resizeEvent(QResizeEvent *e);
    void openCloseSerialPort(void);
    void about(void);
    void setDirty() { setWindowModified ( true ); }
    void updateUi(void);

    void portOpen(const QString &message);
    void displayPortErrorMessage(const QString &message);
    void closePort(const QString &message);
    void portClosed(const QString &message);
    void stopThreads(void);
    void showPropertiesPreview(QListWidgetItem *item);

private:
    bool okToContinue(void);
    void readSettings(void);
    void saveSettings(void);
    void connectSignals(void);
    void createActions(void);
    void createToolbar(void);
    void setupSenderThread(void);
private:
    Ui::MainWindow *ui;
    SettingsDialog *m_sdialog;
    SettingsDialog::SerialSettings m_port;
    QToolBar *m_helpToolBar;
    QToolBar *m_mainToolBar;
    QToolBar *m_animationToolBar;
    QAction *m_quitAction;
    QAction *m_aboutAction;
    QAction *m_settingAction;
    QAction *m_openPortAction;
    QAction *m_aboutQt;
    QShortcut *m_focusAnimationList;
    QShortcut *m_focusAnimationPlaylist;
    QShortcut *m_scSellectAll;
    QThread *m_createThread;
    QThread *m_senderThread;
    Sender *m_sender;
    bool m_portOpened;
    bool m_stopPlay;
    AnimationListWidget *m_animationList;
    AnimationPlayListWidget *m_animationPlaylist;
    Q_DISABLE_COPY(MainWindow)
};

namespace Settings{
    const QString SMainWindowGeometrySettings("geometry");
    }



#endif // MAINWINDOW_H
