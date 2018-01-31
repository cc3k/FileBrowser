#ifndef CUSTOMFILESYSTEMMODEL_H
#define CUSTOMFILESYSTEMMODEL_H

#include <QObject>
#include <QSet>
#include <QPersistentModelIndex>
#include <QFileSystemModel>
#include <QDebug>

class CustomFileSystemModel : public QFileSystemModel
{
public:
    explicit CustomFileSystemModel(QObject *parent);
    ~CustomFileSystemModel();

    bool getIsRoot() const;
    void setIsRoot(bool value);

    QSet<QPersistentModelIndex> getSelection() const;
    void setSelection(const QSet<QPersistentModelIndex> &value);

    //selection
    bool selectIndex(QModelIndex &index, Qt::CheckState state);
    bool selectIndexToggle(QModelIndex &index);

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;


private:
    QSet<QPersistentModelIndex> selection;
    bool isRoot;

protected:
};

#endif // CUSTOMFILESYSTEMMODEL_H
