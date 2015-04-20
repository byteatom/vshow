#ifndef PICTURE_EDITOR_H
#define PICTURE_EDITOR_H

#include <QButtonGroup>

#include "attr_cfger.h"
#include "data/attr.h"
#include "data/image_attr.h"

namespace Ui {
class ImageCfger;
}

class ImageCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit ImageCfger();
    ~ImageCfger();

    void show(Attr<ImageTool::Scale> *scale);

private:
    void scaleChanged(int index);

    Attr<ImageTool::Scale> *scale;
    QButtonGroup scaleBtns;
    Ui::ImageCfger *ui;
};

#endif    // PICTURE_EDITOR_H
