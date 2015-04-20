#ifndef PROGRAMVIEW_H
#define PROGRAMVIEW_H

#include <QFrame>

#include "mouse_select.h"

namespace Ui {
class ProgramView;
}

class Program;

class ProgramView : public QFrame
{
    Q_OBJECT

public:
    explicit ProgramView(Program *program);
    virtual ~ProgramView();

    Program *program;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent * event) override;

private:
    MouseSelect mouseSelect;
    Ui::ProgramView *ui;
};

#endif // PROGRAMVIEW_H
