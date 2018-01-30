#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox *box = new QMessageBox(this);
    box->setWindowTitle("Выход");
    box->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box->setText("Завершить сеанс и выйти из приложения?");
    box->setIcon(QMessageBox::Question);

    if (box->exec() == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sessionId = generateSessionId();

    setWindowTitle("ФПО ВКС. Идентификатор сессии : [" + sessionId + "]");
    setWindowState(Qt::WindowMaximized);
    //setWindowModality(Qt::ApplicationModal); //чтобы не переходил фокус при поп-апах
    setWindowIcon(QIcon(":/images/transfer.png"));

    QShortcut *shortcutF10 = new QShortcut(QKeySequence (Qt::Key_F10), this);
    QObject::connect(shortcutF10, SIGNAL(activated()), ui->buttonQuit, SLOT(click()));

    QShortcut *shortcutF11 = new QShortcut(QKeySequence (Qt::Key_F11), this);
    QObject::connect(shortcutF11, SIGNAL(activated()), ui->buttonSession, SLOT(click()));

    globalRootString = QDir::currentPath();
    base = QDir::AllEntries | QDir::NoSymLinks;
    root = base | QDir::NoDotAndDotDot;
    noRoot = base | QDir::NoDot;

    fileModelLeft = new QFileSystemModel;
    fileModelLeft->setReadOnly(true);
    fileModelLeft->setFilter(root);
    proxyModelLeft = new QSortFilterProxyModel;
    proxyModelLeft->setSourceModel(fileModelLeft);
    connect(fileModelLeft,SIGNAL(directoryLoaded(QString)),this,SLOT(findItemsLeft(QString)));

    //fileModelLeft->sort(0,Qt::AscendingOrder); //вроде ничего не поменялось

    fileModelRight = new QFileSystemModel;
    fileModelRight->setReadOnly(true);
    fileModelRight->setFilter(root);
    proxyModelRight = new QSortFilterProxyModel;
    proxyModelRight->setSourceModel(fileModelRight);
    connect(fileModelLeft,SIGNAL(directoryLoaded(QString)),this,SLOT(findItemsRight(QString)));

    //fileModelRight->sort(0,Qt::AscendingOrder);

    ui->groupBoxLeft->setTitle(fileModelLeft->fileInfo(fileModelLeft->setRootPath(globalRootString)).absoluteFilePath());
    ui->groupBoxRight->setTitle(fileModelLeft->fileInfo(fileModelLeft->setRootPath(globalRootString)).absoluteFilePath());

    panelLeft = new CustomTableView;
    panelLeft->setName(LEFT());
    //panelLeft->setRoot(globalRootString); //тоже вроде ничего не поменялось
    panelLeft->setModel(proxyModelLeft);
    panelLeft->setRootIndex(proxyModelLeft->mapFromSource(fileModelLeft->setRootPath(globalRootString)));

    panelRight = new CustomTableView;
    panelRight->setName(RIGHT());
    //panelRight->setRoot(globalRootString);
    panelRight->setModel(proxyModelRight);
    panelRight->setRootIndex(proxyModelRight->mapFromSource(fileModelRight->setRootPath(globalRootString)));

    //взять из конфига
    ConfigXmlReader *config = new ConfigXmlReader(":/config/stations.xml");
    fileSystemList = config->getFileSystemEntryList();
    qDebug() << "parse configuration for: " << fileSystemList.size() << " entries";
    for (int i = 0; i < fileSystemList.size(); i++)
    {
        ui->comboBoxPanelLeft->addItem(fileSystemList.at(i)->getName());
        ui->comboBoxPanelRight->addItem(fileSystemList.at(i)->getName());
    }
    config->deleteLater();

    connect(panelLeft, SIGNAL(keyTab()), panelRight, SLOT(setFocus()));
    connect(panelRight, SIGNAL(keyTab()), panelLeft, SLOT(setFocus()));

    initTableView(panelLeft);
    initTableView(panelRight);

    listPanel.append(panelLeft);
    listPanel.append(panelRight);

    ui->groupBoxLeft->layout()->addWidget(panelLeft);
    ui->groupBoxRight->layout()->addWidget(panelRight);

    QPixmap pix(":/images/transfer.png");
    QSplashScreen splashScreen(pix);
    splashScreen.show();

    delay(500);
    //panelLeft->selectionModel()->setCurrentIndex(pr);
    //qDebug() << panelLeft->getRowCount();
    panelLeft->selectionModel()->setCurrentIndex(panelLeft->indexAt(QPoint(0,0)),QItemSelectionModel::NoUpdate);
    panelRight->selectRow(0);
    panelRight->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTableView(CustomTableView *view)
{
    view->setColumnHidden(2, true);
    view->verticalHeader()->hide();
    view->setShowGrid(false);
    view->horizontalHeader()->setStretchLastSection(true);
    view->setColumnWidth(0, this->width() / 2);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setAlternatingRowColors(true);
    view->horizontalHeader()->setHighlightSections(false);
    //view->setIconSize(QSize(32, 32));

    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenu(const QPoint &)));
    connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(changeIndex(QModelIndex)));
    connect(view, SIGNAL(keyEnter()), this, SLOT(changeIndex()));
    connect(view->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

CustomTableView *MainWindow::activePanel() const
{
    for(int i = 0; i < listPanel.size(); i++)
    {
        if (listPanel.at(i)->hasFocus())
        {
            return listPanel.at(i);
        }
    }

    return 0;
}

void MainWindow::on_comboBoxPanelLeft_currentIndexChanged(int index)
{
    QString ro = "Режим: RO";
    QString rw = "Режим: RW";

    qDebug() << "left panel init for " << fileSystemList.at(index)->getName();

    panelLeft->setRoot(fileSystemList.at(index)->getMount());
    fileModelLeft->setReadOnly(fileSystemList.at(index)->getIsReadOnly());
    panelLeft->setFilterBase(fileSystemList.at(index)->getFilterBase());
    panelLeft->setFilterRoot(fileSystemList.at(index)->getFilterRoot());

    //qDebug() << panelLeft->getFilterBase();
    //qDebug() << panelLeft->getFilterRoot();

    if (!fileModelLeft->isReadOnly())
    {
        ui->buttonModeLeft->setText(rw);
    }

    if (fileModelLeft->isReadOnly())
    {
        ui->buttonModeLeft->setText(ro);
    }

    fileModelLeft->setFilter(panelLeft->getFilterRoot());

    ui->groupBoxLeft->setTitle(panelLeft->getRoot());
    panelLeft->setRootIndex(proxyModelLeft->mapFromSource(fileModelLeft->setRootPath(panelLeft->getRoot())));

    //так можно выбрать объект по известному пути
    //panelLeft->selectionModel()->setCurrentIndex(proxyModelLeft->mapFromSource(fileModelLeft->index("/home/projekt/Workspace")), QItemSelectionModel::Select);

    //фирменная вещь!!! а я до этого городил огород самостоятельно с стрингом и каррентДир
    //fileModelLeft->rootDirectory();


    //qDebug() << "items: " << panelLeft->getRowCount();

}

void MainWindow::on_comboBoxPanelRight_currentIndexChanged(int index)
{
    QString ro = "Режим: RO";
    QString rw = "Режим: RW";

    qDebug() << "right panel init for " << fileSystemList.at(index)->getName();

    panelRight->setRoot(fileSystemList.at(index)->getMount());
    fileModelRight->setReadOnly(fileSystemList.at(index)->getIsReadOnly());
    panelRight->setFilterBase(fileSystemList.at(index)->getFilterBase());
    panelRight->setFilterRoot(fileSystemList.at(index)->getFilterRoot());


    if (!fileModelRight->isReadOnly())
    {
        ui->buttonModeRight->setText(rw);
    }

    if (fileModelRight->isReadOnly())
    {
        ui->buttonModeRight->setText(ro);
    }

    fileModelRight->setFilter(panelRight->getFilterRoot());

    ui->groupBoxRight->setTitle(panelRight->getRoot());
    panelRight->setRootIndex(proxyModelRight->mapFromSource(fileModelRight->setRootPath(panelRight->getRoot())));
}

void MainWindow::on_buttonRootLeft_clicked()
{
    ui->groupBoxLeft->setTitle(panelLeft->getRoot());
    panelLeft->setRootIndex(proxyModelLeft->mapFromSource(fileModelLeft->setRootPath(panelLeft->getRoot())));
    fileModelLeft->setFilter(root);
}

void MainWindow::on_buttonRootRight_clicked()
{
    ui->groupBoxRight->setTitle(panelRight->getRoot());
    panelRight->setRootIndex(proxyModelRight->mapFromSource(fileModelRight->setRootPath(panelRight->getRoot())));
    fileModelRight->setFilter(root);
}

void MainWindow::on_buttonModeLeft_clicked()
{
    QString ro = "Режим: RO";
    QString rw = "Режим: RW";

    if (fileModelLeft->isReadOnly() == true)
    {
        fileModelLeft->setReadOnly(false);
        ui->buttonModeLeft->setText(rw);
        qDebug() << "left panel set RW";
    }
    else
    {
        fileModelLeft->setReadOnly(true);
        ui->buttonModeLeft->setText(ro);
        qDebug() << "left panel set RO";
    }
}

void MainWindow::on_buttonModeRight_clicked()
{
    QString ro = "Режим: RO";
    QString rw = "Режим: RW";

    if (fileModelRight->isReadOnly() == true)
    {
        fileModelRight->setReadOnly(false);
        ui->buttonModeRight->setText(rw);
        qDebug() << "right panel set RW";

    }
    else
    {
        fileModelRight->setReadOnly(true);
        ui->buttonModeRight->setText(ro);
        qDebug() << "right panel set RO";
    }
}

void MainWindow::on_buttonRename_clicked()
{

}

void MainWindow::on_buttonCopy_clicked()
{
    qDebug() << "copy request for:" << activePanel()->getName();
    qDebug() << "items to copy: " << activePanel()->selectionModel()->selectedRows().size();

    QModelIndexList list = activePanel()->selectionModel()->selectedRows();

    QFileSystemModel *model;
    QSortFilterProxyModel *proxy;

    proxy = dynamic_cast<QSortFilterProxyModel*> (activePanel()->model());
    model = dynamic_cast<QFileSystemModel*> (proxy->sourceModel());

    for (int i = 0; i < list.size(); i++)

    {
        qDebug() << list.at(i).data(Qt::DisplayRole);
        qDebug() << "at row: " << list.at(i).row();
    }
}

void MainWindow::on_buttonMove_clicked()
{

}

void MainWindow::on_buttonCreateDir_clicked()
{

}

void MainWindow::on_buttonDelete_clicked()
{

}

void MainWindow::on_buttonQuit_clicked()
{
    QMessageBox *box = new QMessageBox(this);
    box->setWindowTitle("Выход");
    box->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box->setText("Завершить сеанс и выйти из приложения?");
    box->setIcon(QMessageBox::Question);

    if (box->exec() == QMessageBox::Yes)
    {
        qApp->quit();
    }
    else
    {
        return;
    }
}

void MainWindow::customContextMenu(const QPoint &point)
{
    Q_UNUSED (point)

    if (activePanel()->currentIndex().isValid())
    {
        QMenu menu;
        QAction *sendItem = new QAction("Отправить", this);
        QAction *cutItem = new QAction("Вырезать", this);
        QAction *copyItem = new QAction("Копировать", this);
        QAction *deleteItem = new QAction("Удалить", this);
        QAction *renameItem = new QAction("Переименовать", this);
        QAction *propertyItem = new QAction("Свойства", this);

        connect(copyItem, SIGNAL( triggered() ), SLOT(on_buttonCopy_clicked()));

        menu.addAction(sendItem);
        menu.addSeparator();
        menu.addAction(cutItem);
        menu.addAction(copyItem);
        menu.addSeparator();
        menu.addAction(deleteItem);
        menu.addAction(renameItem);
        menu.addSeparator();
        menu.addAction(propertyItem);

        menu.exec(QCursor::pos());

        copyItem->deleteLater();
    }
}

void MainWindow::changeIndex(QModelIndex index)
{
    QFileSystemModel *model;
    QSortFilterProxyModel *proxy;

    proxy = dynamic_cast<QSortFilterProxyModel*> (activePanel()->model());
    model = dynamic_cast<QFileSystemModel*> (proxy->sourceModel());

    if (!index.isValid() || !model->fileInfo(proxy->mapToSource(index)).isDir())
    {
        return;
    }
    QString currentPath = model->fileInfo(proxy->mapToSource(index.parent())).absoluteFilePath();
    QString indexPath = model->fileInfo(proxy->mapToSource(index)).absoluteFilePath();
    QString currentDir = currentPath.split("/").last();

    if (indexPath == activePanel()->getRoot())
    {
        model->setFilter(QDir::NoFilter);
        model->setFilter(activePanel()->getFilterRoot());
    }
    else
    {
        model->setFilter(QDir::NoFilter);
        model->setFilter(activePanel()->getFilterBase());
    }

    activePanel()->setRootIndex(proxy->mapFromSource(model->setRootPath(indexPath)));
    activePanel()->setCurrentDir(indexPath);

    //проверка на вхождение на уровень ниже или выше
    qDebug() << "leave current dir: " << currentPath;
    //qDebug() << "current dir name: " << currentDir;
    qDebug() << "enter dir: " << indexPath;

    if((indexPath + "/" + currentDir) == currentPath)
    {
        qDebug() << "dir up";
        delay(10);
        panelLeft->selectionModel()->setCurrentIndex(proxyModelLeft->mapFromSource(fileModelLeft->index(currentPath)), QItemSelectionModel::Select);

    }
    else
    {
        qDebug() << "dig in";
        delay(10);
        activePanel()->selectRow(0);
    }

    //отрисовка в заголовке текущего пути
    if (activePanel()->getName() == RIGHT())
    {
        ui->groupBoxRight->setTitle(indexPath);
    }

    if (activePanel()->getName() == LEFT())
    {
        ui->groupBoxLeft->setTitle(indexPath);
    }
}

void MainWindow::findItemsLeft(QString item)
{
    //qDebug() << "left panel loaded " << fileModelLeft->rowCount(fileModelLeft->index(item));
    panelLeft->setRowCount(fileModelLeft->rowCount(fileModelLeft->index(item)));
}

void MainWindow::findItemsRight(QString item)
{
    //qDebug() << "right panel loaded " << fileModelRight->rowCount(fileModelLeft->index(item));
    panelRight->setRowCount(fileModelRight->rowCount(fileModelRight->index(item)));
}

void MainWindow::changeIndex()
{
    changeIndex(activePanel()->currentIndex());
}

void MainWindow::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //qDebug() << "items: " << panelLeft->getRowCount();
    //qDebug() << activePanel()->getName() << " index from " << previous << "to" << current;
    //qDebug() << activePanel()->getName() << " data from  " << previous.data(Qt::DisplayRole) << "to" << current.data(Qt::DisplayRole);
    //qDebug() << activePanel()->getName() << " row from   " << previous.row() << "to" << current.row();
}

QString MainWindow::getRandomString(const int length) const
{
    const QString chars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    QTime stamp = QTime::currentTime();
    qsrand(stamp.msec());

    for(int i=0; i<length; ++i)
    {
        int index = qrand() % chars.length();
        QChar nextChar = chars.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

void MainWindow::delay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }
}

QString MainWindow::generateSessionId() const
{
    QString sessionId = QDateTime::currentDateTime().toString("yyMMdd_HHmmss_") + getRandomString(4);
    return sessionId;
}

void MainWindow::on_buttonSession_clicked()
{
    QMessageBox *box = new QMessageBox(this);
    box->setWindowTitle("Сессия");
    box->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box->setText("Текущий идентификатор: " + sessionId + "\nСгенерировать новый?");
    box->setIcon(QMessageBox::Question);

    if (box->exec() == QMessageBox::Yes)
    {
        sessionId = generateSessionId();
        setWindowTitle("ФПО ВКС. Идентификатор сессии : [" + sessionId + "]");
    }
}
