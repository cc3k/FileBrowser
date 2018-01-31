#include "customfilesystemmodel.h"

CustomFileSystemModel::CustomFileSystemModel(QObject *parent) :QFileSystemModel(parent)
{
    this->isRoot = true;
}

CustomFileSystemModel::~CustomFileSystemModel()
{

}

bool CustomFileSystemModel::getIsRoot() const
{
    return isRoot;
}

void CustomFileSystemModel::setIsRoot(bool value)
{
    isRoot = value;
}

QSet<QPersistentModelIndex> CustomFileSystemModel::getSelection() const
{
    return selection;
}

void CustomFileSystemModel::setSelection(const QSet<QPersistentModelIndex> &value)
{
    selection = value;
}

bool CustomFileSystemModel::selectIndex(QModelIndex &index, Qt::CheckState state)
{
    return setData(index, state, Qt::CheckStateRole);
}

bool CustomFileSystemModel::selectIndexToggle(QModelIndex &index)
{
    //qDebug() << index.data(Qt::CheckStateRole);
    if (index.data(Qt::CheckStateRole) == Qt::Checked)
    {
        return setData(index, Qt::Unchecked, Qt::CheckStateRole);
    }

    if (index.data(Qt::CheckStateRole) == Qt::Unchecked)
    {
        return setData(index, Qt::Checked, Qt::CheckStateRole);
    }
}

bool CustomFileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole)
    {
        if (value == Qt::Checked)
        {
            selection.insert(index);
            //qDebug() << index << " check";
        }

        if (value == Qt::Unchecked)
        {
            selection.remove(index);
            //qDebug() << index << "uncheck";
        }

        emit dataChanged(index, index);
        return true;
    }

    return QFileSystemModel::setData(index, value, role);
}

QVariant CustomFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!isRoot )
    {
        if (role == Qt::CheckStateRole && index.column() == 0 && index.row() > 0)
        {
            return selection.contains(index) ? Qt::Checked : Qt::Unchecked;
        }
    }
    else
    {
        if (role == Qt::CheckStateRole && index.column() == 0)
        {
            return selection.contains(index) ? Qt::Checked : Qt::Unchecked;
        }
    }

    if (role == Qt::TextColorRole)
    {
        if(selection.contains(index))
        {
            return QColor(Qt::blue);
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignBottom;
    }

    return QFileSystemModel::data(index, role);
}

Qt::ItemFlags CustomFileSystemModel::flags(const QModelIndex &index) const
{
    return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable;
}

