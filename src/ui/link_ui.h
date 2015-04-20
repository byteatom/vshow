#ifndef LINKUI_H
#define LINKUI_H

#include <QDialog>
#include <QButtonGroup>

#include "data/sender_card.h"
#include "link_item.h"

namespace Ui {
class LinkUi;
}

class LinkUi : public QDialog {
    Q_OBJECT

public:
    typedef void (LinkUi::*StyleOp)(QPoint &pos);
    struct StyleOps {
        StyleOp init;
        StyleOp step;
    };
    static const StyleOps styleOps[];

    explicit LinkUi(QWidget *parent, RecverCard::Proxy &proxy);
    ~LinkUi();

    void widthChanged(LinkItem *item, int value);
    void heightChanged(LinkItem *item, int value);

private:
    void dimChanged(int value);
    void styleChanged(int id);
    void cellClicked(int row, int col);
    void itemClicked(bool in, int row, int col);
    void finish();
    void cancel();

    void load();
    void zoom();
    void sort();
    void save();

    void styleInit(QPoint &pos);
    void style0Init(QPoint &pos);
    void style1Init(QPoint &pos);
    void style2Init(QPoint &pos);
    void style3Init(QPoint &pos);
    void style4Init(QPoint &pos);
    void style5Init(QPoint &pos);
    void style6Init(QPoint &pos);
    void style7Init(QPoint &pos);
    void style8Init(QPoint &pos);

    void styleStep(QPoint &pos);
    void style0Step(QPoint &pos);
    void style1Step(QPoint &pos);
    void style2Step(QPoint &pos);
    void style3Step(QPoint &pos);
    void style4Step(QPoint &pos);
    void style5Step(QPoint &pos);
    void style6Step(QPoint &pos);
    void style7Step(QPoint &pos);
    void style8Step(QPoint &pos);

    RecverCard::Proxy &proxy;
    LinkItem::FocusSink sink;

    QButtonGroup styleBtns;
    Ui::LinkUi *ui;
};

#endif    // LINKUI_H
