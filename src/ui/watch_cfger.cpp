#include "watch_cfger.h"
#include "ui_watch_cfger.h"
#include <QColorDialog>
#include <QFontMetrics>
#include "data/watch_attr.h"
#include "data/font_attr.h"


#define WATCH_RANGE_MIN 0
#define WATCH_RANGE_MAX 9
#define DOT_SIZE_MIN WATCH_RANGE_MIN
#define DOT_SIZE_MAX WATCH_RANGE_MAX
#define HAND_SIZE_MIN WATCH_RANGE_MIN
#define HAND_SIZE_MAX WATCH_RANGE_MAX


const double WatchCfger::step = 2;

WatchCfger::WatchCfger() : ui(new Ui::WatchCfger), watch(nullptr) {
    ui->setupUi(this);
    QStringList dialStyleList;
    dialStyleList << tr("Circle") << tr("RoundRect") << tr("Ellipse")
                  << tr("Rhombus") << tr("Windmill");
    ui->dialStyle->addItems(dialStyleList);
    ui->dialStyle->setCurrentIndex(0);
    QStringList numStyleList;
    numStyleList << tr("Arabic") << tr("Roman");
    ui->hourNumStyle->addItems(numStyleList);
    ui->hourNumStyle->setCurrentIndex(0);
    ui->tsnNumStyle->addItems(numStyleList);
    ui->tsnNumStyle->setCurrentIndex(0);
    ui->hourNumFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                                  FontAttr::FONT_SIZE_MAX);
    ui->tsnNumFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                                 FontAttr::FONT_SIZE_MAX);
    ui->hourPointSize->setRange(DOT_SIZE_MIN, DOT_SIZE_MAX);
    ui->tsnPointSize->setRange(DOT_SIZE_MIN, DOT_SIZE_MAX);
    ui->minutePointSize->setRange(DOT_SIZE_MIN, DOT_SIZE_MAX);
    QStringList pointStyleList;
    pointStyleList << tr("Circle") << tr("Square") << tr("Line");
    ui->hourPointStyle->addItems(pointStyleList);
    ui->hourPointStyle->setCurrentIndex(0);
    ui->tsnPointStyle->addItems(pointStyleList);
    ui->tsnPointStyle->setCurrentIndex(0);
    ui->minutePointStyle->addItems(pointStyleList);
    ui->minutePointStyle->setCurrentIndex(0);
    ui->hourHandSize->setRange(HAND_SIZE_MIN, HAND_SIZE_MAX);
    ui->minuteHandSize->setRange(HAND_SIZE_MIN, HAND_SIZE_MAX);
    ui->secondHandSize->setRange(HAND_SIZE_MIN, HAND_SIZE_MAX);
    QStringList handStyleList;
    handStyleList << tr("Line") << tr("Rhombus");
    ui->hourHandStyle->addItems(handStyleList);
    ui->hourHandStyle->setCurrentIndex(0);
    ui->minuteHandStyle->addItems(handStyleList);
    ui->minuteHandStyle->setCurrentIndex(0);
    ui->secondHandStyle->addItems(handStyleList);
    ui->secondHandStyle->setCurrentIndex(0);
    QStringList deltTypeList;
    deltTypeList << tr("+") << tr("-");
    ui->deltType->addItems(deltTypeList);
    ui->deltType->setCurrentIndex(0);
    ui->deltTime->setTime(QTime());
    ui->enabledText->setChecked(false);
    ui->lineEdit->setText("");
    ui->textFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                               FontAttr::FONT_SIZE_MAX);
    connect(ui->dialStyle,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(dialStyle(int)));
    connect(ui->hourNumStyle,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(hourNumStyle(const QString &)));
    connect(ui->hourNumColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(hourNumColor(bool)));
    connect(ui->hourNumFont,
            SIGNAL(currentFontChanged(const QFont &)),
            this,
            SLOT(hourNumFont(const QFont &)));
    connect(ui->hourNumFontSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(hourNumFontSize(int)));
    connect(ui->hourNumDisplay,
            SIGNAL(toggled(bool)),
            this,
            SLOT(hourNumDisplay(bool)));
    connect(ui->tsnNumStyle,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(tsnNumStyle(const QString &)));
    connect(
        ui->tsnNumColor, SIGNAL(clicked(bool)), this, SLOT(tsnNumColor(bool)));
    connect(ui->tsnNumFont,
            SIGNAL(currentFontChanged(QFont)),
            this,
            SLOT(tsnNumFont(QFont)));
    connect(ui->tsnNumFontSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(tsnNumFontSize(int)));
    connect(ui->tsnNumDisplay,
            SIGNAL(toggled(bool)),
            this,
            SLOT(tsnNumDisplay(bool)));
    connect(ui->hourPointSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(hourPointSize(int)));
    connect(ui->hourPointColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(hourPointColor(bool)));
    connect(ui->hourPointStyle,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(hourPointStyle(const QString &)));
    connect(ui->tsnPointSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(tsnPointSize(int)));
    connect(ui->tsnPointColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tsnPointColor(bool)));
    connect(ui->tsnPointStyle,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(tsnPointStyle(const QString &)));
    connect(ui->minutePointSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(minutePointSize(int)));
    connect(ui->minutePointColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(minutePointColor(bool)));
    connect(ui->minutePointStyle,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(minutePointStyle(const QString &)));
    connect(ui->hourHandSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(hourHandSize(int)));
    connect(ui->hourHandColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(hourHandColor(bool)));
    connect(ui->hourHandStyle,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(hourHandStyle(const QString &)));
    connect(ui->minuteHandSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(minuteHandSize(int)));
    connect(ui->minuteHandColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(minuteHandColor(bool)));
    connect(ui->minuteHandStyle,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(minuteHandStyle(const QString &)));
    connect(ui->secondHandSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(secondHandSize(int)));
    connect(ui->secondHandColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(secondHandColor(bool)));
    connect(ui->secondHandStyle,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(secondHandStyle(const QString &)));
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
    connect(ui->down, SIGNAL(clicked(bool)), this, SLOT(down(bool)));
    connect(ui->left, SIGNAL(clicked(bool)), this, SLOT(left(bool)));
    connect(ui->center, SIGNAL(clicked(bool)), this, SLOT(center(bool)));
    connect(ui->right, SIGNAL(clicked(bool)), this, SLOT(right(bool)));
    connect(ui->up, SIGNAL(clicked(bool)), this, SLOT(up(bool)));
}

WatchCfger::~WatchCfger() {
    delete ui;
}

void WatchCfger::show(Watch *watch) {
    this->watch = watch;
    ui->dialStyle->setCurrentIndex(
        static_cast<int>(this->watch->attr()->dial.value.dialStyle));
    ui->hourNumStyle->setCurrentIndex(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialNum.hourNum.numStyle));
    QString hourNumFontName = QString::fromStdWString(
        this->watch->attr()->dial.value.dialTime.dialNum.hourNum.fontName);
    if (hourNumFontName.isEmpty()) {
        ui->hourNumFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->hourNumFont->setCurrentText(hourNumFontName);
    }
    ui->hourNumFontSize->setValue(
        this->watch->attr()->dial.value.dialTime.dialNum.hourNum.fontSize);
    ui->hourNumDisplay->setChecked(static_cast<bool>(
        this->watch->attr()->dial.value.dialTime.dialNum.hourNum.displayType));
    ui->tsnNumStyle->setCurrentIndex(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialNum.tsnNum.numStyle));
    QString tsnNumFontName = QString::fromStdWString(
        this->watch->attr()->dial.value.dialTime.dialNum.tsnNum.fontName);
    if (tsnNumFontName.isEmpty()) {
        ui->tsnNumFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->tsnNumFont->setCurrentText(tsnNumFontName);
    }
    ui->tsnNumFontSize->setValue(
        this->watch->attr()->dial.value.dialTime.dialNum.tsnNum.fontSize);
    ui->tsnNumDisplay->setChecked(static_cast<bool>(
        this->watch->attr()->dial.value.dialTime.dialNum.tsnNum.displayType));
    ui->hourPointSize->setValue(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialPoint.hourPoint.pointSize));
    ui->hourPointStyle->setCurrentIndex(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialPoint.hourPoint.pointStyle));
    ui->tsnPointSize->setValue(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialPoint.tsnPoint.pointSize));
    ui->tsnPointStyle->setCurrentIndex(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialPoint.tsnPoint.pointStyle));
    ui->minutePointSize->setValue(
        static_cast<int>(this->watch->attr()->dial.value.dialTime.dialPoint
                             .minutePoint.pointSize));
    ui->minutePointStyle->setCurrentIndex(
        static_cast<int>(this->watch->attr()->dial.value.dialTime.dialPoint
                             .minutePoint.pointStyle));
    ui->hourHandSize->setValue(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialHand.hourHand.handSize));
    ui->hourHandStyle->setCurrentIndex(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialHand.hourHand.handStyle));
    ui->minuteHandSize->setValue(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialHand.minuteHand.handSize));
    ui->minuteHandStyle->setCurrentIndex(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialHand.minuteHand.handStyle));
    ui->secondHandSize->setValue(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialHand.secondHand.handSize));
    ui->secondHandStyle->setCurrentIndex(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialHand.secondHand.handStyle));
    ui->deltType->setCurrentIndex(static_cast<int>(
        this->watch->attr()->dial.value.dialTime.dialTimeDiff.deltType));
    QTime deltTime(
        this->watch->attr()->dial.value.dialTime.dialTimeDiff.deltHour,
        this->watch->attr()->dial.value.dialTime.dialTimeDiff.deltMinute,
        this->watch->attr()->dial.value.dialTime.dialTimeDiff.deltSecond);
    ui->deltTime->setTime(deltTime);
    ui->enabledText->setChecked(
        this->watch->attr()->dial.value.dialText.enabledText);
    ui->lineEdit->setText(
        QString::fromStdWString(this->watch->attr()->dial.value.dialText.text));
    QString textFontName = QString::fromStdWString(
        this->watch->attr()->dial.value.dialText.textAttr.textFontName);
    if (textFontName.isEmpty()) {
        ui->textFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->textFont->setCurrentText(textFontName);
    }
    ui->textFontSize->setValue(
        this->watch->attr()->dial.value.dialText.textAttr.textFontSize);
    AttrCfger::show();
}

void WatchCfger::hide() {
    watch = nullptr;
    AttrCfger::hide();
}


void WatchCfger::dialStyle(int index) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialStyle
            = static_cast<DialStyle>(ui->dialStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::hourNumStyle(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialNum.hourNum.numStyle
            = static_cast<DialNumStyle>(ui->hourNumStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::hourNumColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select Hour Number Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(dialInfo.dialTime.dialNum.hourNum.numColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialTime.dialNum.hourNum.numColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::hourNumFont(const QFont &f) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialNum.hourNum.fontName = f.family().toStdWString();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::hourNumFontSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialNum.hourNum.fontSize
            = ui->hourNumFontSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::hourNumDisplay(bool checked) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialNum.hourNum.displayType
            = (DialDisplayType)checked;
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::tsnNumStyle(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialNum.tsnNum.numStyle
            = static_cast<DialNumStyle>(ui->tsnNumStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::tsnNumColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select TSN Number Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(dialInfo.dialTime.dialNum.tsnNum.numColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialTime.dialNum.tsnNum.numColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::tsnNumFont(const QFont &f) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialNum.tsnNum.fontName = f.family().toStdWString();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::tsnNumFontSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialNum.tsnNum.fontSize = ui->tsnNumFontSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::tsnNumDisplay(bool checked) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialNum.tsnNum.displayType = (DialDisplayType)checked;
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::hourPointSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialPoint.hourPoint.pointSize
            = ui->hourPointSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::hourPointColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select Hour Dot Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(dialInfo.dialTime.dialPoint.hourPoint.pointColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialTime.dialPoint.hourPoint.pointColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::hourPointStyle(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialPoint.hourPoint.pointStyle
            = static_cast<DialPointStyle>(ui->hourPointStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::tsnPointSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialPoint.tsnPoint.pointSize
            = ui->tsnPointSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::tsnPointColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select TSN Dot Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(dialInfo.dialTime.dialPoint.tsnPoint.pointColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialTime.dialPoint.tsnPoint.pointColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::tsnPointStyle(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialPoint.tsnPoint.pointStyle
            = static_cast<DialPointStyle>(ui->tsnPointStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::minutePointSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialPoint.minutePoint.pointSize
            = ui->minutePointSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::minutePointColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select Minute Dot Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(
            dialInfo.dialTime.dialPoint.minutePoint.pointColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialTime.dialPoint.minutePoint.pointColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::minutePointStyle(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialPoint.minutePoint.pointStyle
            = static_cast<DialPointStyle>(ui->minutePointStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::hourHandSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialHand.hourHand.handSize
            = ui->hourHandSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::hourHandColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select Hour Hand Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(dialInfo.dialTime.dialHand.hourHand.handColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialTime.dialHand.hourHand.handColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::hourHandStyle(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialHand.hourHand.handStyle
            = static_cast<DialHandStyle>(ui->hourHandStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::minuteHandSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialHand.minuteHand.handSize
            = ui->minuteHandSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::minuteHandColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select Minute Hand Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(dialInfo.dialTime.dialHand.minuteHand.handColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialTime.dialHand.minuteHand.handColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::minuteHandStyle(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialHand.minuteHand.handStyle
            = static_cast<DialHandStyle>(ui->minuteHandStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::secondHandSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialHand.secondHand.handSize
            = ui->secondHandSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::secondHandColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select Second Hand Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(dialInfo.dialTime.dialHand.secondHand.handColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialTime.dialHand.secondHand.handColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::secondHandStyle(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialHand.secondHand.handStyle
            = static_cast<DialHandStyle>(ui->secondHandStyle->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::deltType(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialTimeDiff.deltType
            = static_cast<DialDeltType>(ui->deltType->currentIndex());
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::deltTime(const QTime &time) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialTime.dialTimeDiff.deltHour = time.hour();
        dialInfo.dialTime.dialTimeDiff.deltMinute = time.minute();
        dialInfo.dialTime.dialTimeDiff.deltSecond = time.second();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::enableText(bool checked) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialText.enabledText = ui->enabledText->isChecked();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::lineEdit(const QString &) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialText.text = ui->lineEdit->text().toStdWString();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::textFont(const QFont &f) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialText.textAttr.textFontName = f.family().toStdWString();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::textFontSize(int) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialText.textAttr.textFontSize = ui->textFontSize->value();
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::textColor(bool checked) {
    if (watch) {
        QColorDialog colorDlg;
        DialInfo dialInfo = watch->attr()->dial.value;
        colorDlg.setWindowTitle(tr("Select Text Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(dialInfo.dialText.textAttr.textColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            dialInfo.dialText.textAttr.textColor
                = colorDlg.currentColor().rgba();
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::bold(bool checked) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialText.textAttr.bold = !dialInfo.dialText.textAttr.bold;
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::italic(bool checked) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialText.textAttr.italic = !dialInfo.dialText.textAttr.italic;
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::underLine(bool checked) {
    if (watch) {
        DialInfo dialInfo = watch->attr()->dial.value;
        dialInfo.dialText.textAttr.underLine
            = !dialInfo.dialText.textAttr.underLine;
        watch->attr()->dial = dialInfo;
    }
}

void WatchCfger::down(bool checked) {
    if (watch) {
        bool enabledText = isEnabledText();
        if (enabledText) {
            QRectF viewRect = watch->view()->rect();
            double watchDiameter = qMin(viewRect.width(), viewRect.height());
            DialInfo dialInfo = watch->attr()->dial.value;
            dialInfo.dialText.textAttr.yPosRatio
                = (dialInfo.dialText.textAttr.yPosRatio * watchDiameter + step)
                  / watchDiameter;
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::left(bool checked) {
    if (watch) {
        bool enabledText = isEnabledText();
        if (enabledText) {
            QRectF viewRect = watch->view()->rect();
            double watchDiameter = qMin(viewRect.width(), viewRect.height());
            DialInfo dialInfo = watch->attr()->dial.value;
            dialInfo.dialText.textAttr.xPosRatio
                = (dialInfo.dialText.textAttr.xPosRatio * watchDiameter - step)
                  / watchDiameter;
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::center(bool checked) {
    if (watch) {
        bool enabledText = isEnabledText();
        if (enabledText) {
            DialInfo dialInfo = watch->attr()->dial.value;
            dialInfo.dialText.textAttr.xPosRatio = X_RATIO;
            dialInfo.dialText.textAttr.yPosRatio = Y_RATIO;
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::right(bool checked) {
    if (watch) {
        bool enabledText = isEnabledText();
        if (enabledText) {
            QRectF viewRect = watch->view()->rect();
            double watchDiameter = qMin(viewRect.width(), viewRect.height());
            DialInfo dialInfo = watch->attr()->dial.value;
            dialInfo.dialText.textAttr.xPosRatio
                = (dialInfo.dialText.textAttr.xPosRatio * watchDiameter + step)
                  / watchDiameter;
            watch->attr()->dial = dialInfo;
        }
    }
}

void WatchCfger::up(bool checked) {
    if (watch) {
        bool enabledText = isEnabledText();
        if (enabledText) {
            QRectF viewRect = watch->view()->rect();
            double watchDiameter = qMin(viewRect.width(), viewRect.height());
            DialInfo dialInfo = watch->attr()->dial.value;
            dialInfo.dialText.textAttr.yPosRatio
                = (dialInfo.dialText.textAttr.yPosRatio * watchDiameter - step)
                  / watchDiameter;
            watch->attr()->dial = dialInfo;
        }
    }
}

bool WatchCfger::isEnabledText() {
    bool enabledText = watch->attr()->dial.value.dialText.enabledText;
    return enabledText;
}

QString WatchCfger::getText() {
    QString text
        = QString::fromStdWString(watch->attr()->dial.value.dialText.text);
    return text;
}

QFont WatchCfger::getTextFont() {
    QFont textFont;
    QString textFontName = QString::fromStdWString(
        watch->attr()->dial.value.dialText.textAttr.textFontName);
    if (textFontName.isEmpty()) {
        QFont textFont;
        textFontName = textFont.defaultFamily();
    }
    int textFontSize = watch->attr()->dial.value.dialText.textAttr.textFontSize;
    bool bold = watch->attr()->dial.value.dialText.textAttr.bold;
    bool italic = watch->attr()->dial.value.dialText.textAttr.italic;
    bool underLine = watch->attr()->dial.value.dialText.textAttr.underLine;
    textFont.setFamily(textFontName);
    textFont.setPixelSize(textFontSize);
    textFont.setBold(bold);
    textFont.setItalic(italic);
    textFont.setUnderline(underLine);
    return textFont;
}
