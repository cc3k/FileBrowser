#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableView>
#include <QKeyEvent>
#include <QDir>
#include <QFocusEvent>
#include <QItemSelectionModel>
#include <QModelIndex>

#include <QDebug>

class CustomTableView : public QTableView
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

public:
    explicit CustomTableView(QWidget *parent = 0);
    ~CustomTableView() {}


    QString getName() const;
    void setName(const QString &value);

    QString getRoot() const;
    void setRoot(const QString &value);

    QDir::Filters getFilterRoot() const;
    void setFilterRoot(const QDir::Filters &value);

    QDir::Filters getFilterBase() const;
    void setFilterBase(const QDir::Filters &value);

    QString getCurrentDir() const;
    void setCurrentDir(const QString &value);

    int getRowCount() const;
    void setRowCount(int value);

private:
    QString name;
    QString root;
    int rowCount;

    QDir::Filters filterRoot;
    QDir::Filters filterBase;

    QString currentDir;

    void selectCurrentRow();

signals:
    void keyEnter();
    void keyTab();
    void selectItem();
    void selectAllItems();
};

#endif // CUSTOMTABLEVIEW_H
