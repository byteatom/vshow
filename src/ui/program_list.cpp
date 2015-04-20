#include "program_list.h"
#include "ui_program_list.h"
#include "main_ui.h"

ProgramList::ProgramList(QWidget *parent)
    : QDialog(parent), ui(new Ui::ProgramList) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    ui->programs->viewport()->setBackgroundRole(QPalette::Window);
    // setAttribute(Qt::WA_TranslucentBackground, true);
    // setWindowOpacity(0.5);
}

ProgramList::~ProgramList() {
    delete ui;
}

void ProgramList::refreshProgramList() {
    showProgramList();
}

void ProgramList::showEvent(QShowEvent *) {
    showProgramList();
}

void ProgramList::leaveEvent(QEvent *) {
    hide();
    static_cast<ScreenView *>(this->parent())->screen->playCtrl->hide();
}

void ProgramList::showProgramList() {
    ui->programs->clear();
    Screen *screen = static_cast<ScreenView *>(this->parent())->screen;
    for (auto organProgram : screen->organs) {
        Slice *program = static_cast<Slice *>(organProgram);
        QString itemProgram = QString::fromStdWString(
            static_cast<Slice *>(organProgram)->attr()->name);
        ui->programs->addItem(itemProgram);
        if (screen->serialItor != screen->organs.end())
            if (static_cast<Slice *>(*screen->serialItor) == program) {
                ui->programs->setItemSelected(
                    ui->programs->findItems(itemProgram, Qt::MatchCaseSensitive)
                        .first(),
                    true);
            }
    }
}
