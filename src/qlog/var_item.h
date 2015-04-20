#ifndef VAR_ITEM_H
#define VAR_ITEM_H

#include <QTableWidgetItem>
#include <QVariant>

class VarItem : public QTableWidgetItem
{
public:
    explicit VarItem(QVariant &var);
    virtual ~VarItem();

    QVariant var;
};

#endif // VAR_ITEM_H
