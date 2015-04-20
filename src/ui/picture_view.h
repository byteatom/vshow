#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <QWidget>
#include <QPixmap>
#include <QPointF>

#include "mouse_select.h"
#include "animator.h"

class Picture;

class PictureView : public QWidget
{
    Q_OBJECT
public:
    explicit PictureView(Picture *picture);
    virtual ~PictureView();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent * event) override;

private:
    Picture *picture;
    MouseSelect mouseSelect;
};

#endif // PICTUREVIEW_H
