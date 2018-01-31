#include "customtableview.h"

CustomTableView::CustomTableView(QWidget *parent) : QTableView(parent)
{
    root = "EMPTY";
    name = "EMPTY";
    rowCount = 0;
}

QString CustomTableView::getName() const
{
    return name;
}

void CustomTableView::setName(const QString &value)
{
    name = value;
}

QString CustomTableView::getRoot() const
{
    return root;
}

void CustomTableView::setRoot(const QString &value)
{
    root = value;
}

QDir::Filters CustomTableView::getFilterRoot() const
{
    return filterRoot;
}

void CustomTableView::setFilterRoot(const QDir::Filters &value)
{
    filterRoot = value;
}

QDir::Filters CustomTableView::getFilterBase() const
{
    return filterBase;
}

void CustomTableView::setFilterBase(const QDir::Filters &value)
{
    filterBase = value;
}

QString CustomTableView::getCurrentDir() const
{
    return currentDir;
}

void CustomTableView::setCurrentDir(const QString &value)
{
    currentDir = value;
}

int CustomTableView::getRowCount() const
{
    return rowCount;
}

void CustomTableView::setRowCount(int value)
{
    rowCount = value;
}

void CustomTableView::selectCurrentRow()
{
    qDebug() << currentIndex().data(Qt::DisplayRole);
    qDebug() << "selectRow";
}

void CustomTableView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    
    case Qt::Key_Return:
    {
        emit keyEnter();
        break;
    }

    case Qt::Key_Tab:
    {
        emit keyTab();
        break;
    }

    case Qt::Key_Space:
    case Qt::Key_Insert:
    {
        emit selectItem();
        break;
    }

    default:
        QTableView::keyPressEvent(event);
        break;
    }

}

void CustomTableView::focusInEvent(QFocusEvent *event)
{
    selectRow(currentIndex().row());
    QTableView::focusInEvent(event);
}

void CustomTableView::focusOutEvent(QFocusEvent *event)
{
    clearSelection();
    QTableView::focusOutEvent(event);
}
