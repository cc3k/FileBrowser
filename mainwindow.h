#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QCloseEvent>
#include <QDir>
#include <QPainter>
#include <QUuid>
#include <QDateTime>
#include <QTime>
#include <QShortcut>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QTableView>
#include <QMessageBox>
#include <QSplashScreen>

#include <QDebug>

#include "customtableview.h"
#include "customfilesystemmodel.h"
#include "filesystementry.h"
#include "configxmlreader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBoxPanelLeft_currentIndexChanged(int index);
    void on_comboBoxPanelRight_currentIndexChanged(int index);
    void on_buttonRootLeft_clicked();
    void on_buttonRootRight_clicked();
    void on_buttonModeLeft_clicked();
    void on_buttonModeRight_clicked();
    void on_buttonRename_clicked();
    void on_buttonCopy_clicked();
    void on_buttonMove_clicked();
    void on_buttonCreateDir_clicked();
    void on_buttonDelete_clicked();
    void on_buttonQuit_clicked();

    void customContextMenu(const QPoint &point);

    void changeIndex(QModelIndex index);
    void findItemsLeft(QString item);
    void findItemsRight(QString item);
    void changeIndex();

    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void selectCurrentItem();
    void selectAll();

    const QString &LEFT()
    {
      static const QString constant("LEFT");
      return constant;
    }

    const QString &RIGHT()
    {
      static const QString constant("RIGHT");
      return constant;
    }

    void on_buttonSession_clicked();

private:
    Ui::MainWindow *ui;

    CustomFileSystemModel *fileModelLeft;
    CustomFileSystemModel *fileModelRight;
    QSortFilterProxyModel *proxyModelLeft;
    QSortFilterProxyModel *proxyModelRight;
    CustomTableView *panelLeft;
    CustomTableView *panelRight;

    QList<CustomTableView*> listPanel;

    QString globalRootString;
    QDir::Filters base;
    QDir::Filters root;
    QDir::Filters noRoot;

    QString sessionId;

    QList<FileSystemEntry*> fileSystemList;

    CustomTableView* activePanel() const;

    void initTableView(CustomTableView *view);
    QString generateSessionId() const;
    QString getRandomString(const int length) const;

    void delay(int msec);
};

#endif // MAINWINDOW_H
