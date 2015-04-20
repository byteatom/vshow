#ifndef ATTRCFGER_H
#define ATTRCFGER_H

#include <QFrame>

class Slice;

class AttrCfger : public QFrame {
public:
    AttrCfger();
    virtual ~AttrCfger();

protected:
    virtual QSize sizeHint() const override;
};

#endif    // ATTRCFGER_H
