#ifndef PROGRAMCFGER_H
#define PROGRAMCFGER_H

#include "attr_cfger.h"
#include "data/program_attr.h"

namespace Ui {
class ProgramCfger;
}

class ProgramCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit ProgramCfger();
    virtual ~ProgramCfger();

    void show(ProgramAttr *attr);

private:
    ProgramAttr *attr;
    Ui::ProgramCfger *ui;
};

#endif    // PROGRAMCFGER_H
