#include "program_view.h"
#include "ui_program_view.h"

#include <QPainter>

#include "program.h"
#include "qlog/qlog.h"
#include "main_ui.h"

ProgramView::ProgramView(Program *program) :
    QFrame(nullptr),
    ui(new Ui::ProgramView),
    program{program},
    mouseSelect{this, program}
{
    ui->setupUi(this);

    //setAttribute(Qt::WA_OpaquePaintEvent, true);

    program->owner->view()->layout.addWidget(this);
}

ProgramView::~ProgramView()
{
    delete ui;
}

void ProgramView::paintEvent(QPaintEvent *event)
{
    //qlog << "ProgramView::paintEvent " << QString::fromStdWString(program->attr()->name) << qdebug;
    QPainter painter(this);
    if (ImageAttr::BG_COLOR_NONE != program->attr()->bgColor) {
        painter.fillRect(rect(), QColor::fromRgba(program->attr()->bgColor));
    }
    if (!program->bgCache.isNull()) {
        painter.drawPixmap(Slice::POS_TOPLEFT, program->bgCache);
    }
}

void ProgramView::resizeEvent(QResizeEvent * event)
{
    program->cacheView();
    QFrame::resizeEvent(event);
}
