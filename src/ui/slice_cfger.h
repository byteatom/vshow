#ifndef SLICECFGER_H
#define SLICECFGER_H

#include <QButtonGroup>

#include "attr_cfger.h"
#include "data/slice_attr.h"

namespace Ui {
class SliceCfger;
}

class SliceCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit SliceCfger();
    virtual ~SliceCfger();

    void show(SliceAttr *attr);
    SliceAttr *attr;

private:
    // slot
    void nameChanged(const QString &name);
    void colorSwitch(int state);
    void colorSelect();

    Ui::SliceCfger *ui;
};

#endif    // SLICECFGER_H
