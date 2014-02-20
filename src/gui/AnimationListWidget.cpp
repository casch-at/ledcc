#include "AnimationListWidget.hpp"
#include <QKeyEvent>
#include <QDebug>
//#include <QtWidgets/QAction>

AnimationListWidget::AnimationListWidget(QWidget *parent):
    QListWidget(parent)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText("Lift");
    item->setToolTip("Available properties \n\nSpeed (default=80)\nDelay (default=50)\nIterations (default=10) ");
//    addItem("Lift");
    addItem(item);
    addItem("String Fly");
    addItem("Random Spark Flash");
    addItem("Random Spark");
    addItem("Random Filler");
    addItem("Loadbar");
    addItem("Axis Nail Wall");
    addItem("Wire Box Center Shrink Grow");
    addItem("Wire Box Corner Shrink Grow");
    addItem("Random Z-Axis Lift");
    addItem("Rain");
    addItem("Wall");
    addItem("Firework");
    setFocusPolicy(Qt::StrongFocus);
}

void AnimationListWidget::keyPressEvent(QKeyEvent *event)
{
//    qDebug() << "Event key is: " << QString::number(event->key(),16)
//             << "Qt::Key_Enter is: " << QString::number(Qt::Key_Enter,16);
    int cRow = -1;

    switch (event->key()) {
    case Qt::Key_Enter:
        emit itemsSelected(selectedItems());
        break;
    case Qt::Key_Return:
        emit itemsSelected(selectedItems());
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
        if(cRow == count()-1)
            setCurrentRow(0);
        else
            setCurrentRow(cRow+1);
        break;
    case Qt::Key_Escape:
        for(int i=0;i < count();i++){
            setItemSelected(item(i),false);
        }
        break;
    case Qt::Key_Space:
        item(currentIndex().row())->setSelected(true);
        break;
    default:
        break;
    }
}

void AnimationListWidget::selectAllItems()
{
    if(hasFocus())
        selectAll();
}

void AnimationListWidget::insertAnimation(const QString &animation)
{
    addItem(animation);
}


