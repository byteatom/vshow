#ifndef TIMERATTR_H
#define TIMERATTR_H

#include "image_attr.h"
#include "segment_attr.h"

typedef uint32_t TimerColor;

typedef enum { TIMER_UP, TIMER_DOWN } TimerType;

struct TimerTargetDate {
    int year;
    int month;
    int day;

public:
    TimerTargetDate operator=(const TimerTargetDate &newValue) {
        year = newValue.year;
        month = newValue.month;
        day = newValue.day;
        return *this;
    }
    bool operator!=(const TimerTargetDate &newValue) {
        if (year != newValue.year || month != newValue.month
            || day != newValue.day) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerTargetTime {
    int hour;
    int minute;
    int second;

public:
    TimerTargetTime operator=(const TimerTargetTime &newValue) {
        hour = newValue.hour;
        minute = newValue.minute;
        second = newValue.second;
        return *this;
    }
    bool operator!=(const TimerTargetTime &newValue) {
        if (hour != newValue.hour || minute != newValue.minute
            || second != newValue.second) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerTargetDateTime {
    TimerTargetDate date;
    TimerTargetTime time;

public:
    TimerTargetDateTime operator=(const TimerTargetDateTime &newValue) {
        date = newValue.date;
        time = newValue.time;
        return *this;
    }
    bool operator!=(const TimerTargetDateTime &newValue) {
        if (date != newValue.date || time != newValue.time) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerDisplayFormat {
    bool displayDay;
    bool displayHour;
    bool displayMinute;
    bool displaySecond;

public:
    TimerDisplayFormat operator=(const TimerDisplayFormat &newValue) {
        displayDay = newValue.displayDay;
        displayHour = newValue.displayHour;
        displayMinute = newValue.displayMinute;
        displaySecond = newValue.displaySecond;
        return *this;
    }
    bool operator!=(const TimerDisplayFormat &newValue) {
        if (displayDay != newValue.displayDay
            || displayHour != newValue.displayHour
            || displayMinute != newValue.displayMinute
            || displaySecond != newValue.displaySecond) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerTimeFont {
    std::wstring timeFontName;
    int timeFontSize;

public:
    TimerTimeFont operator=(const TimerTimeFont &newValue) {
        timeFontName = newValue.timeFontName;
        timeFontSize = newValue.timeFontSize;
        return *this;
    }
    bool operator!=(const TimerTimeFont &newValue) {
        if (timeFontName != newValue.timeFontName
            || timeFontSize != newValue.timeFontSize) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerTimeColor {
    TimerColor numColor;
    TimerColor unitColor;

public:
    TimerTimeColor operator=(const TimerTimeColor &newValue) {
        numColor = newValue.numColor;
        unitColor = newValue.unitColor;
        return *this;
    }
    bool operator!=(const TimerTimeColor &newValue) {
        if (numColor != newValue.numColor || unitColor != newValue.unitColor) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerTime {
    TimerType timerType;
    TimerTargetDateTime targetDateTime;
    TimerDisplayFormat displayFormat;
    TimerTimeFont timeFont;
    TimerTimeColor timeColor;

public:
    TimerTime operator=(const TimerTime &newValue) {
        timerType = newValue.timerType;
        targetDateTime = newValue.targetDateTime;
        displayFormat = newValue.displayFormat;
        timeFont = newValue.timeFont;
        timeColor = newValue.timeColor;
        return *this;
    }
    bool operator!=(const TimerTime &newValue) {
        if (timerType != newValue.timerType
            || targetDateTime != newValue.targetDateTime
            || displayFormat != newValue.displayFormat
            || timeFont != newValue.timeFont
            || timeColor != newValue.timeColor) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerFixedTextAttr {
    std::wstring textFontName;
    int textFontSize;
    TimerColor textColor;
    bool bold;
    bool italic;
    bool underLine;
    int xPos;
    int yPos;

public:
    TimerFixedTextAttr operator=(const TimerFixedTextAttr &newValue) {
        textFontName = newValue.textFontName;
        textFontSize = newValue.textFontSize;
        textColor = newValue.textColor;
        bold = newValue.bold;
        italic = newValue.italic;
        underLine = newValue.underLine;
        xPos = newValue.xPos;
        yPos = newValue.yPos;
        return *this;
    }
    bool operator!=(const TimerFixedTextAttr &newValue) {
        if (textFontName != newValue.textFontName
            || textFontSize != newValue.textFontSize
            || textColor != newValue.textColor || bold != newValue.bold
            || italic != newValue.italic || underLine != newValue.underLine
            || xPos != newValue.xPos || yPos != newValue.yPos) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerText {
    bool enabledText;
    bool newline;
    std::wstring text;
    TimerFixedTextAttr textAttr;

public:
    TimerText operator=(const TimerText &newValue) {
        enabledText = newValue.enabledText;
        newline = newValue.newline;
        text = newValue.text;
        textAttr = newValue.textAttr;
        return *this;
    }
    bool operator!=(const TimerText &newValue) {
        if (enabledText != newValue.enabledText || newline != newValue.newline
            || text != newValue.text || textAttr != newValue.textAttr) {
            return true;
        } else {
            return false;
        }
    }
};

struct TimerInfo {
    TimerTime timerTime;
    TimerText timerText;

public:
    TimerInfo operator=(const TimerInfo &newValue) {
        timerTime = newValue.timerTime;
        timerText = newValue.timerText;
        return *this;
    }
    bool operator!=(const TimerInfo &newValue) {
        if (timerTime != newValue.timerTime
            || timerText != newValue.timerText) {
            return true;
        } else {
            return false;
        }
    }
};

class TimerAttr : public SegmentAttr {
public:
    explicit TimerAttr(RegionAttr *owner);
    virtual ~TimerAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    Attr<TimerInfo> timer;
};

#endif    // TIMERATTR_H
