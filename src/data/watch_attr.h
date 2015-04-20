#ifndef WATCHATTR_H
#define WATCHATTR_H

#include "image_attr.h"
#include "segment_attr.h"

#define X_RATIO 0.50
#define Y_RATIO 0.75

typedef uint32_t DialColor;

typedef enum {
    DS_CIRCLE,
    DS_ROUNDRECT,
    DS_ELLIPSE,
    DS_RHOMBUS,
    DS_WINDMILL
} DialStyle;

typedef enum { DPS_CIRCLE, DPS_SQUARE, DPS_LINE } DialPointStyle;

typedef enum { DHS_LINE, DHS_RHOMBUS } DialHandStyle;

typedef enum { DNS_ARABIC, DNS_ROAMER } DialNumStyle;

typedef enum { DDT_YES = 1, DDT_NO = 0 } DialDisplayType;

typedef enum { DDT_POSITIVE, DDT_NEGATIVE } DialDeltType;

struct DialPointAttr {
    uint32_t pointSize;
    DialColor pointColor;
    DialPointStyle pointStyle;

public:
    DialPointAttr operator=(const DialPointAttr &newValue) {
        pointSize = newValue.pointSize;
        pointColor = newValue.pointColor;
        pointStyle = newValue.pointStyle;
        return *this;
    }
    bool operator!=(const DialPointAttr &newValue) {
        if (pointSize != newValue.pointSize || pointColor != newValue.pointColor
            || pointStyle != newValue.pointStyle) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialPoint {
    DialPointAttr hourPoint;
    DialPointAttr tsnPoint;
    DialPointAttr minutePoint;

public:
    DialPoint operator=(const DialPoint &newValue) {
        hourPoint = newValue.hourPoint;
        tsnPoint = newValue.tsnPoint;
        minutePoint = newValue.minutePoint;
        return *this;
    }
    bool operator!=(const DialPoint &newValue) {
        if (hourPoint != newValue.hourPoint || tsnPoint != newValue.tsnPoint
            || minutePoint != newValue.minutePoint) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialHandAttr {
    uint32_t handSize;
    DialColor handColor;
    DialHandStyle handStyle;

public:
    DialHandAttr operator=(const DialHandAttr &newValue) {
        handSize = newValue.handSize;
        handColor = newValue.handColor;
        handStyle = newValue.handStyle;
        return *this;
    }
    bool operator!=(const DialHandAttr &newValue) {
        if (handSize != newValue.handSize || handColor != newValue.handColor
            || handStyle != newValue.handStyle) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialHand {
    DialHandAttr hourHand;
    DialHandAttr minuteHand;
    DialHandAttr secondHand;

public:
    DialHand operator=(const DialHand &newValue) {
        hourHand = newValue.hourHand;
        minuteHand = newValue.minuteHand;
        secondHand = newValue.secondHand;
        return *this;
    }
    bool operator!=(const DialHand &newValue) {
        if (hourHand != newValue.hourHand || minuteHand != newValue.minuteHand
            || secondHand != newValue.secondHand) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialNumAttr {
    DialNumStyle numStyle;
    DialColor numColor;
    std::wstring fontName;
    int fontSize;
    DialDisplayType displayType;

public:
    DialNumAttr operator=(const DialNumAttr &newValue) {
        numStyle = newValue.numStyle;
        numColor = newValue.numColor;
        fontName = newValue.fontName;
        fontSize = newValue.fontSize;
        displayType = newValue.displayType;
        return *this;
    }
    bool operator!=(const DialNumAttr &newValue) {
        if (numStyle != newValue.numStyle || numColor != newValue.numColor
            || fontName != newValue.fontName || fontSize != newValue.fontSize
            || displayType != newValue.displayType) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialNum {
    DialNumAttr hourNum;
    DialNumAttr tsnNum;

public:
    DialNum operator=(const DialNum &newValue) {
        hourNum = newValue.hourNum;
        tsnNum = newValue.tsnNum;
        return *this;
    }
    bool operator!=(const DialNum &newValue) {
        if (hourNum != newValue.hourNum || tsnNum != newValue.tsnNum) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialTimeDiff {
    DialDeltType deltType;
    int deltHour;
    int deltMinute;
    int deltSecond;

public:
    DialTimeDiff operator=(const DialTimeDiff &newValue) {
        deltType = newValue.deltType;
        deltHour = newValue.deltHour;
        deltMinute = newValue.deltMinute;
        deltSecond = newValue.deltSecond;
        return *this;
    }
    bool operator!=(const DialTimeDiff &newValue) {
        if (deltType != newValue.deltType || deltHour != newValue.deltHour
            || deltMinute != newValue.deltMinute
            || deltSecond != newValue.deltSecond) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialTime {
    DialPoint dialPoint;
    DialHand dialHand;
    DialNum dialNum;
    DialTimeDiff dialTimeDiff;

public:
    DialTime operator=(const DialTime &newValue) {
        dialPoint = newValue.dialPoint;
        dialHand = newValue.dialHand;
        dialNum = newValue.dialNum;
        dialTimeDiff = newValue.dialTimeDiff;
        return *this;
    }
    bool operator!=(const DialTime &newValue) {
        if (dialPoint != newValue.dialPoint || dialHand != newValue.dialHand
            || dialNum != newValue.dialNum
            || dialTimeDiff != newValue.dialTimeDiff) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialFixedTextAttr {
    std::wstring textFontName;
    int textFontSize;
    DialColor textColor;
    bool bold;
    bool italic;
    bool underLine;
    double xPosRatio;
    double yPosRatio;

public:
    DialFixedTextAttr operator=(const DialFixedTextAttr &newValue) {
        textFontName = newValue.textFontName;
        textFontSize = newValue.textFontSize;
        textColor = newValue.textColor;
        bold = newValue.bold;
        italic = newValue.italic;
        underLine = newValue.underLine;
        xPosRatio = newValue.xPosRatio;
        yPosRatio = newValue.yPosRatio;
        return *this;
    }
    bool operator!=(const DialFixedTextAttr &newValue) {
        if (textFontName != newValue.textFontName
            || textFontSize != newValue.textFontSize
            || textColor != newValue.textColor || bold != newValue.bold
            || italic != newValue.italic || underLine != newValue.underLine
            || xPosRatio != newValue.xPosRatio
            || yPosRatio != newValue.yPosRatio) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialText {
    bool enabledText;
    std::wstring text;
    DialFixedTextAttr textAttr;

public:
    DialText operator=(const DialText &newValue) {
        enabledText = newValue.enabledText;
        text = newValue.text;
        textAttr = newValue.textAttr;
        return *this;
    }
    bool operator!=(const DialText &newValue) {
        if (enabledText != newValue.enabledText || text != newValue.text
            || textAttr != newValue.textAttr) {
            return true;
        } else {
            return false;
        }
    }
};

struct DialInfo {
    DialStyle dialStyle;
    DialTime dialTime;
    DialText dialText;

public:
    DialInfo operator=(const DialInfo &newValue) {
        dialStyle = newValue.dialStyle;
        dialTime = newValue.dialTime;
        dialText = newValue.dialText;
        return *this;
    }
    bool operator!=(const DialInfo &newValue) {
        if (dialStyle != newValue.dialStyle || dialTime != newValue.dialTime
            || dialText != newValue.dialText) {
            return true;
        } else {
            return false;
        }
    }
};

class WatchAttr : public SegmentAttr {
public:
    explicit WatchAttr(RegionAttr *owner);
    virtual ~WatchAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    Attr<DialInfo> dial;
};

#endif    // WATCHATTR_H
