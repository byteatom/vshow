#ifndef SCREEN_EDITOR_H
#define SCREEN_EDITOR_H

#include "attr_cfger.h"
#include "data/screen_attr.h"

namespace Ui {
class ScreenCfger;
}

class ScreenCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit ScreenCfger();
    virtual ~ScreenCfger();

    void show(ScreenAttr *attr);

private:
    void lockChanged(bool checked);
    void topChanged(bool checked);

    ScreenAttr *attr;
    Ui::ScreenCfger *ui;
};

#endif    // SCREEN_EDITOR_H
