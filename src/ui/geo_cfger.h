#ifndef REGIONCFGER_H
#define REGIONCFGER_H

#include "attr_cfger.h"
#include "data/geo_attr.h"

namespace Ui {
class GeoCfger;
}

class GeoCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit GeoCfger();
    virtual ~GeoCfger();

    void show(GeoAttr *attr);

private:
    void xChanged(int x);
    void yChanged(int y);
    void widthChanged(int width);
    void heightChanged(int height);

    GeoAttr *attr;
    Ui::GeoCfger *ui;
};

#endif    // REGIONCFGER_H
