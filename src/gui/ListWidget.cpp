#include "ListWidget.hpp"
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>
#ifdef _DEBUG_
#include <QDebug>
#endif

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent),
    m_showPropertiesPreview(new QTimer)
{
    // QListWidget setup
    setDropIndicatorShown(true);
    setFocusPolicy(Qt::StrongFocus);
    setMovement(Free);
    setDragDropMode(DragDrop);

    // Change background color when hovering over item
    setMouseTracking(true);
    setStyleSheet("QListWidget::item:hover {background:lightblue;}");

    setSelectionRectVisible(true);
    setAutoScroll(true);
    setAutoScrollMargin(10);

    // Timer setup for animation properties preview
    m_showPropertiesPreview->setSingleShot(true);
    m_showPropertiesPreview->setInterval(200);

    connect( m_showPropertiesPreview, &QTimer::timeout, this, &ListWidget::on_showPropertiesPreviewTimerTimeout);
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
    QListWidgetItem *item = itemAt(e->pos());

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
    if( !items.isEmpty() && count() )
        Q_EMIT showPropertiePreview(  items.first() );
}

void ListWidget::on_showPropertiesPreviewTimerTimeout()
{
    if( m_itemToShowProperties )
        Q_EMIT showPropertiePreview( m_itemToShowProperties );
}


