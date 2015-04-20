#include "filesrc_cfger.h"
#include "ui_filesrc_cfger.h"

#include <QFileDialog>

#include "segment.h"

FileSrcCfger::FileSrcCfger()
    : ui(new Ui::FileSrcCfger), attr(nullptr), filter(nullptr) {
    ui->setupUi(this);

    connect(ui->open, &QPushButton::clicked, this, &FileSrcCfger::open);
    connect(
        ui->path, &QLineEdit::textChanged, this, &FileSrcCfger::fileChanged);
}

FileSrcCfger::~FileSrcCfger() {
    delete ui;
}

void FileSrcCfger::show(Attr<std::wstring> *attr, const QString *filter) {
    this->attr = attr;
    this->filter = filter;
    AttrCfger::show();
    ui->path->setText(QString::fromStdWString(*attr));
}

void FileSrcCfger::setTitle(const QString &tile) {
    ui->tag->setText(tile);
}

void FileSrcCfger::open() {
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Select Source File"),
                                                "",
                                                *filter,
                                                nullptr,
                                                QFileDialog::ReadOnly);
    if (file.isEmpty()) {
        return;
    }
    ui->path->setText(file);
}

void FileSrcCfger::fileChanged(const QString &path) {
    *attr = path.toStdWString();
}
