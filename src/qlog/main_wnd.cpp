#include "main_wnd.h"
#include "ui_main_wnd.h"

#include <QTcpSocket>
#include <QVariant>

#include "data_view.h"

MainWnd::MainWnd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWnd),
    currentFinded(0)
{
    ui->setupUi(this);

    ui->table->setColumnWidth(0, 64);
    ui->table->setColumnWidth(1, 64);
    ui->table->setColumnWidth(3, 32);

    log.len = 0;

    connect(ui->clear, &QPushButton::clicked, this, &MainWnd::clear);
    connect(ui->bottom, &QPushButton::clicked, this, &MainWnd::bottom);
    connect(ui->top, &QPushButton::clicked, this, &MainWnd::top);
    connect(ui->filter, &QLineEdit::textChanged, this, &MainWnd::filterChanged);
    connect(ui->find, &QPushButton::clicked, this, &MainWnd::find);
    connect(ui->table, &QTableWidget::itemDoubleClicked, this, &MainWnd::showData);
    connect(&server, &QTcpServer::newConnection, this, &MainWnd::accept);

    server.listen(QHostAddress::Any, 34567);
}

MainWnd::~MainWnd()
{
    delete ui;
}

void MainWnd::accept()
{
    QTcpSocket *client = server.nextPendingConnection();
    if (client) {
        connect(client, &QTcpSocket::readyRead, this, &MainWnd::read);
    }
}

void MainWnd::read()
{
    QTcpSocket *client = static_cast<QTcpSocket *>(sender());
    if (!client) return;

    while (client->bytesAvailable() > sizeof(quint32)) {
        stream.setDevice(client);
        if (0 == log.len) {
            log.readLen(stream);
        }
        if (0 == log.len)
            continue;
        if (client->bytesAvailable() < log.len)
            break;
        log.readElse(stream);
        log.len = 0;
        if (!ui->pause->isChecked())
            addLog();
    }
}

void MainWnd::addLog()
{
    int row = ui->table->rowCount();
    ui->table->insertRow(row);
    QTableWidgetItem *item;

    item= new QTableWidgetItem(QString::number(log.processId));
    ui->table->setItem(row, COL_PROCESS, item);

    item = new QTableWidgetItem(QString::number(log.threadId));
    ui->table->setItem(row, COL_THREAD, item);

    QString stamp = QString::number(log.milSecHead);
    if (log.milSecTail > log.milSecHead)
        stamp.append("+").append(QString::number(log.milSecTail - log.milSecHead));
    item = new QTableWidgetItem(stamp);
    ui->table->setItem(row, COL_STAMP, item);

    item = new QTableWidgetItem(QString::number(log.level));
    ui->table->setItem(row, COL_LEVEL, item);

    item = new VarItem(log.var);
    ui->table->setItem(row, COL_DATA, item);

    item = new QTableWidgetItem(log.text);
    ui->table->setItem(row, COL_TEXT, item);

    if (ui->scroll->isChecked())
        ui->table->scrollToBottom();
}

void MainWnd::clear()
{
    ui->table->setRowCount(0);
    finded.clear();
    currentFinded = 0;
}

void MainWnd::bottom()
{
    ui->table->scrollToBottom();
}

void MainWnd::top()
{
    ui->table->scrollToTop();
}

void MainWnd::filterChanged(const QString &text)
{
    finded.clear();
    currentFinded = 0;
}

void MainWnd::find()
{
    if (finded.empty()) {
        finded = ui->table->findItems(ui->filter->text(), Qt::MatchContains);
        currentFinded = 0;
    }

    if (finded.empty()) return;

    ++currentFinded;
    if (currentFinded == finded.size()) currentFinded = 0;

    ui->table->setCurrentItem(finded[currentFinded]);
}

void MainWnd::showData(QTableWidgetItem *item)
{
    if (MainWnd::COL_DATA != item->column()) return;

    DataView *view = new DataView(this);
    if (view->canShow(item))
        view->show();
    else
        delete view;
}
