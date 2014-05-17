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

class QShortcut;
class AnimationHandler;
class HelpDialog;
class AboutDialog;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent (QCloseEvent *);
    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *e);
public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();
Q_SIGNALS:
private Q_SLOTS:
    void about(void);
    void setDirty() { setWindowModified ( true ); }
    void updateUi(bool portOpen);
    void updateAnimationActions();
    void help();
    void savePlaylist();
    void openPlaylist();
private:
    bool okToContinue(void);
    void readSettings(void);
    void saveSettings(void);
    void connectSignals(AnimationHandler *animationHandler);
private:
    Ui::MainWindow *m_ui;
    HelpDialog *m_helpDialog;
    QThread *m_animationHandlerThread;
    QShortcut *m_focusAnimationList;
    QShortcut *m_focusAnimationPlaylist;
    QShortcut *m_scSellectAll;
    bool m_open; /* Port open or not */
    Q_DISABLE_COPY(MainWindow)
};
#endif // MAINWINDOW_H
