#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include <QListWidget>
#include "AnimationItem.hpp"

class QTimer;
class QShortcut;

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~ListWidget();

Q_SIGNALS:
    void showPropertiePreview(QListWidgetItem *item);
    void displayAnimationOptions(const AnimationOptions::Options *options);
    void updateUi(void);
public Q_SLOTS:
    void selectAllItems();
    void on_itemSelectionChanged();
    void on_showPropertiesPreviewTimerTimeout();
    void on_customContextMenuRequest(const QPoint &pos);
    void on_itemDoubleClicked(QListWidgetItem *item);
    void editItem();
    inline void focus() { setFocus(); }
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void leaveEvent(QEvent *e);
    QListWidgetItem *m_itemToShowProperties;
    QPoint m_dragStartPos;
    QPoint m_dragStopPos;

    QAction *m_editAction;
    QAction *createAction(const QString &text, const QString &tooltip = "");

private:
    void createActions();
    u_int8_t m_scrollThrough;
    QTimer *m_showPropertiesPreview;
    QListWidgetItem *itemToShowProperties;
};

#endif // LISTWIDGET_HPP
