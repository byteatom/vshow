#ifndef LEAKSPINBOX_H
#define LEAKSPINBOX_H

#include <QSpinBox>
#include <QMouseEvent>

class LeakSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit LeakSpinBox(QWidget *parent = 0);
    ~LeakSpinBox();

protected:
    void catchEvent(QObject *widget);
    virtual bool eventFilter(QObject* obj, QEvent *event) override;
    virtual bool event(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // LEAKSPINBOX_H
