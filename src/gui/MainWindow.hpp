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

#include "AnimationOptionsStruct.hpp"

/*Forward deceleration*/
class QTimer;
class QShortcut;
class Lift;
class Animation;
class QListWidgetItem;
class Sender;
class AnimationItem;
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
    void playAnimations(void);
    void animationDone(void);
    void updateAnimation(const AnimationItem *item);
    void updateItemToolTip(const Options &aOptions);
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
    void playNextAnimation(const AnimationItem *item);
    void setupAnimationItems(void);
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
//    QShortcut *scShowHideAnimationProperties;
    QThread *m_createThread;
    QThread *m_senderThread;
    Animation *m_currentAnimation;
    Sender *m_sender;
    QHash<QString,Animation*> m_animationHash;
    bool m_portOpened;
    bool m_stopPlay;
    AnimationListWidget *m_animationList;
    AnimationPlayListWidget *m_animationPlaylist;
    Q_DISABLE_COPY(MainWindow)
};

namespace Settings{
    const QString MainWindowGeometrySettings("geometry");
    }

namespace Animations{
    const QString WireBoxCenterShrinkGrow("Wire Box Center Shrink Grow");
    const QString WireBoxCornerShrinkGrow("Wire Box Corner Shrink Grow");
    const QString Lift("Lift");
    const QString Wall("Wall");
    const QString Firework("Firework");
    const QString Rain("Rain");
    const QString RandomSpark("Random Spark");
    const QString RandomSparkFlash("Random Spark Flash");
    const QString RandomFiller("Random Filler");
    const QString RandomZLift("Random Z-Axis Lift");
    const QString StringFly("String Fly");
    const QString Loadbar("Loadbar");
    const QString AxisNailWall("Axis Nail Wall");
    }

#endif // MAINWINDOW_H
