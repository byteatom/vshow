#ifndef APPATTR_H
#define APPATTR_H

#include "util/xml_dom.h"

class AppAttr
{
public:
    AppAttr();

    ~AppAttr();

    void load();
    void save();

    std::wstring projectFile;
    std::wstring runDir;
    unsigned int tick;
    std::wstring language;
private:
    XmlDom xml;
};

#endif // APP_ATTR_H
