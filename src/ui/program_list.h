#ifndef PROGRAMLIST_H
#define PROGRAMLIST_H

#include <QDialog>

namespace Ui {
class ProgramList;
}

class ProgramList : public QDialog {
    Q_OBJECT

public:
    explicit ProgramList(QWidget *parent);
    ~ProgramList();
    void refreshProgramList();

protected:
    virtual void showEvent(QShowEvent *) override;
    virtual void leaveEvent(QEvent *) override;

private:
    void showProgramList();
    Ui::ProgramList *ui;
};

#endif    // PROGRAMLIST_H
