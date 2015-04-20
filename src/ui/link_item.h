#ifndef LINK_ITEM_H
#define LINK_ITEM_H

#include <QFrame>
#include <QSpinBox>

#include <functional>

namespace Ui {
class LinkItem;
}

class LinkUi;

class LinkItem : public QFrame {
    Q_OBJECT

public:
    typedef std::function<void(bool in, int row, int col)> FocusSink;
    explicit LinkItem(int row, int col, LinkUi *linkUi);
    ~LinkItem();
    QSpinBox *index;
    QSpinBox *width;
    QSpinBox *height;
    int row;
    int col;

private:
    void catchEvent(QObject *widget);
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
    virtual bool event(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    void widthChanged();
    void heightChanged();

    bool focusIn;
    FocusSink sink;

    LinkUi *linkUi;
    Ui::LinkItem *ui;
};

#endif    // LINK_ITEM_H
