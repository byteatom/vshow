#ifndef CLOCKATTR_H
#define CLOCKATTR_H

#include "image_attr.h"
#include "segment_attr.h"

typedef uint32_t ClockColor;

typedef enum { CLS_SINGLE, CLS_MULTI } ClockLineStyle;

typedef enum {
    CDF_DATE_WEEK_TIME,
    CDF_DATE_TIME_WEEK,
    CDF_WEEK_DATE_TIME,
    CDF_WEEK_TIME_DATE,
    CDF_TIME_DATE_WEEK,
    CDF_TIME_WEEK_DATE
} ClockDisplayFormat;

typedef enum {
    CDAF_YYYY_MM_DD,
    CDAF_YYYY_DD_MM,
    CDAF_MM_YYYY_DD,
    CDAF_MM_DD_YYYY,
    CDAF_DD_YYYY_MM,
    CDAF_DD_MM_YYYY
} ClockDateAlignFormat;

typedef enum {
    CDUF_SLASH,
    CDUF_CHINESE,
    CDUF_CHINESE_TW,
    CDUF_COLON
} ClockDateUintFormat;

typedef enum { CWF_CHINESE, CWF_CHINESE_TW, CWF_ENGLISH } ClockWeekTextFormat;

typedef enum {
    CTAF_HH_MM_SS,
    CTAF_HH_MM,
} ClockTimeAlignFormat;

typedef enum {
    CTUF_COLON,
    CTUF_CHINESE,
    CTUF_CHINESE_TW,
} ClockTimeUintFormat;

typedef enum { CDT_POSITIVE, CDT_NEGATIVE } ClockDeltType;

struct ClockDateFormat {
    ClockDateAlignFormat dateAlignFormat;
    ClockDateUintFormat dateUintFormat;
    ClockColor dateColor;
    std::wstring dateFontName;
    int dateFontSize;

public:
    ClockDateFormat operator=(const ClockDateFormat &newValue) {
        dateAlignFormat = newValue.dateAlignFormat;
        dateUintFormat = newValue.dateUintFormat;
        dateColor = newValue.dateColor;
        dateFontName = newValue.dateFontName;
        dateFontSize = newValue.dateFontSize;
        return *this;
    }
    bool operator!=(const ClockDateFormat &newValue) {
        if (dateAlignFormat != newValue.dateAlignFormat
            || dateUintFormat != newValue.dateUintFormat
            || dateColor != newValue.dateColor
            || dateFontName != newValue.dateFontName
            || dateFontSize != newValue.dateFontSize) {
            return true;
        } else {
            return false;
        }
    }
};

struct ClockWeekFormat {
    ClockWeekTextFormat clockWeekTextFormat;
    ClockColor weekColor;
    std::wstring weekFontName;
    int weekFontSize;

public:
    ClockWeekFormat operator=(const ClockWeekFormat &newValue) {
        clockWeekTextFormat = newValue.clockWeekTextFormat;
        weekColor = newValue.weekColor;
        weekFontName = newValue.weekFontName;
        weekFontSize = newValue.weekFontSize;
        return *this;
    }
    bool operator!=(const ClockWeekFormat &newValue) {
        if (clockWeekTextFormat != newValue.clockWeekTextFormat
            || weekColor != newValue.weekColor
            || weekFontName != newValue.weekFontName
            || weekFontSize != newValue.weekFontSize) {
            return true;
        } else {
            return false;
        }
    }
};

struct ClockTimeFormat {
    ClockTimeAlignFormat timeAlignFormat;
    ClockTimeUintFormat timeUintFormat;
    ClockColor timeColor;
    std::wstring timeFontName;
    int timeFontSize;

public:
    ClockTimeFormat operator=(const ClockTimeFormat &newValue) {
        timeAlignFormat = newValue.timeAlignFormat;
        timeUintFormat = newValue.timeUintFormat;
        timeColor = newValue.timeColor;
        timeFontName = newValue.timeFontName;
        timeFontSize = newValue.timeFontSize;
        return *this;
    }
    bool operator!=(const ClockTimeFormat &newValue) {
        if (timeAlignFormat != newValue.timeAlignFormat
            || timeUintFormat != newValue.timeUintFormat
            || timeColor != newValue.timeColor
            || timeFontName != newValue.timeFontName
            || timeFontSize != newValue.timeFontSize) {
            return true;
        } else {
            return false;
        }
    }
};

struct ClockTimeDiff {
    ClockDeltType deltType;
    int deltHour;
    int deltMinute;
    int deltSecond;

public:
    ClockTimeDiff operator=(const ClockTimeDiff &newValue) {
        deltType = newValue.deltType;
        deltHour = newValue.deltHour;
        deltMinute = newValue.deltMinute;
        deltSecond = newValue.deltSecond;
        return *this;
    }
    bool operator!=(const ClockTimeDiff &newValue) {
        if (deltType != newValue.deltType || deltHour != newValue.deltHour
            || deltMinute != newValue.deltMinute
            || deltSecond != newValue.deltSecond) {
            return true;
        } else {
            return false;
        }
    }
};

struct ClockTime {
    ClockLineStyle lineStyle;
    int lineSpace;
    ClockDisplayFormat displayFormat;
    ClockDateFormat dateFormat;
    ClockWeekFormat weekFormat;
    ClockTimeFormat timeFormat;
    ClockTimeDiff clockTimeDiff;

public:
    ClockTime operator=(const ClockTime &newValue) {
        lineStyle = newValue.lineStyle;
        lineSpace = newValue.lineSpace;
        displayFormat = newValue.displayFormat;
        dateFormat = newValue.dateFormat;
        weekFormat = newValue.weekFormat;
        timeFormat = newValue.timeFormat;
        clockTimeDiff = newValue.clockTimeDiff;
        return *this;
    }
    bool operator!=(const ClockTime &newValue) {
        if (lineStyle != newValue.lineStyle || lineSpace != newValue.lineSpace
            || displayFormat != newValue.displayFormat
            || dateFormat != newValue.dateFormat
            || weekFormat != newValue.weekFormat
            || timeFormat != newValue.timeFormat
            || clockTimeDiff != newValue.clockTimeDiff) {
            return true;
        } else {
            return false;
        }
    }
};

struct ClockTextAttr {
    std::wstring textFontName;
    int textFontSize;
    ClockColor textColor;
    bool bold;
    bool italic;
    bool underLine;
    int xPos;
    int yPos;

public:
    ClockTextAttr operator=(const ClockTextAttr &newValue) {
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
    bool operator!=(const ClockTextAttr &newValue) {
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

struct ClockText {
    bool enabledText;
    std::wstring text;
    ClockTextAttr textAttr;

public:
    ClockText operator=(const ClockText &newValue) {
        enabledText = newValue.enabledText;
        text = newValue.text;
        textAttr = newValue.textAttr;
        return *this;
    }
    bool operator!=(const ClockText &newValue) {
        if (enabledText != newValue.enabledText || text != newValue.text
            || textAttr != newValue.textAttr) {
            return true;
        } else {
            return false;
        }
    }
};

struct ClockInfo {
    ClockTime clockTime;
    ClockText clockText;

public:
    ClockInfo operator=(const ClockInfo &newValue) {
        clockTime = newValue.clockTime;
        clockText = newValue.clockText;
        return *this;
    }
    bool operator!=(const ClockInfo &newValue) {
        if (clockTime != newValue.clockTime
            || clockText != newValue.clockText) {
            return true;
        } else {
            return false;
        }
    }
};

class ClockAttr : public SegmentAttr {
public:
    explicit ClockAttr(RegionAttr *owner);
    virtual ~ClockAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    Attr<ClockInfo> clock;
};

#endif    // CLOCKATTR_H
