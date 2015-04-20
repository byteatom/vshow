#include "var_item.h"

VarItem::VarItem(QVariant &var) :
    var(var),
    QTableWidgetItem(QString(var.typeName()))
{

}

VarItem::~VarItem()
{

}

