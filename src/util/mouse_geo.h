#ifndef MOUSE_GEO_H
#define MOUSE_GEO_H

#include <QWidget>
#include <QMouseEvent>

class MouseGeometry : public QObject
{
    Q_OBJECT

public:
    static const int EDGE_SIZE = 8;
    explicit MouseGeometry(QWidget *widget);
    virtual ~MouseGeometry();

    void setEnabled(bool enable);

protected:
  virtual bool eventFilter(QObject* obj, QEvent *event) override;

private:
    void press(QMouseEvent *event);
    void move(QMouseEvent *event);

    bool enabled;
    QWidget *widget;
    QPointF dragPos;
    QRectF dragRect;
    bool onTopEdge;
    bool onBottomEdge;
    bool onLeftEdge;
    bool onRightEdge;
    Qt::CursorShape cursor;
};

#endif // MOUSE_GEO_H
