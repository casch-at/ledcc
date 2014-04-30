#include "ListWidget.hpp"
#include "AnimationItem.hpp"

#include <QTimer>
#include <QKeyEvent>
#include <QApplication>
#include <QShortcut>
#include <QMenu>
#include <QAction>
#include <Animations.hpp>

#ifdef _DEBUG_
#include <QDebug>
#endif

ListWidget::ListWidget(QWidget *parent):
    QListWidget(parent),
    m_scrollThrough(0),
    m_showPropertiesPreview(new QTimer)
{
    // QListWidget setup
    setFocusPolicy(Qt::StrongFocus);

    // Change background color when hovering over item
    setMouseTracking(true);
    setStyleSheet("QListWidget::item:hover {background:lightblue;}");

    setSelectionRectVisible(true);
    setAutoScroll(true);
    setAutoScrollMargin(10);
    setSelectionBehavior(SelectItems);
    setSelectionMode(ExtendedSelection);
    setAlternatingRowColors(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    // Timer setup for animation properties preview
    m_showPropertiesPreview->setSingleShot(true);
    m_showPropertiesPreview->setInterval(200);

    connect( m_showPropertiesPreview, &QTimer::timeout, this, &ListWidget::on_showPropertiesPreviewTimerTimeout);
    connect( this, &QListWidget::customContextMenuRequested, this, &ListWidget::on_customContextMenuRequest);
    connect( this, &QListWidget::itemSelectionChanged, this, &ListWidget::on_itemSelectionChanged);

}

ListWidget::~ListWidget()
{
    delete m_showPropertiesPreview;
}



void ListWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
}


void ListWidget::mouseMoveEvent(QMouseEvent *e)
{

    if( !(e->buttons()) & Qt::LeftButton
            && (e->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance() ){
        e->ignore();
        return;
    }
    AnimationItem *item = dynamic_cast<AnimationItem*>(itemAt(e->pos()));

    if( item )
    {
        if( item != m_itemToShowProperties )
        {
            m_itemToShowProperties = item;
        }
        m_showPropertiesPreview->start();
    }
    else
    {
        m_showPropertiesPreview->stop();
    }

    QListWidget::mouseMoveEvent(e);
}

void ListWidget::leaveEvent(QEvent *e)
{
    m_showPropertiesPreview->stop();
    e->accept();
}

void ListWidget::selectAllItems()
{
    if( hasFocus() )
        selectAll();
}

void ListWidget::on_itemSelectionChanged()
{
    QList<QListWidgetItem*> items = selectedItems();
    if( !items.isEmpty() && count() ){
        AnimationItem *item = dynamic_cast<AnimationItem*>(items.first());
        Q_EMIT showPropertiePreview( item->getAnimationPropertiesAsPlainText() );
    }
}

void ListWidget::on_showPropertiesPreviewTimerTimeout()
{
    if( m_itemToShowProperties )
        Q_EMIT showPropertiePreview( m_itemToShowProperties->getAnimationPropertiesAsPlainText() );
}

void ListWidget::on_customContextMenuRequest(const QPoint &pos)
{
    QMenu menu(this);
    menu.addActions(actions());
    menu.exec(viewport()->mapToGlobal(pos));
}



QAction *ListWidget::createAction(const QString &text, const QString &tooltip)
{
    QAction *action = new QAction(text,this);
    action->setToolTip(tooltip);
    action->setStatusTip(tooltip);
    return action;
}


void ListWidget::keyPressEvent(QKeyEvent *e)
{
    QModelIndex index;

    switch (e->key()) {
    case Qt::Key_Up:
        index = moveCursor( QAbstractItemView::MoveUp, e->modifiers() );

        if( index.row() > 0)
            m_scrollThrough = 0;

        if ( index.row()  == 0 ){
            m_scrollThrough++;

            switch (m_scrollThrough)
            {
            case 1:
                setCurrentRow( 0, selectionCommand(index,dynamic_cast<QEvent*>(e)) );
                break;
            case 2:
                setCurrentRow( count() - 1, selectionCommand(index,dynamic_cast<QEvent*>(e)) );
                m_scrollThrough = 0;
                break;
            default:
                break;
            }
        }
        else{
            setCurrentRow( index.row(), selectionCommand(index,dynamic_cast<QEvent*>(e)) );
        }
        break;
    case Qt::Key_Down:
        index = moveCursor( QAbstractItemView::MoveDown, e->modifiers() );
        if (index.row() + 1 < count())
            m_scrollThrough = 0;

        if( index.row() + 1 == count() ){
            m_scrollThrough++;

            switch (m_scrollThrough)
            {
            case 1:
                setCurrentRow( count() -1, selectionCommand(index,dynamic_cast<QEvent*>(e)) );
                break;
            case 2:
                setCurrentRow( 0, selectionCommand(index,dynamic_cast<QEvent*>(e)) );
                m_scrollThrough = 0;
                break;
            default:
                break;
            }
        }
        else{
            setCurrentRow( index.row(), selectionCommand(index,dynamic_cast<QEvent*>(e)) );
        }

        break;
    case Qt::Key_Escape:
        for(int i=0;i < count();i++){
            setItemSelected(item(i),false);
        }
        break;
    }
}



