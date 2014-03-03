#include "AnimationPlayListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>

AnimationPlayListWidget::AnimationPlayListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);

}

void AnimationPlayListWidget::clearList()
{
    clear();
    emit updateUi();
}

void AnimationPlayListWidget::newItem(QList<QListWidgetItem *> item)
{
    foreach (QListWidgetItem *i, item) {
        addItem(i->clone());
    }
    emit updateUi();
}

void AnimationPlayListWidget::keyPressEvent(QKeyEvent *event)
{
    int cRow = -1;

    switch (event->key()) {
    case Qt::Key_Delete:
        foreach(QListWidgetItem *i,selectedItems())
            delete i;
        if(count())
            setCurrentRow(currentRow());
        emit updateUi();
        break;
    case Qt::Key_Up:
        cRow = currentRow();
        if(cRow == 0)
            setCurrentRow(count()-1);
        else
            setCurrentRow(cRow-1);
        break;
    case Qt::Key_Down:
        cRow = currentRow();
        qDebug()<< "Rows: " << count();
        if(cRow == count()-1)
            setCurrentRow(0);
        else
            setCurrentRow(cRow+1);
        break;
        //    case Qt::Key_Return:
        //        break;
        //    case Qt::Key_Return:
        //        break;
    case Qt::Key_Escape:
        for(int i=0;i < count();i++){
            if(item(i)->isSelected())
                setItemSelected(item(i),false);
        }
        break;
    default:
        break;
    }

}

void AnimationPlayListWidget::selectAllItems(void)
{
    if(hasFocus())
        selectAll();
}


AnimationItem *AnimationPlayListWidget::getNextAnimation()
{
    static int row;
    if(row >= count())
        row=0;
    return dynamic_cast<AnimationItem*>(item(row++));
}
