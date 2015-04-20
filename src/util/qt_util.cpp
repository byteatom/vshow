#include "qt_util.h"

#include <QApplication>
#include <QDesktopWidget>

namespace QtUtil {


QSize parentSize(QWidget *widget)
{
    QWidget *parent = widget->parentWidget();
    return parent ? parent->frameGeometry().size() :
                    QApplication::desktop()->screenGeometry().size();
}

}
