#include "timer_attr.h"
#include "font_attr.h"


TimerAttr::TimerAttr(RegionAttr *owner) : SegmentAttr(owner) {
    type = TIMER_SLICE;
    timer.value.timerTime.timerType = TIMER_UP;
    timer.value.timerTime.targetDateTime.date.year = 2015;
    timer.value.timerTime.targetDateTime.date.month = 1;
    timer.value.timerTime.targetDateTime.date.day = 1;
    timer.value.timerTime.targetDateTime.time.hour = 0;
    timer.value.timerTime.targetDateTime.time.minute = 0;
    timer.value.timerTime.targetDateTime.time.second = 0;
    timer.value.timerTime.displayFormat.displayDay = true;
    timer.value.timerTime.displayFormat.displayHour = true;
    timer.value.timerTime.displayFormat.displayMinute = true;
    timer.value.timerTime.displayFormat.displaySecond = true;
    timer.value.timerTime.timeFont.timeFontName = L"";
    timer.value.timerTime.timeFont.timeFontSize = FontAttr::FONT_SIZE_DEFAULT;
    timer.value.timerTime.timeColor.numColor = ImageAttr::OPAQUE_VIOLET;
    timer.value.timerTime.timeColor.unitColor = ImageAttr::OPAQUE_SEAGREEN;
    timer.value.timerText.enabledText = false;
    timer.value.timerText.newline = false;
    timer.value.timerText.text = L"";
    timer.value.timerText.textAttr.textFontName = L"";
    timer.value.timerText.textAttr.textFontSize = FontAttr::FONT_SIZE_DEFAULT;
    ;
    timer.value.timerText.textAttr.textColor = ImageAttr::OPAQUE_FIREBRICK;
    timer.value.timerText.textAttr.bold = false;
    timer.value.timerText.textAttr.italic = false;
    timer.value.timerText.textAttr.underLine = false;
    timer.value.timerText.textAttr.xPos = 0;
    timer.value.timerText.textAttr.yPos = 0;
}

TimerAttr::~TimerAttr() {
}

void TimerAttr::load(Xml::Element *element) {
    TimerInfo timerInfo;
    SegmentAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getNodeName()};
        if (tag == L"timerType") {
            timerInfo.timerTime.timerType
                = static_cast<TimerType>(std::stoi(child->getTextContent()));
        } else if (tag == L"year") {
            timerInfo.timerTime.targetDateTime.date.year
                = std::stoi(child->getTextContent());
        } else if (tag == L"month") {
            timerInfo.timerTime.targetDateTime.date.month
                = std::stoi(child->getTextContent());
        } else if (tag == L"day") {
            timerInfo.timerTime.targetDateTime.date.day
                = std::stoi(child->getTextContent());
        } else if (tag == L"hour") {
            timerInfo.timerTime.targetDateTime.time.hour
                = std::stoi(child->getTextContent());
        } else if (tag == L"minute") {
            timerInfo.timerTime.targetDateTime.time.minute
                = std::stoi(child->getTextContent());
        } else if (tag == L"second") {
            timerInfo.timerTime.targetDateTime.time.second
                = std::stoi(child->getTextContent());
        } else if (tag == L"displayDay") {
            timerInfo.timerTime.displayFormat.displayDay
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"displayHour") {
            timerInfo.timerTime.displayFormat.displayHour
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"displayMinute") {
            timerInfo.timerTime.displayFormat.displayMinute
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"displaySecond") {
            timerInfo.timerTime.displayFormat.displaySecond
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"timeFontName") {
            timerInfo.timerTime.timeFont.timeFontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"timeFontSize") {
            timerInfo.timerTime.timeFont.timeFontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"numColor") {
            timerInfo.timerTime.timeColor.numColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"unitColor") {
            timerInfo.timerTime.timeColor.unitColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"enabledText") {
            timerInfo.timerText.enabledText
                = std::stoi(child->getTextContent());
        } else if (tag == L"newline") {
            timerInfo.timerText.newline = std::stoi(child->getTextContent());
        } else if (tag == L"text") {
            timerInfo.timerText.text = std::wstring(child->getTextContent());
        } else if (tag == L"textFontName") {
            timerInfo.timerText.textAttr.textFontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"textFontSize") {
            timerInfo.timerText.textAttr.textFontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"textColor") {
            timerInfo.timerText.textAttr.textColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"bold") {
            timerInfo.timerText.textAttr.bold
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"italic") {
            timerInfo.timerText.textAttr.italic
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"underLine") {
            timerInfo.timerText.textAttr.underLine
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"xPos") {
            timerInfo.timerText.textAttr.xPos
                = std::stoi(child->getTextContent());
        } else if (tag == L"yPos") {
            timerInfo.timerText.textAttr.yPos
                = std::stoi(child->getTextContent());
        }
    }
    timer = timerInfo;
}

void TimerAttr::save(Xml::Doc *doc, Xml::Node *parent) {
    TimerInfo timerInfo = timer.value;
    Xml::Node *self = parent->appendChild(doc->createElement(L"timer"));
    SegmentAttr::save(doc, self);
    self->appendChild(doc->createElement(L"timerType"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.timerType).c_str()));

    self->appendChild(doc->createElement(L"year"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.targetDateTime.date.year)
                .c_str()));
    self->appendChild(doc->createElement(L"month"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.targetDateTime.date.month)
                .c_str()));
    self->appendChild(doc->createElement(L"day"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.targetDateTime.date.day)
                .c_str()));
    self->appendChild(doc->createElement(L"hour"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.targetDateTime.time.hour)
                .c_str()));
    self->appendChild(doc->createElement(L"minute"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.targetDateTime.time.minute)
                .c_str()));
    self->appendChild(doc->createElement(L"second"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.targetDateTime.time.second)
                .c_str()));

    self->appendChild(doc->createElement(L"displayDay"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.displayFormat.displayDay)
                .c_str()));
    self->appendChild(doc->createElement(L"displayHour"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.displayFormat.displayHour)
                .c_str()));
    self->appendChild(doc->createElement(L"displayMinute"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.displayFormat.displayMinute)
                .c_str()));
    self->appendChild(doc->createElement(L"displaySecond"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.displayFormat.displaySecond)
                .c_str()));

    self->appendChild(doc->createElement(L"timeFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(timerInfo.timerTime.timeFont.timeFontName).c_str()));
    self->appendChild(doc->createElement(L"timeFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.timeFont.timeFontSize)
                .c_str()));
    self->appendChild(doc->createElement(L"numColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.timeColor.numColor).c_str()));
    self->appendChild(doc->createElement(L"unitColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerTime.timeColor.unitColor).c_str()));

    self->appendChild(doc->createElement(L"enabledText"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.enabledText).c_str()));
    self->appendChild(doc->createElement(L"newline"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.newline).c_str()));
    self->appendChild(doc->createElement(L"text"))->appendChild(
        doc->createTextNode(std::wstring(timerInfo.timerText.text).c_str()));
    self->appendChild(doc->createElement(L"textFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(timerInfo.timerText.textAttr.textFontName).c_str()));
    self->appendChild(doc->createElement(L"textFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.textAttr.textFontSize)
                .c_str()));
    self->appendChild(doc->createElement(L"textColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.textAttr.textColor).c_str()));
    self->appendChild(doc->createElement(L"bold"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.textAttr.bold).c_str()));
    self->appendChild(doc->createElement(L"italic"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.textAttr.italic).c_str()));
    self->appendChild(doc->createElement(L"underLine"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.textAttr.underLine).c_str()));
    self->appendChild(doc->createElement(L"xPos"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.textAttr.xPos).c_str()));
    self->appendChild(doc->createElement(L"yPos"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(timerInfo.timerText.textAttr.yPos).c_str()));
}
