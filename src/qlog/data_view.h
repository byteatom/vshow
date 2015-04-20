#ifndef DATA_VIEW_H
#define DATA_VIEW_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableWidgetItem>

namespace Ui {
class DataView;
}

class DataView : public QDialog
{
    Q_OBJECT

public:
    explicit DataView(QWidget *parent);
    ~DataView();
    bool canShow(QTableWidgetItem *item);

private:
    void previous();
    void next();
    bool load(QTableWidgetItem *item);
    QGraphicsScene scene;
    QGraphicsView *graph;
    QTableWidget *table;
    int row;
    int col;

    Ui::DataView *ui;
};

#endif // DATA_VIEW_H
