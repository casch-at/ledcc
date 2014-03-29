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
    virtual void dropEvent(QDropEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void leaveEvent(QEvent *e);
    QListWidgetItem *m_itemToShowProperties;
public Q_SLOTS:
    void selectAllItems(void);
    void on_itemSelectionChanged();
    void on_showPropertiesPreviewTimerTimeout();
    void focus();
private:
    QTimer *m_showPropertiesPreview;
    QListWidgetItem *itemToShowProperties;
};

#endif // LISTWIDGET_HPP
