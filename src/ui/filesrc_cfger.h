#ifndef FILESRCCFGER_H
#define FILESRCCFGER_H

#include <string>

#include "attr_cfger.h"
#include "data/attr.h"

namespace Ui {
class FileSrcCfger;
}

class FileSrcCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit FileSrcCfger();
    ~FileSrcCfger();

    void show(Attr<std::wstring> *attr, const QString *filter);
    void setTitle(const QString &tile);

private:
    void open();
    void fileChanged(const QString &path);

    Attr<std::wstring> *attr;
    const QString *filter;
    Ui::FileSrcCfger *ui;
};

#endif    // FILESRCCFGER_H
