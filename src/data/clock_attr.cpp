#include "clock_attr.h"
#include "font_attr.h"


ClockAttr::ClockAttr(RegionAttr *owner) : SegmentAttr(owner) {
    type = CLOCK_SLICE;
    clock.value.clockTime.lineStyle = CLS_MULTI;
    clock.value.clockTime.displayFormat = CDF_DATE_WEEK_TIME;
    clock.value.clockTime.lineSpace = 0;

    clock.value.clockTime.dateFormat.dateAlignFormat = CDAF_YYYY_MM_DD;
    clock.value.clockTime.dateFormat.dateColor = ImageAttr::OPAQUE_VIOLET;
    clock.value.clockTime.dateFormat.dateFontName = L"";
    clock.value.clockTime.dateFormat.dateFontSize = FontAttr::FONT_SIZE_DEFAULT;
    ;
    clock.value.clockTime.dateFormat.dateUintFormat = CDUF_SLASH;

    clock.value.clockTime.weekFormat.clockWeekTextFormat = CWF_CHINESE;
    clock.value.clockTime.weekFormat.weekColor = ImageAttr::OPAQUE_DARKGREEN;
    clock.value.clockTime.weekFormat.weekFontName = L"";
    clock.value.clockTime.weekFormat.weekFontSize = FontAttr::FONT_SIZE_DEFAULT;
    ;

    clock.value.clockTime.timeFormat.timeAlignFormat = CTAF_HH_MM_SS;
    clock.value.clockTime.timeFormat.timeColor = ImageAttr::OPAQUE_ORANGERED;
    clock.value.clockTime.timeFormat.timeFontName = L"";
    clock.value.clockTime.timeFormat.timeFontSize = FontAttr::FONT_SIZE_DEFAULT;
    ;
    clock.value.clockTime.timeFormat.timeUintFormat = CTUF_COLON;

    clock.value.clockTime.clockTimeDiff.deltType = CDT_POSITIVE;
    clock.value.clockTime.clockTimeDiff.deltHour = 0;
    clock.value.clockTime.clockTimeDiff.deltMinute = 0;
    clock.value.clockTime.clockTimeDiff.deltSecond = 0;

    clock.value.clockText.enabledText = false;
    clock.value.clockText.text = L"";
    clock.value.clockText.textAttr.textFontName = L"";
    clock.value.clockText.textAttr.textFontSize = FontAttr::FONT_SIZE_DEFAULT;
    ;
    clock.value.clockText.textAttr.textColor = ImageAttr::OPAQUE_FIREBRICK;
    clock.value.clockText.textAttr.bold = false;
    clock.value.clockText.textAttr.italic = false;
    clock.value.clockText.textAttr.underLine = false;
    clock.value.clockText.textAttr.xPos = 0;
    clock.value.clockText.textAttr.yPos = 0;
}

ClockAttr::~ClockAttr() {
}

void ClockAttr::load(Xml::Element *element) {
    ClockInfo clockInfo;
    SegmentAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getNodeName()};
        if (tag == L"lineStyle") {
            clockInfo.clockTime.lineStyle = static_cast<ClockLineStyle>(
                std::stoi(child->getTextContent()));
        } else if (tag == L"displayFormat") {
            clockInfo.clockTime.displayFormat = static_cast<ClockDisplayFormat>(
                std::stoi(child->getTextContent()));
        } else if (tag == L"lineSpace") {
            clockInfo.clockTime.lineSpace = std::stoi(child->getTextContent());
        } else if (tag == L"dateAlignFormat") {
            clockInfo.clockTime.dateFormat.dateAlignFormat
                = static_cast<ClockDateAlignFormat>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"dateColor") {
            clockInfo.clockTime.dateFormat.dateColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"dateFontName") {
            clockInfo.clockTime.dateFormat.dateFontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"dateFontSize") {
            clockInfo.clockTime.dateFormat.dateFontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"dateUintFormat") {
            clockInfo.clockTime.dateFormat.dateUintFormat
                = static_cast<ClockDateUintFormat>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"clockWeekTextFormat") {
            clockInfo.clockTime.weekFormat.clockWeekTextFormat
                = static_cast<ClockWeekTextFormat>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"weekColor") {
            clockInfo.clockTime.weekFormat.weekColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"weekFontName") {
            clockInfo.clockTime.weekFormat.weekFontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"weekFontSize") {
            clockInfo.clockTime.weekFormat.weekFontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"timeAlignFormat") {
            clockInfo.clockTime.timeFormat.timeAlignFormat
                = static_cast<ClockTimeAlignFormat>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"timeColor") {
            clockInfo.clockTime.timeFormat.timeColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"timeFontName") {
            clockInfo.clockTime.timeFormat.timeFontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"timeFontSize") {
            clockInfo.clockTime.timeFormat.timeFontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"timeUintFormat") {
            clockInfo.clockTime.timeFormat.timeUintFormat
                = static_cast<ClockTimeUintFormat>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"deltType") {
            clockInfo.clockTime.clockTimeDiff.deltType
                = static_cast<ClockDeltType>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"deltHour") {
            clockInfo.clockTime.clockTimeDiff.deltHour
                = std::stoi(child->getTextContent());
        } else if (tag == L"deltMinute") {
            clockInfo.clockTime.clockTimeDiff.deltMinute
                = std::stoi(child->getTextContent());
        } else if (tag == L"deltSecond") {
            clockInfo.clockTime.clockTimeDiff.deltSecond
                = std::stoi(child->getTextContent());
        } else if (tag == L"enabledText") {
            clockInfo.clockText.enabledText
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"text") {
            clockInfo.clockText.text = std::wstring(child->getTextContent());
        } else if (tag == L"textFontName") {
            clockInfo.clockText.textAttr.textFontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"textFontSize") {
            clockInfo.clockText.textAttr.textFontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"textColor") {
            clockInfo.clockText.textAttr.textColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"bold") {
            clockInfo.clockText.textAttr.bold
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"italic") {
            clockInfo.clockText.textAttr.italic
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"underLine") {
            clockInfo.clockText.textAttr.underLine
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"xPos") {
            clockInfo.clockText.textAttr.xPos
                = std::stoi(child->getTextContent());
        } else if (tag == L"yPos") {
            clockInfo.clockText.textAttr.yPos
                = std::stoi(child->getTextContent());
        }
    }
    clock = clockInfo;
}

void ClockAttr::save(Xml::Doc *doc, Xml::Node *parent) {
    ClockInfo clockInfo = clock.value;
    Xml::Node *self = parent->appendChild(doc->createElement(L"clock"));
    SegmentAttr::save(doc, self);
    self->appendChild(doc->createElement(L"lineStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.lineStyle).c_str()));

    self->appendChild(doc->createElement(L"displayFormat"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.displayFormat).c_str()));
    self->appendChild(doc->createElement(L"lineSpace"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.lineSpace).c_str()));

    self->appendChild(doc->createElement(L"dateAlignFormat"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.dateFormat.dateAlignFormat)
                .c_str()));
    self->appendChild(doc->createElement(L"dateColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.dateFormat.dateColor).c_str()));
    self->appendChild(doc->createElement(L"dateFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(clockInfo.clockTime.dateFormat.dateFontName).c_str()));
    self->appendChild(doc->createElement(L"dateFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.dateFormat.dateFontSize)
                .c_str()));
    self->appendChild(doc->createElement(L"dateUintFormat"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.dateFormat.dateUintFormat)
                .c_str()));

    self->appendChild(doc->createElement(L"clockWeekTextFormat"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.weekFormat.clockWeekTextFormat)
                .c_str()));
    self->appendChild(doc->createElement(L"weekColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.weekFormat.weekColor).c_str()));
    self->appendChild(doc->createElement(L"weekFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(clockInfo.clockTime.weekFormat.weekFontName).c_str()));
    self->appendChild(doc->createElement(L"weekFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.weekFormat.weekFontSize)
                .c_str()));

    self->appendChild(doc->createElement(L"timeAlignFormat"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.timeFormat.timeAlignFormat)
                .c_str()));
    self->appendChild(doc->createElement(L"timeColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.timeFormat.timeColor).c_str()));
    self->appendChild(doc->createElement(L"timeFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(clockInfo.clockTime.timeFormat.timeFontName).c_str()));
    self->appendChild(doc->createElement(L"timeFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.timeFormat.timeFontSize)
                .c_str()));
    self->appendChild(doc->createElement(L"timeUintFormat"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.timeFormat.timeUintFormat)
                .c_str()));

    self->appendChild(doc->createElement(L"deltType"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.clockTimeDiff.deltType)
                .c_str()));
    self->appendChild(doc->createElement(L"deltHour"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.clockTimeDiff.deltHour)
                .c_str()));
    self->appendChild(doc->createElement(L"deltMinute"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.clockTimeDiff.deltMinute)
                .c_str()));
    self->appendChild(doc->createElement(L"deltSecond"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockTime.clockTimeDiff.deltSecond)
                .c_str()));

    self->appendChild(doc->createElement(L"enabledText"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockText.enabledText).c_str()));
    self->appendChild(doc->createElement(L"text"))->appendChild(
        doc->createTextNode(std::wstring(clockInfo.clockText.text).c_str()));
    self->appendChild(doc->createElement(L"textFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(clockInfo.clockText.textAttr.textFontName).c_str()));
    self->appendChild(doc->createElement(L"textFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockText.textAttr.textFontSize)
                .c_str()));
    self->appendChild(doc->createElement(L"textColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockText.textAttr.textColor).c_str()));
    self->appendChild(doc->createElement(L"bold"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockText.textAttr.bold).c_str()));
    self->appendChild(doc->createElement(L"italic"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockText.textAttr.italic).c_str()));
    self->appendChild(doc->createElement(L"underLine"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockText.textAttr.underLine).c_str()));
    self->appendChild(doc->createElement(L"xPos"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockText.textAttr.xPos).c_str()));
    self->appendChild(doc->createElement(L"yPos"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(clockInfo.clockText.textAttr.yPos).c_str()));
}
