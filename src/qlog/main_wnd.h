#ifndef MAIN_UI_H
#define MAIN_UI_H

#include <QDialog>
#include <QTcpServer>
#include <QList>

#include "qlog.h"
#include "var_item.h"

namespace Ui {
class MainWnd;
}

class MainWnd : public QDialog
{
    Q_OBJECT

public:
    enum {
        COL_PROCESS,
        COL_THREAD,
        COL_STAMP,
        COL_LEVEL,
        COL_DATA,
        COL_TEXT
    };

    explicit MainWnd(QWidget *parent = 0);
    ~MainWnd();

private:
    void accept();
    void read();
    void addLog();
    void clear();
    void bottom();
    void top();
    void filterChanged(const QString &text);
    void find();
    void showData(QTableWidgetItem *item);

    QTcpServer server;
    QLog log;
    QString text;
    QDataStream stream;
    QList<QTableWidgetItem *> finded;
    int currentFinded;

    Ui::MainWnd *ui;
};

#endif // MAIN_UI_H
