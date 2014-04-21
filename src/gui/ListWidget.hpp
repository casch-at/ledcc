#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include <QListWidget>




class QTimer;
class QShortcut;
class AnimationItem;

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = Q_NULLPTR);
    virtual ~ListWidget();

Q_SIGNALS:
    void showPropertiePreview(QStringList itemProperties);
    void updateUi(void);
public Q_SLOTS:
    void selectAllItems();
    void on_itemSelectionChanged();
    void on_showPropertiesPreviewTimerTimeout();
    void on_customContextMenuRequest(const QPoint &pos);
    inline void focus() { setFocus(); }
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void leaveEvent(QEvent *e);
    AnimationItem *m_itemToShowProperties;
    QPoint m_dragStartPos;
    QPoint m_dragStopPos;

    QAction *createAction(const QString &text, const QString &tooltip = "");

private:
    u_int8_t m_scrollThrough;
    QTimer *m_showPropertiesPreview;
};

#endif // LISTWIDGET_HPP
