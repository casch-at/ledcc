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
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

// Application includes
#include "AnimationHandler.hpp"
#include "AnimationOptions.hpp"
#include "Sender.hpp"
#include "Config.hpp"
#include "HelpDialog.hpp"
#include "AboutDialog.hpp"
// ThirdParty
#include "alt_key.hpp"
#include "aqp.hpp"


// Qt includes
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenuBar>
#include <QToolBar>
#include <QByteArray>
#include <QEventLoop>
#include <QShortcut>
#include <QFileDialog>
#include <QQuickView>
#ifdef _DEBUG_
#include <QDebug>
#endif
//TODO:: Pass File path where the xml file should be stored to the XmlReaderWriter class
//       We need than only one function for default saving and user specific save location
//TODO:: Create a HTML help dialog
// Namespaces
using namespace std;
/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_helpDialog(Q_NULLPTR),
    m_focusAnimationList(new  QShortcut(QKeySequence(tr("Alt+1")),this)),
    m_focusAnimationPlaylist(new  QShortcut(QKeySequence(tr("Alt+2")),this)),
    m_scSellectAll(new  QShortcut(QKeySequence(tr("Ctrl+A")),this)),
    m_aboutMsgBox(Q_NULLPTR)
{
    m_ui->setupUi(this); // Ui must be first created befor accessing the elements
    m_ui->m_animationList->setFocus();
    m_animationHandler = new AnimationHandler(this,this);
    m_ui->m_animationPlaylist->addActions(QList<QAction*>()
                                          << m_ui->m_editAction << m_ui->m_moveUpAction << m_ui->m_moveDownAction
                                          << m_ui->m_duplicateAction << m_ui->m_removeAction << m_ui->m_clearAction
                                          << m_ui->m_selectAllAction);
    m_ui->m_animationList->addAction(m_ui->m_selectAllAction);
    addActions(QList<QAction *>()
               << m_ui->m_clearAction << m_ui->m_duplicateAction << m_ui->m_editAction
               << m_ui->m_playAction << m_ui->m_quitAction << m_ui->m_removeAction
               << m_ui->m_settingsAction << m_ui->m_stopAction << m_ui->m_moveUpAction
               << m_ui->m_moveDownAction << m_ui->m_selectAllAction);
    m_open = false;
    updateUi(false); // Should be called after we set the animationPlaylist actions
    readSettings ();
    connectSignals();
    AQP::accelerateActions(actions());
    AQP::accelerateMenu(m_ui->menuBar);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(void)
{
    delete m_animationHandler;
    delete m_ui;
}

/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent( QCloseEvent *event ) {
    if ( okToContinue() ) {
        QList<QDialog *> allDialogs = findChildren<QDialog *>();
        for(int i = 0; i < allDialogs.size(); ++i) {
            allDialogs.at(i)->close();
        }
        saveSettings ();
        event->accept();
    }
    else
        event->ignore();
}

/**
 * @brief Checks if the window was modefied
 * @return boolean
 */
bool MainWindow::okToContinue(void)
{
    if(isWindowModified ())
    {
        int r = QMessageBox::warning( this, tr( "3D-LED Cube" ),
                                      tr( "Do you really want to exit the programm?" ),
                                      QMessageBox::Yes | QMessageBox::Default,
                                      QMessageBox::Cancel | QMessageBox::Escape );

        if ( r == QMessageBox::Yes )
            return true;
        else if ( r == QMessageBox::Cancel )
            return false;
        return true;
    }
    return true;
}

/**
 * @brief MainWindow::resizeEvent
 * @param e
 */
void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
}


/*!
 \brief Save the application settings

*/
void MainWindow::saveSettings(void){
    config()->set(Settings::MainWindowGeometrySettings,saveGeometry());
    config()->set(Settings::AnimationOptionPreviewGeometry, m_ui->m_animationPropertiesPreview->saveGeometry());
    config()->set(Settings::AnimationListSplitterState, m_ui->m_animationListSplitter->saveState());
    config()->set(Settings::IsAnimationOptionPreviewHidden, m_ui->m_animationPropertiesPreview->isHidden());
    config()->set(Settings::IsAnimationToolbarHidden, m_ui->m_animationTB->isHidden());
    config()->set(Settings::IsHelpToolbarHidden, m_ui->m_helpTB->isHidden());
    config()->set(Settings::IsMainToolbarHidden, m_ui->m_mainTB->isHidden());
}

/*!
 \brief Restore the saved application settings

*/
void MainWindow::readSettings (void){
    restoreGeometry (config()->get(Settings::MainWindowGeometrySettings).toByteArray ());
    m_ui->m_animationPropertiesPreview->restoreGeometry( config()->get(Settings::AnimationOptionPreviewGeometry).toByteArray ());
    m_ui->m_animationListSplitter->restoreState(config()->get(Settings::AnimationListSplitterState).toByteArray());
    m_ui->m_animationPropertiesPreview->setHidden(config()->get(Settings::IsAnimationOptionPreviewHidden).toBool());
    m_ui->m_animationTB->setHidden(config()->get(Settings::IsAnimationToolbarHidden).toBool());
    m_ui->m_helpTB->setHidden(config()->get(Settings::IsHelpToolbarHidden).toBool());
    m_ui->m_mainTB->setHidden(config()->get(Settings::IsMainToolbarHidden).toBool());
}

/*!
 \brief Gets called when the serial port gets opened or closed and when the animation playlist gets modified.
        Configures the application action buttons according to the boolean value.

 \param portOpen
*/
void MainWindow::updateUi(bool portOpen)
{
    if (portOpen) {
        if (!m_open) {
            m_open = true;
            m_ui->m_openClosePortAction->setIcon( QIcon( "://images/disconnect.png"));
            m_ui->m_openClosePortAction->setToolTip(tr("Disconnect from seriell device  O"));
        }
    } else {
        if (m_open) {
            m_open = false;
            m_ui->m_openClosePortAction->setIcon( QIcon( "://images/connect.png"));
            m_ui->m_openClosePortAction->setToolTip(tr("Connect to seriell device  O"));
        }
    }
    updateAnimationActions();
}

void MainWindow::updateAnimationActions()
{
    Q_ASSERT(!m_ui->m_animationPlaylist->actions().isEmpty());

    if (m_open) {
        if( m_ui->m_animationPlaylist->count() && !m_ui->m_stopAction->isEnabled()) {
            m_ui->m_playAction->setEnabled(true);
            m_ui->m_stopAction->setDisabled(true);
        }
        else{
            m_ui->m_playAction->setDisabled(true);
        }
    } else {
        m_ui->m_playAction->setDisabled(true);
        m_ui->m_stopAction->setDisabled(true);
    }

    if(m_ui->m_animationPlaylist->count())
        foreach (QAction *a, m_ui->m_animationPlaylist->actions())
            a->setEnabled(true);
    else
        foreach (QAction *a, m_ui->m_animationPlaylist->actions())
            a->setDisabled(true);
}

void MainWindow::help()
{
    if (!m_helpDialog)
        m_helpDialog = new HelpDialog(this);
    m_helpDialog->show();
}

void MainWindow::savePlaylist()
{
    QFileDialog *fileDialog = new QFileDialog(this, "Save animation playlist", QString(),"XML File (*.xml)");
    QString lastPath = config()->get(Settings::LastSavePath).toString();

    fileDialog->setDirectory( lastPath.isEmpty() ? QDir::homePath() : lastPath);
    fileDialog->restoreState(config()->get(Settings::FileDialogState).toByteArray());
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDefaultSuffix("xml");

    if ( fileDialog->exec() == QDialog::Accepted ){
        QString pathToFile = fileDialog->selectedFiles().first();
        m_ui->m_animationPlaylist->saveAnimationPlaylistItemsTo(pathToFile);
        config()->set(Settings::LastSavePath,fileDialog->directory().absolutePath());
        config()->set(Settings::FileDialogState, fileDialog->saveState());
    }
    delete fileDialog;
}

void MainWindow::openPlaylist()
{
    QString lastPath = config()->get(Settings::LastOpenPath).toString();
    QFileDialog *fileDialog = new QFileDialog(this,"Open animation playlist", QString(),"*.xml" );

    fileDialog->setDirectory( lastPath.isEmpty() ? QDir::homePath() : lastPath);
    fileDialog->restoreState(config()->get(Settings::FileDialogState).toByteArray());
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setNameFilter("XML File (*.xml)");

    if ( fileDialog->exec() == QDialog::Accepted ){
        QString pathToFile = fileDialog->selectedFiles().first();
        qDebug( ) << fileDialog->selectedFiles().first();
        m_ui->m_animationPlaylist->openAnimationPlaylistFrom(pathToFile);
        config()->set(Settings::LastOpenPath,fileDialog->directory().absolutePath());
        config()->set(Settings::FileDialogState, fileDialog->saveState());
    }
    delete fileDialog;
}

/**
 * @brief Create connections
 *
 */
void MainWindow::connectSignals(void)
{

    // Overall connections
    connect( m_ui->m_openClosePortAction, &QAction::triggered, m_animationHandler ,&AnimationHandler::openCloseSerialPort);
    connect( m_ui->m_quitAction, &QAction::triggered, this,&MainWindow::close);
    connect( m_ui->m_aboutAction, &QAction::triggered,this,&MainWindow::about);
    connect( m_ui->m_settingsAction, &QAction::triggered,m_animationHandler->m_settingsDialog,&QWidget::show);
    connect( m_ui->m_stopAction, &QAction::triggered , m_animationHandler , &AnimationHandler::stopThreads);
    connect( m_ui->m_aboutQt, &QAction::triggered, this, &QApplication::aboutQt);
    connect( m_ui->m_infoAction, &QAction::triggered, this, &MainWindow::help);
    connect( m_ui->m_selectAllAction, &QAction::triggered, m_ui->m_animationPlaylist, &ListWidget::selectAllItems);
    connect( m_ui->m_selectAllAction, &QAction::triggered, m_ui->m_animationList, &ListWidget::selectAllItems);
    // ListWidgets shortcuts
    connect( m_focusAnimationList, &QShortcut::activated, m_ui->m_animationList, &ListWidget::focus);
    connect( m_focusAnimationPlaylist, &QShortcut::activated, m_ui->m_animationPlaylist, &ListWidget::focus);
    connect( m_scSellectAll, &QShortcut::activated, m_ui->m_animationList, &ListWidget::selectAllItems);
    connect( m_scSellectAll, &QShortcut::activated, m_ui->m_animationPlaylist, &ListWidget::selectAllItems);

    // ListWidget interconnections
    connect( m_ui->m_animationList , &AnimationListWidget::addToPlaylist , m_ui->m_animationPlaylist , &AnimationPlayListWidget::newItem);
    connect( m_ui->m_animationList , &AnimationListWidget::showPropertiePreview , m_ui->m_animationPropertiesPreview , &AnimationPropertiesPreview::createPropertiePreview);
    connect( m_ui->m_animationPlaylist, &AnimationPlayListWidget::contentChanged , this, &MainWindow::updateAnimationActions);
    connect( m_ui->m_animationPlaylist, &AnimationPlayListWidget::showPropertiePreview, m_ui->m_animationPropertiesPreview, &AnimationPropertiesPreview::createPropertiePreview);
    connect( m_ui->m_openAction, &QAction::triggered, this , &MainWindow::openPlaylist);
    connect( m_ui->m_saveAction, &QAction::triggered, this , &MainWindow::savePlaylist);

    // Animation Playlist action
    connect( m_ui->m_clearAction, &QAction::triggered, m_ui->m_animationPlaylist, &AnimationPlayListWidget::clearList);
    connect( m_ui->m_duplicateAction, &QAction::triggered, m_ui->m_animationPlaylist, &AnimationPlayListWidget::duplicateItems);
    connect( m_ui->m_removeAction, &QAction::triggered, m_ui->m_animationPlaylist, &AnimationPlayListWidget::removeItems);
    connect( m_ui->m_moveUpAction, &QAction::triggered, m_ui->m_animationPlaylist, &AnimationPlayListWidget::moveItemsUpDown);
    connect( m_ui->m_moveDownAction, &QAction::triggered, m_ui->m_animationPlaylist, &AnimationPlayListWidget::moveItemsUpDown);
    connect( m_ui->m_editAction, &QAction::triggered, m_ui->m_animationPlaylist, &AnimationPlayListWidget::editItem);
    connect( m_ui->m_playAction, &QAction::triggered, m_animationHandler, &AnimationHandler::playAnimations);

    connect( m_animationHandler->getSender(), &Sender::portOpenChanged, this, &MainWindow::updateUi);
    connect( m_animationHandler, &AnimationHandler::updateUi, this, &MainWindow::updateUi);
}

/**
 * @brief About the Application
 *
 */
void MainWindow::about()
{
    if (!m_aboutMsgBox) {
        m_aboutMsgBox = new AboutDialog(
                    tr("<h2>3D-LED Cube Control</h2>"),
                    QString("%1").arg(LEDCC_VERSION),
                    tr("LED Cube Control<br>"
                       "<p>Copyright &copy; 2014 Christian Schwarzgruber"
                       "<p>The <b>3D-LED Cube</b> program has been implemented with the "
                       "indention to send the created animation to a µC, without worring about "
                       "the computation power of the µC. "
                       "<p>The application is designed like a modern music player, you can start "
                       "and stop the animation, add animations to the playlist or remove it from the playlist. "
                       "Furthermore, all animation properties can be adjusted through the user interface."),
//                       "<p><address><a href=''>License: GNU General Public License</a></address>"),
                    QPixmap("://images/ledgreen.png"),
                    this);
    }
    m_aboutMsgBox->show();
}
