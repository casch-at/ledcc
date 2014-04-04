#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include <QListWidget>

class QTimer;

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~ListWidget();
Q_SIGNALS:
    void showPropertiePreview(QListWidgetItem *item);
protected:
//    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void leaveEvent(QEvent *e);
    QListWidgetItem *m_itemToShowProperties;
    QPoint m_dragStartPos;
    QPoint m_dragStopPos;
public Q_SLOTS:
    void selectAllItems(void);
    void on_itemSelectionChanged();
    void on_showPropertiesPreviewTimerTimeout();
    inline void focus() { setFocus(); }
private:
    QTimer *m_showPropertiesPreview;
    QListWidgetItem *itemToShowProperties;

};

#endif // LISTWIDGET_HPP
