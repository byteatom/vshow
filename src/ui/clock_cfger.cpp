#include "clock_cfger.h"
#include "ui_clock_cfger.h"
#include <QColorDialog>
#include "data/font_attr.h"


ClockCfger::ClockCfger() : ui(new Ui::ClockCfger), clock(nullptr) {
    ui->setupUi(this);
    ui->wrap->setChecked(true);
    QStringList displayFormatList;
    displayFormatList << tr("Date Week Time") << tr("Date Time Week")
                      << tr("Week Date Time") << tr("Week Time Date")
                      << tr("Time Date Week") << tr("Time Week Date");
    ui->displayFormat->addItems(displayFormatList);
    ui->displayFormat->setCurrentIndex(0);
    ui->lineSpace->setRange(0, 100);
    QStringList dateFormatList;
    dateFormatList << tr("YYYY MM DD") << tr("YYYY DD MM") << tr("MM YYYY DD")
                   << tr("MM DD YYYY") << tr("DD YYYY MM") << tr("DD MM YYYY");
    ui->dateFormat->addItems(dateFormatList);
    ui->dateFormat->setCurrentIndex(0);
    QStringList weekFormatList;
    weekFormatList << tr("Chinese") << tr("Chinese_TW") << tr("English");
    ui->weekFormat->addItems(weekFormatList);
    ui->weekFormat->setCurrentIndex(0);
    QStringList timeFormatList;
    timeFormatList << tr("HH MM SS") << tr("HH MM");
    ui->timeFormat->addItems(timeFormatList);
    ui->timeFormat->setCurrentIndex(0);
    ui->dateFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                               FontAttr::FONT_SIZE_MAX);
    ui->dateFontSize->setValue(FontAttr::FONT_SIZE_DEFAULT);
    ui->weekFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                               FontAttr::FONT_SIZE_MAX);
    ui->weekFontSize->setValue(FontAttr::FONT_SIZE_DEFAULT);
    ui->timeFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                               FontAttr::FONT_SIZE_MAX);
    ui->timeFontSize->setValue(FontAttr::FONT_SIZE_DEFAULT);
    QStringList dateUnitFormatList;
    dateUnitFormatList << tr("Slash") << tr("Chinese") << tr("Chinese_TW")
                       << tr("Colon");
    ui->dateUnitFormat->addItems(dateUnitFormatList);
    ui->dateUnitFormat->setCurrentIndex(0);
    QStringList timeUnitFormatList;
    timeUnitFormatList << tr("Colon") << tr("Chinese") << tr("Chinese_TW");
    ui->timeUnitFormat->addItems(timeUnitFormatList);
    ui->timeUnitFormat->setCurrentIndex(0);
    QStringList deltTypeList;
    deltTypeList << tr("+") << tr("-");
    ui->deltType->addItems(deltTypeList);
    ui->deltType->setCurrentIndex(0);
    ui->deltTime->setTime(QTime());
    ui->enabledText->setChecked(false);
    ui->lineEdit->setText("");
    ui->textFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                               FontAttr::FONT_SIZE_MAX);
    connect(ui->wrap, &QCheckBox::toggled, this, &ClockCfger::wrapChanged);
    connect(ui->displayFormat,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(displayFormat(const QString &)));
    connect(
        ui->lineSpace, SIGNAL(valueChanged(int)), this, SLOT(lineSpace(int)));
    connect(ui->dateFormat,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(dateFormat(const QString &)));
    connect(ui->dateColor, SIGNAL(clicked(bool)), this, SLOT(dateColor(bool)));
    connect(ui->dateFont,
            SIGNAL(currentFontChanged(const QFont &)),
            this,
            SLOT(dateFont(const QFont &)));
    connect(ui->dateFontSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(dateFontSize(int)));
    connect(ui->weekFormat,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(weekFormat(const QString &)));
    connect(ui->weekColor, SIGNAL(clicked(bool)), this, SLOT(weekColor(bool)));
    connect(ui->weekFont,
            SIGNAL(currentFontChanged(const QFont &)),
            this,
            SLOT(weekFont(const QFont &)));
    connect(ui->weekFontSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(weekFontSize(int)));
    connect(ui->timeFormat,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(timeFormat(const QString &)));
    connect(ui->timeColor, SIGNAL(clicked(bool)), this, SLOT(timeColor(bool)));
    connect(ui->timeFont,
            SIGNAL(currentFontChanged(const QFont &)),
            this,
            SLOT(timeFont(const QFont &)));
    connect(ui->timeFontSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(timeFontSize(int)));
    connect(ui->dateUnitFormat,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(dateUnitFormat(const QString &)));
    connect(ui->timeUnitFormat,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(timeUnitFormat(const QString &)));

    connect(ui->deltType,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(deltType(const QString &)));
    connect(
        ui->deltTime, SIGNAL(timeChanged(QTime)), this, SLOT(deltTime(QTime)));
    connect(
        ui->enabledText, SIGNAL(clicked(bool)), this, SLOT(enableText(bool)));
    connect(ui->lineEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(lineEdit(QString)));
    connect(ui->textFont,
            SIGNAL(currentFontChanged(QFont)),
            this,
            SLOT(textFont(QFont)));
    connect(ui->textFontSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(textFontSize(int)));
    connect(ui->textColor, SIGNAL(clicked(bool)), this, SLOT(textColor(bool)));
    connect(ui->bold, SIGNAL(clicked(bool)), this, SLOT(bold(bool)));
    connect(ui->italic, SIGNAL(clicked(bool)), this, SLOT(italic(bool)));
    connect(ui->underLine, SIGNAL(clicked(bool)), this, SLOT(underLine(bool)));
}

ClockCfger::~ClockCfger() {
    delete ui;
}

void ClockCfger::show(Clock *clock) {
    this->clock = clock;
    ui->wrap->setChecked(clock->attr()->clock.value.clockTime.lineStyle);
    ui->lineSpace->setValue(this->clock->attr()->clock.value.clockTime.lineSpace);
    ui->dateFormat->setCurrentIndex(static_cast<int>(
        this->clock->attr()->clock.value.clockTime.dateFormat.dateAlignFormat));
    QString dateFontName = QString::fromStdWString(
        this->clock->attr()->clock.value.clockTime.dateFormat.dateFontName);
    if (dateFontName.isEmpty()) {
        ui->timeFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->dateFont->setCurrentText(dateFontName);
    }
    ui->dateFontSize->setValue(
        this->clock->attr()->clock.value.clockTime.dateFormat.dateFontSize);
    ui->weekFormat->setCurrentIndex(
        static_cast<int>(this->clock->attr()->clock.value.clockTime.weekFormat
                             .clockWeekTextFormat));
    QString weekFontName = QString::fromStdWString(
        this->clock->attr()->clock.value.clockTime.weekFormat.weekFontName);
    if (weekFontName.isEmpty()) {
        ui->timeFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->weekFont->setCurrentText(weekFontName);
    }
    ui->weekFontSize->setValue(
        this->clock->attr()->clock.value.clockTime.weekFormat.weekFontSize);
    ui->timeFormat->setCurrentIndex(static_cast<int>(
        this->clock->attr()->clock.value.clockTime.timeFormat.timeAlignFormat));
    QString timeFontName = QString::fromStdWString(
        this->clock->attr()->clock.value.clockTime.timeFormat.timeFontName);
    if (timeFontName.isEmpty()) {
        ui->timeFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->timeFont->setCurrentText(timeFontName);
    }
    ui->timeFontSize->setValue(
        this->clock->attr()->clock.value.clockTime.timeFormat.timeFontSize);
    ui->dateUnitFormat->setCurrentIndex(static_cast<int>(
        this->clock->attr()->clock.value.clockTime.dateFormat.dateUintFormat));
    ui->timeUnitFormat->setCurrentIndex(static_cast<int>(
        this->clock->attr()->clock.value.clockTime.timeFormat.timeUintFormat));
    ui->deltType->setCurrentIndex(static_cast<int>(
        this->clock->attr()->clock.value.clockTime.clockTimeDiff.deltType));
    QTime deltTime(
        this->clock->attr()->clock.value.clockTime.clockTimeDiff.deltHour,
        this->clock->attr()->clock.value.clockTime.clockTimeDiff.deltMinute,
        this->clock->attr()->clock.value.clockTime.clockTimeDiff.deltSecond);
    ui->deltTime->setTime(deltTime);
    ui->enabledText->setChecked(
        this->clock->attr()->clock.value.clockText.enabledText);
    ui->lineEdit->setText(
        QString::fromStdWString(this->clock->attr()->clock.value.clockText.text));
    QString textFontName = QString::fromStdWString(
        this->clock->attr()->clock.value.clockText.textAttr.textFontName);
    if (textFontName.isEmpty()) {
        ui->textFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->textFont->setCurrentText(textFontName);
    }
    ui->textFontSize->setValue(
        this->clock->attr()->clock.value.clockText.textAttr.textFontSize);
    AttrCfger::show();
}

void ClockCfger::hide()
{
    clock=nullptr;
    AttrCfger::hide();
}

void ClockCfger::wrapChanged(bool checked) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.lineStyle = (ClockLineStyle)checked;
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::displayFormat(const QString &) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.displayFormat = static_cast<ClockDisplayFormat>(
            ui->displayFormat->currentIndex());
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::lineSpace(int) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.lineSpace = ui->lineSpace->value();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::dateFormat(const QString &) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.dateFormat.dateAlignFormat
            = static_cast<ClockDateAlignFormat>(ui->dateFormat->currentIndex());
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::dateColor(bool checked) {
    if (clock) {
        QColorDialog colorDlg;
        ClockInfo clockInfo = clock->attr()->clock.value;
        colorDlg.setWindowTitle(tr("Select Date Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(clockInfo.clockTime.dateFormat.dateColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            clockInfo.clockTime.dateFormat.dateColor
                = colorDlg.currentColor().rgba();
            clock->attr()->clock = clockInfo;
        }
    }
}

void ClockCfger::dateFont(const QFont &f) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.dateFormat.dateFontName = f.family().toStdWString();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::dateFontSize(int) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.dateFormat.dateFontSize = ui->dateFontSize->value();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::weekFormat(const QString &) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.weekFormat.clockWeekTextFormat
            = static_cast<ClockWeekTextFormat>(ui->weekFormat->currentIndex());
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::weekColor(bool checked) {
    if (clock) {
        QColorDialog colorDlg;
        ClockInfo clockInfo = clock->attr()->clock.value;
        colorDlg.setWindowTitle(tr("Select Week Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(clockInfo.clockTime.weekFormat.weekColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            clockInfo.clockTime.weekFormat.weekColor
                = colorDlg.currentColor().rgba();
            clock->attr()->clock = clockInfo;
        }
    }
}

void ClockCfger::weekFont(const QFont &f) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.weekFormat.weekFontName = f.family().toStdWString();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::weekFontSize(int) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.weekFormat.weekFontSize = ui->weekFontSize->value();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::timeFormat(const QString &) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.timeFormat.timeAlignFormat
            = static_cast<ClockTimeAlignFormat>(ui->timeFormat->currentIndex());
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::timeColor(bool checked) {
    if (clock) {
        QColorDialog colorDlg;
        ClockInfo clockInfo = clock->attr()->clock.value;
        colorDlg.setWindowTitle(tr("Select Time Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(clockInfo.clockTime.timeFormat.timeColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            clockInfo.clockTime.timeFormat.timeColor
                = colorDlg.currentColor().rgba();
            clock->attr()->clock = clockInfo;
        }
    }
}

void ClockCfger::timeFont(const QFont &f) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.timeFormat.timeFontName = f.family().toStdWString();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::timeFontSize(int) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.timeFormat.timeFontSize = ui->timeFontSize->value();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::dateUnitFormat(const QString &) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.dateFormat.dateUintFormat
            = static_cast<ClockDateUintFormat>(
                ui->dateUnitFormat->currentIndex());
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::timeUnitFormat(const QString &) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.timeFormat.timeUintFormat
            = static_cast<ClockTimeUintFormat>(
                ui->timeUnitFormat->currentIndex());
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::deltType(const QString &) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.clockTimeDiff.deltType
            = static_cast<ClockDeltType>(ui->deltType->currentIndex());
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::deltTime(const QTime &time) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockTime.clockTimeDiff.deltHour = time.hour();
        clockInfo.clockTime.clockTimeDiff.deltMinute = time.minute();
        clockInfo.clockTime.clockTimeDiff.deltSecond = time.second();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::enableText(bool checked) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockText.enabledText = ui->enabledText->isChecked();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::lineEdit(const QString &) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockText.text = ui->lineEdit->text().toStdWString();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::textFont(const QFont &f) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockText.textAttr.textFontName = f.family().toStdWString();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::textFontSize(int) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockText.textAttr.textFontSize = ui->textFontSize->value();
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::textColor(bool checked) {
    if (clock) {
        QColorDialog colorDlg;
        ClockInfo clockInfo = clock->attr()->clock.value;
        colorDlg.setWindowTitle(tr("Select Text Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(clockInfo.clockText.textAttr.textColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            clockInfo.clockText.textAttr.textColor
                = colorDlg.currentColor().rgba();
            clock->attr()->clock = clockInfo;
        }
    }
}

void ClockCfger::bold(bool checked) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockText.textAttr.bold = !clockInfo.clockText.textAttr.bold;
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::italic(bool checked) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockText.textAttr.italic
            = !clockInfo.clockText.textAttr.italic;
        clock->attr()->clock = clockInfo;
    }
}

void ClockCfger::underLine(bool checked) {
    if (clock) {
        ClockInfo clockInfo = clock->attr()->clock.value;
        clockInfo.clockText.textAttr.underLine
            = !clockInfo.clockText.textAttr.underLine;
        clock->attr()->clock = clockInfo;
    }
}
