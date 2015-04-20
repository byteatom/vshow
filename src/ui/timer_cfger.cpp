#include "timer_cfger.h"
#include "ui_timer_cfger.h"
#include <QColorDialog>
#include "data/font_attr.h"


TimerCfger::TimerCfger() : ui(new Ui::TimerCfger), timer(nullptr) {
    ui->setupUi(this);
    timerTypeBtns.addButton(ui->upTimer);
    timerTypeBtns.addButton(ui->downTimer);
    ui->downTimer->setChecked(true);
    ui->targetDate->setDate(QDate::currentDate());
    ui->targetTime->setTime(QTime::currentTime());
    ui->day->setChecked(true);
    ui->hour->setChecked(true);
    ui->minute->setChecked(true);
    ui->second->setChecked(true);
    ui->timeFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                               FontAttr::FONT_SIZE_MAX);
    ui->timeFontSize->setValue(FontAttr::FONT_SIZE_DEFAULT);
    ui->enabledText->setChecked(false);
    ui->newline->setChecked(false);
    ui->lineText->setText("");
    ui->textFontSize->setRange(FontAttr::FONT_SIZE_MIN,
                               FontAttr::FONT_SIZE_MAX);
    connect(ui->upTimer, SIGNAL(toggled(bool)), this, SLOT(upTimer(bool)));
    connect(ui->downTimer, SIGNAL(toggled(bool)), this, SLOT(downTimer(bool)));
    connect(ui->targetDate,
            SIGNAL(dateChanged(const QDate &)),
            this,
            SLOT(targetDate(const QDate &)));
    connect(ui->targetTime,
            SIGNAL(timeChanged(const QTime &)),
            this,
            SLOT(targetTime(const QTime &)));
    connect(ui->day, SIGNAL(toggled(bool)), this, SLOT(day(bool)));
    connect(ui->hour, SIGNAL(toggled(bool)), this, SLOT(hour(bool)));
    connect(ui->minute, SIGNAL(toggled(bool)), this, SLOT(minute(bool)));
    connect(ui->second, SIGNAL(toggled(bool)), this, SLOT(second(bool)));
    connect(ui->timeFont,
            SIGNAL(currentFontChanged(const QFont &)),
            this,
            SLOT(timeFont(const QFont &)));
    connect(ui->timeFontSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(timeFontSize(int)));
    connect(ui->timeNumColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(timeNumColor(bool)));
    connect(ui->timeUnitColor,
            SIGNAL(clicked(bool)),
            this,
            SLOT(timeUnitColor(bool)));
    connect(
        ui->enabledText, SIGNAL(toggled(bool)), this, SLOT(enabledText(bool)));
    connect(ui->newline, SIGNAL(toggled(bool)), this, SLOT(newline(bool)));
    connect(ui->lineText,
            SIGNAL(textChanged(const QString &)),
            this,
            SLOT(lineText(const QString &)));
    connect(ui->textFont,
            SIGNAL(currentFontChanged(const QFont &)),
            this,
            SLOT(textFont(const QFont &)));
    connect(ui->textFontSize,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(textFontSize(int)));
    connect(ui->textColor, SIGNAL(clicked(bool)), this, SLOT(textColor(bool)));
    connect(ui->bold, SIGNAL(clicked(bool)), this, SLOT(bold(bool)));
    connect(ui->italic, SIGNAL(clicked(bool)), this, SLOT(italic(bool)));
    connect(ui->underLine, SIGNAL(clicked(bool)), this, SLOT(underLine(bool)));
}

TimerCfger::~TimerCfger() {
    delete ui;
}

void TimerCfger::show(Timer *timer) {
    this->timer = timer;
    ui->upTimer->setChecked(
        TIMER_UP == this->timer->attr()->timer.value.timerTime.timerType);
    ui->downTimer->setChecked(
        TIMER_DOWN == this->timer->attr()->timer.value.timerTime.timerType);
    int year
        = this->timer->attr()->timer.value.timerTime.targetDateTime.date.year;
    int month
        = this->timer->attr()->timer.value.timerTime.targetDateTime.date.month;
    int day = this->timer->attr()->timer.value.timerTime.targetDateTime.date.day;
    QDate targetDate(year, month, day);
    ui->targetDate->setDate(targetDate);
    int hour
        = this->timer->attr()->timer.value.timerTime.targetDateTime.time.hour;
    int minute
        = this->timer->attr()->timer.value.timerTime.targetDateTime.time.minute;
    int second
        = this->timer->attr()->timer.value.timerTime.targetDateTime.time.second;
    QTime targetTime(hour, minute, second);
    ui->targetTime->setTime(targetTime);
    ui->day->setChecked(
        this->timer->attr()->timer.value.timerTime.displayFormat.displayDay);
    ui->hour->setChecked(
        this->timer->attr()->timer.value.timerTime.displayFormat.displayHour);
    ui->minute->setChecked(
        this->timer->attr()->timer.value.timerTime.displayFormat.displayMinute);
    ui->second->setChecked(
        this->timer->attr()->timer.value.timerTime.displayFormat.displaySecond);
    QString timeFontName = QString::fromStdWString(
        this->timer->attr()->timer.value.timerTime.timeFont.timeFontName);
    if (timeFontName.isEmpty()) {
        ui->timeFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->timeFont->setCurrentText(timeFontName);
    }
    ui->timeFontSize->setValue(
        this->timer->attr()->timer.value.timerTime.timeFont.timeFontSize);
    ui->enabledText->setChecked(
        this->timer->attr()->timer.value.timerText.enabledText);
    ui->newline->setChecked(this->timer->attr()->timer.value.timerText.newline);
    ui->lineText->setText(
        QString::fromStdWString(this->timer->attr()->timer.value.timerText.text));
    QString textFontName;
    textFontName = QString::fromStdWString(
        this->timer->attr()->timer.value.timerText.textAttr.textFontName);
    if (textFontName.isEmpty()) {
        ui->textFont->setCurrentIndex(FontAttr::FONT_INDEX_DEFAULT);
    } else {
        ui->textFont->setCurrentText(textFontName);
    }
    ui->textFontSize->setValue(
        this->timer->attr()->timer.value.timerText.textAttr.textFontSize);
    AttrCfger::show();
}

void TimerCfger::hide()
{
    timer=nullptr;
    AttrCfger::hide();
}


void TimerCfger::upTimer(bool checked) {
    if (timer && checked) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.timerType = TIMER_UP;
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::downTimer(bool checked) {
    if (timer && checked) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.timerType = TIMER_DOWN;
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::targetDate(const QDate &date) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.targetDateTime.date.year = date.year();
        timerInfo.timerTime.targetDateTime.date.month = date.month();
        timerInfo.timerTime.targetDateTime.date.day = date.day();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::targetTime(const QTime &time) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.targetDateTime.time.hour = time.hour();
        timerInfo.timerTime.targetDateTime.time.minute = time.minute();
        timerInfo.timerTime.targetDateTime.time.second = time.second();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::day(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.displayFormat.displayDay = ui->day->isChecked();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::hour(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.displayFormat.displayHour = ui->hour->isChecked();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::minute(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.displayFormat.displayMinute
            = ui->minute->isChecked();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::second(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.displayFormat.displaySecond
            = ui->second->isChecked();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::timeFont(const QFont &f) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.timeFont.timeFontName = f.family().toStdWString();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::timeFontSize(int) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerTime.timeFont.timeFontSize = ui->timeFontSize->value();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::timeNumColor(bool checked) {
    if (timer) {
        QColorDialog colorDlg;
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        colorDlg.setWindowTitle(tr("Select Number Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(timerInfo.timerTime.timeColor.numColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            timerInfo.timerTime.timeColor.numColor
                = colorDlg.currentColor().rgba();
            this->timer->attr()->timer = timerInfo;
        }
    }
}

void TimerCfger::timeUnitColor(bool checked) {
    if (timer) {
        QColorDialog colorDlg;
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        colorDlg.setWindowTitle(tr("Select Unit Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(timerInfo.timerTime.timeColor.unitColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            timerInfo.timerTime.timeColor.unitColor
                = colorDlg.currentColor().rgba();
            this->timer->attr()->timer = timerInfo;
        }
    }
}

void TimerCfger::enabledText(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerText.enabledText = ui->enabledText->isChecked();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::newline(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerText.newline = ui->newline->isChecked();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::lineText(const QString &) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerText.text = ui->lineText->text().toStdWString();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::textFont(const QFont &f) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerText.textAttr.textFontName = f.family().toStdWString();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::textFontSize(int) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerText.textAttr.textFontSize = ui->textFontSize->value();
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::textColor(bool checked) {
    if (timer) {
        QColorDialog colorDlg;
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        colorDlg.setWindowTitle(tr("Select Text Color"));
        colorDlg.setOptions(colorDlg.options()
                            | QColorDialog::ShowAlphaChannel);
        QColor currentColor;
        currentColor.setRgba(timerInfo.timerText.textAttr.textColor);
        colorDlg.setCurrentColor(currentColor);
        if (colorDlg.exec()) {
            timerInfo.timerText.textAttr.textColor
                = colorDlg.currentColor().rgba();
            this->timer->attr()->timer = timerInfo;
        }
    }
}

void TimerCfger::bold(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerText.textAttr.bold = !timerInfo.timerText.textAttr.bold;
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::italic(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerText.textAttr.italic
            = !timerInfo.timerText.textAttr.italic;
        this->timer->attr()->timer = timerInfo;
    }
}

void TimerCfger::underLine(bool checked) {
    if (timer) {
        TimerInfo timerInfo = this->timer->attr()->timer.value;
        timerInfo.timerText.textAttr.underLine
            = !timerInfo.timerText.textAttr.underLine;
        this->timer->attr()->timer = timerInfo;
    }
}
