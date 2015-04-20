#ifndef DATA_H
#define DATA_H

#include "app_attr.h"
#include "project_attr.h"
#include "card_manager.h"

class Data
{
public:
    static Data *obj;

    Data();
    ~Data();

    void load();
    void save();
    void free();

    AppAttr *appAttr;
    ProjectAttr *projectAttr;
    CardManager *cards;
};

#endif // DATA_H
