#include "text_cfger.h"
#include "ui_text_cfger.h"

#include <QColorDialog>

#include "text.h"


#define FONT_SIZE_MIN 5
#define FONT_SIZE_MAX 100
#define FONT_SIZE_DEFAULT 16

TextCfger::TextCfger() : ui(new Ui::TextCfger), text(nullptr) {
    ui->setupUi(this);

    alignBtns.addButton(ui->left);
    alignBtns.addButton(ui->center);
    alignBtns.addButton(ui->right);
    alignBtns.addButton(ui->justify);


    ui->size->setRange(FONT_SIZE_MIN, FONT_SIZE_MAX);
    ui->size->setValue(FONT_SIZE_DEFAULT);
    connect(ui->family,
            &QFontComboBox::currentFontChanged,
            this,
            &TextCfger::family);
    connect(ui->size, SIGNAL(valueChanged(int)), this, SLOT(textSize(int)));
    connect(ui->wrap, &QPushButton::clicked, this, &TextCfger::wrap);
    connect(ui->left, &QPushButton::clicked, this, &TextCfger::left);
    connect(ui->center, &QPushButton::clicked, this, &TextCfger::center);
    connect(ui->right, &QPushButton::clicked, this, &TextCfger::right);
    connect(ui->justify, &QPushButton::clicked, this, &TextCfger::justify);
    connect(ui->bgColor, &QPushButton::clicked, this, &TextCfger::backColor);
    connect(ui->fgColor, &QPushButton::clicked, this, &TextCfger::foreColor);
    connect(ui->bold, &QPushButton::clicked, this, &TextCfger::bold);
    connect(ui->underLine, &QPushButton::clicked, this, &TextCfger::underLine);
    connect(ui->italic, &QPushButton::clicked, this, &TextCfger::italic);
}

TextCfger::~TextCfger() {
    delete ui;
}

void TextCfger::show(Text *text) {
    this->text = text;

    ui->wrap->setChecked(text->attr()->wrap);
    updateBtns();

    AttrCfger::show();
}

void TextCfger::family(const QFont &font) {
    if (text)
        text->editor->setFontFamily(font.family());
}

void TextCfger::textSize(int textSize) {
    if (text)
        text->editor->setFontPointSize(textSize);
}

void TextCfger::wrap(bool checked) {
    text->editor->setLineWrapMode(checked ? QTextEdit::WidgetWidth
                                          : QTextEdit::NoWrap);
    text->attr()->wrap = checked;
}

void TextCfger::left(bool checked) {
    if (checked && text)
        text->editor->setAlignment(Qt::AlignLeft);
}

void TextCfger::center(bool checked) {
    if (checked && text)
        text->editor->setAlignment(Qt::AlignHCenter);
}

void TextCfger::right(bool checked) {
    if (checked && text)
        text->editor->setAlignment(Qt::AlignRight);
}

void TextCfger::justify(bool checked) {
    if (checked && text)
        text->editor->setAlignment(Qt::AlignJustify);
}

void TextCfger::backColor() {
    QColor color = QColorDialog::getColor(Qt::black,
                                          this,
                                          tr("Select Background Color"),
                                          QColorDialog::ShowAlphaChannel);
    if (color.isValid() && text) {
        text->editor->setTextBackgroundColor(color);
    }
}

void TextCfger::foreColor() {
    QColor color = QColorDialog::getColor(Qt::white,
                                          this,
                                          tr("Select Foreground Color"),
                                          QColorDialog::ShowAlphaChannel);
    if (color.isValid() && text) {
        text->editor->setTextColor(color);
    }
}

void TextCfger::bold(bool checked) {
    if (text)
        text->editor->setFontWeight(checked ? QFont::Bold : QFont::Normal);
}

void TextCfger::underLine(bool checked) {
    if (text)
        text->editor->setFontUnderline(checked);
}

void TextCfger::italic(bool checked) {
    if (text)
        text->editor->setFontItalic(checked);
}


void TextCfger::updateBtns() {
    QTextCharFormat &format = text->editor->currentCharFormat();
    QFont font = format.font();
    ui->family->blockSignals(true);
    ui->size->blockSignals(true);
    // ui->family->setCurrentIndex(ui->family->findText(QFontInfo(font).family()));
    ui->family->setCurrentText(QFontInfo(font).family());
    // ui->size->setCurrentIndex(ui->size->findText(QString::number(font.pointSize())));
    ui->size->setValue(font.pointSize());
    ui->family->blockSignals(false);
    ui->size->blockSignals(false);
    ui->bold->setChecked(font.bold());
    ui->italic->setChecked(font.italic());
    ui->underLine->setChecked(font.underline());

    Qt::Alignment align = text->editor->alignment();
    if (align & Qt::AlignLeft)
        ui->left->setChecked(true);
    else if (align & Qt::AlignHCenter)
        ui->center->setChecked(true);
    else if (align & Qt::AlignRight)
        ui->right->setChecked(true);
    else if (align & Qt::AlignJustify)
        ui->justify->setChecked(true);
}
