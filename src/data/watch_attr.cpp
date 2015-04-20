#include "watch_attr.h"
#include "font_attr.h"


WatchAttr::WatchAttr(RegionAttr *owner) : SegmentAttr(owner) {
    type = WATCH_SLICE;

    dial.value.dialStyle = DS_CIRCLE;

    dial.value.dialTime.dialPoint.hourPoint.pointSize = 3;
    dial.value.dialTime.dialPoint.hourPoint.pointColor
        = ImageAttr::OPAQUE_ROYALBLUE;
    dial.value.dialTime.dialPoint.hourPoint.pointStyle = DPS_SQUARE;
    dial.value.dialTime.dialPoint.tsnPoint.pointSize = 5;
    dial.value.dialTime.dialPoint.tsnPoint.pointColor
        = ImageAttr::OPAQUE_STEELBLUE;
    dial.value.dialTime.dialPoint.tsnPoint.pointStyle = DPS_SQUARE;
    dial.value.dialTime.dialPoint.minutePoint.pointSize = 1;
    dial.value.dialTime.dialPoint.minutePoint.pointColor
        = ImageAttr::OPAQUE_BLUE;
    dial.value.dialTime.dialPoint.minutePoint.pointStyle = DPS_SQUARE;

    dial.value.dialTime.dialHand.hourHand.handSize = 5;
    dial.value.dialTime.dialHand.hourHand.handColor
        = ImageAttr::OPAQUE_DARKGREEN;
    dial.value.dialTime.dialHand.hourHand.handStyle = DHS_LINE;
    dial.value.dialTime.dialHand.minuteHand.handSize = 3;
    dial.value.dialTime.dialHand.minuteHand.handColor
        = ImageAttr::OPAQUE_VIOLET;
    dial.value.dialTime.dialHand.minuteHand.handStyle = DHS_LINE;
    dial.value.dialTime.dialHand.secondHand.handSize = 2;
    dial.value.dialTime.dialHand.secondHand.handColor
        = ImageAttr::OPAQUE_FIREBRICK;
    dial.value.dialTime.dialHand.secondHand.handStyle = DHS_LINE;

    dial.value.dialTime.dialNum.hourNum.displayType = DDT_YES;
    dial.value.dialTime.dialNum.hourNum.fontName = L"";
    dial.value.dialTime.dialNum.hourNum.fontSize = FontAttr::FONT_SIZE_DEFAULT;
    ;
    dial.value.dialTime.dialNum.hourNum.numColor = ImageAttr::OPAQUE_INDIGO;
    dial.value.dialTime.dialNum.hourNum.numStyle = DNS_ARABIC;

    dial.value.dialTime.dialNum.tsnNum.displayType = DDT_YES;
    dial.value.dialTime.dialNum.tsnNum.fontName = L"";
    dial.value.dialTime.dialNum.tsnNum.fontSize = FontAttr::FONT_SIZE_DEFAULT;
    ;
    dial.value.dialTime.dialNum.tsnNum.numColor = ImageAttr::OPAQUE_FIREBRICK;
    dial.value.dialTime.dialNum.tsnNum.numStyle = DNS_ARABIC;

    dial.value.dialTime.dialTimeDiff.deltType = DDT_POSITIVE;
    dial.value.dialTime.dialTimeDiff.deltHour = 0;
    dial.value.dialTime.dialTimeDiff.deltMinute = 0;
    dial.value.dialTime.dialTimeDiff.deltSecond = 0;

    dial.value.dialText.enabledText = false;
    dial.value.dialText.text = L"";
    dial.value.dialText.textAttr.bold = false;
    dial.value.dialText.textAttr.italic = false;
    dial.value.dialText.textAttr.underLine = false;
    dial.value.dialText.textAttr.textFontName = L"";
    dial.value.dialText.textAttr.textFontSize = FontAttr::FONT_SIZE_DEFAULT;
    ;
    dial.value.dialText.textAttr.textColor = ImageAttr::OPAQUE_FIREBRICK;
    dial.value.dialText.textAttr.xPosRatio = X_RATIO;
    dial.value.dialText.textAttr.yPosRatio = Y_RATIO;
}

WatchAttr::~WatchAttr() {
}

void WatchAttr::load(Xml::Element *element) {
    DialInfo dialInfo;
    SegmentAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getNodeName()};
        if (tag == L"dialStyle") {
            dialInfo.dialStyle
                = static_cast<DialStyle>(std::stoi(child->getTextContent()));
        } else if (tag == L"hourPointSize") {
            dialInfo.dialTime.dialPoint.hourPoint.pointSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"hourPointColor") {
            dialInfo.dialTime.dialPoint.hourPoint.pointColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"hourPointStyle") {
            dialInfo.dialTime.dialPoint.hourPoint.pointStyle
                = static_cast<DialPointStyle>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"tsnPointSize") {
            dialInfo.dialTime.dialPoint.tsnPoint.pointSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"tsnPointColor") {
            dialInfo.dialTime.dialPoint.tsnPoint.pointColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"tsnPointStyle") {
            dialInfo.dialTime.dialPoint.tsnPoint.pointStyle
                = static_cast<DialPointStyle>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"minutePointSize") {
            dialInfo.dialTime.dialPoint.minutePoint.pointSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"minutePointColor") {
            dialInfo.dialTime.dialPoint.minutePoint.pointColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"minutePointStyle") {
            dialInfo.dialTime.dialPoint.minutePoint.pointStyle
                = static_cast<DialPointStyle>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"hourHandSize") {
            dialInfo.dialTime.dialHand.hourHand.handSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"hourHandColor") {
            dialInfo.dialTime.dialHand.hourHand.handColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"hourHandStyle") {
            dialInfo.dialTime.dialHand.hourHand.handStyle
                = static_cast<DialHandStyle>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"minuteHandSize") {
            dialInfo.dialTime.dialHand.minuteHand.handSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"minuteHandColor") {
            dialInfo.dialTime.dialHand.minuteHand.handColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"minuteHandStyle") {
            dialInfo.dialTime.dialHand.minuteHand.handStyle
                = static_cast<DialHandStyle>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"secondHandSize") {
            dialInfo.dialTime.dialHand.secondHand.handSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"secondHandColor") {
            dialInfo.dialTime.dialHand.secondHand.handColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"secondHandStyle") {
            dialInfo.dialTime.dialHand.secondHand.handStyle
                = static_cast<DialHandStyle>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"hourNumDisplayType") {
            dialInfo.dialTime.dialNum.hourNum.displayType
                = static_cast<DialDisplayType>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"hourNumFontName") {
            dialInfo.dialTime.dialNum.hourNum.fontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"hourNumFontSize") {
            dialInfo.dialTime.dialNum.hourNum.fontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"hourNumColor") {
            dialInfo.dialTime.dialNum.hourNum.numColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"hourNumStyle") {
            dialInfo.dialTime.dialNum.hourNum.numStyle
                = static_cast<DialNumStyle>(std::stoi(child->getTextContent()));
        } else if (tag == L"tsnNumDisplayType") {
            dialInfo.dialTime.dialNum.tsnNum.displayType
                = static_cast<DialDisplayType>(
                    std::stoi(child->getTextContent()));
        } else if (tag == L"tsnNumFontName") {
            dialInfo.dialTime.dialNum.tsnNum.fontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"tsnNumFontSize") {
            dialInfo.dialTime.dialNum.tsnNum.fontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"tsnNumColor") {
            dialInfo.dialTime.dialNum.tsnNum.numColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"tsnNumStyle") {
            dialInfo.dialTime.dialNum.tsnNum.numStyle
                = static_cast<DialNumStyle>(std::stoi(child->getTextContent()));
        } else if (tag == L"deltType") {
            dialInfo.dialTime.dialTimeDiff.deltType
                = static_cast<DialDeltType>(std::stoi(child->getTextContent()));
        } else if (tag == L"deltHour") {
            dialInfo.dialTime.dialTimeDiff.deltHour
                = std::stoi(child->getTextContent());
        } else if (tag == L"deltMinute") {
            dialInfo.dialTime.dialTimeDiff.deltMinute
                = std::stoi(child->getTextContent());
        } else if (tag == L"deltSecond") {
            dialInfo.dialTime.dialTimeDiff.deltSecond
                = std::stoi(child->getTextContent());
        } else if (tag == L"enabledText") {
            dialInfo.dialText.enabledText
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"text") {
            dialInfo.dialText.text = std::wstring(child->getTextContent());
        } else if (tag == L"bold") {
            dialInfo.dialText.textAttr.bold
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"italic") {
            dialInfo.dialText.textAttr.italic
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"underLine") {
            dialInfo.dialText.textAttr.underLine
                = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"textFontName") {
            dialInfo.dialText.textAttr.textFontName
                = std::wstring(child->getTextContent());
        } else if (tag == L"textFontSize") {
            dialInfo.dialText.textAttr.textFontSize
                = std::stoi(child->getTextContent());
        } else if (tag == L"textColor") {
            dialInfo.dialText.textAttr.textColor
                = std::stoul(child->getTextContent());
        } else if (tag == L"xPosRatio") {
            dialInfo.dialText.textAttr.xPosRatio
                = std::stod(child->getTextContent());
        } else if (tag == L"yPosRatio") {
            dialInfo.dialText.textAttr.yPosRatio
                = std::stod(child->getTextContent());
        }
    }
    dial = dialInfo;
}

void WatchAttr::save(Xml::Doc *doc, Xml::Node *parent) {
    DialInfo dialInfo = dial.value;
    Xml::Node *self = parent->appendChild(doc->createElement(L"watch"));
    SegmentAttr::save(doc, self);
    self->appendChild(doc->createElement(L"dialStyle"))->appendChild(
        doc->createTextNode(std::to_wstring(dialInfo.dialStyle).c_str()));

    self->appendChild(doc->createElement(L"hourPointSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.hourPoint.pointSize)
                .c_str()));
    self->appendChild(doc->createElement(L"hourPointColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.hourPoint.pointColor)
                .c_str()));
    self->appendChild(doc->createElement(L"hourPointStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.hourPoint.pointStyle)
                .c_str()));
    self->appendChild(doc->createElement(L"tsnPointSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.tsnPoint.pointSize)
                .c_str()));
    self->appendChild(doc->createElement(L"tsnPointColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.tsnPoint.pointColor)
                .c_str()));
    self->appendChild(doc->createElement(L"tsnPointStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.tsnPoint.pointStyle)
                .c_str()));
    self->appendChild(doc->createElement(L"minutePointSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.minutePoint.pointSize)
                .c_str()));
    self->appendChild(doc->createElement(L"minutePointColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.minutePoint.pointColor)
                .c_str()));
    self->appendChild(doc->createElement(L"minutePointStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialPoint.minutePoint.pointStyle)
                .c_str()));

    self->appendChild(doc->createElement(L"hourHandSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.hourHand.handSize)
                .c_str()));
    self->appendChild(doc->createElement(L"hourHandColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.hourHand.handColor)
                .c_str()));
    self->appendChild(doc->createElement(L"hourHandStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.hourHand.handStyle)
                .c_str()));
    self->appendChild(doc->createElement(L"minuteHandSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.minuteHand.handSize)
                .c_str()));
    self->appendChild(doc->createElement(L"minuteHandColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.minuteHand.handColor)
                .c_str()));
    self->appendChild(doc->createElement(L"minuteHandStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.minuteHand.handStyle)
                .c_str()));
    self->appendChild(doc->createElement(L"secondHandSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.secondHand.handSize)
                .c_str()));
    self->appendChild(doc->createElement(L"secondHandColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.secondHand.handColor)
                .c_str()));
    self->appendChild(doc->createElement(L"secondHandStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialHand.secondHand.handStyle)
                .c_str()));

    self->appendChild(doc->createElement(L"hourNumDisplayType"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialNum.hourNum.displayType)
                .c_str()));
    self->appendChild(doc->createElement(L"hourNumFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(dialInfo.dialTime.dialNum.hourNum.fontName).c_str()));
    self->appendChild(doc->createElement(L"hourNumFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialNum.hourNum.fontSize)
                .c_str()));
    self->appendChild(doc->createElement(L"hourNumColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialNum.hourNum.numColor)
                .c_str()));
    self->appendChild(doc->createElement(L"hourNumStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialNum.hourNum.numStyle)
                .c_str()));
    self->appendChild(doc->createElement(L"tsnNumDisplayType"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialNum.tsnNum.displayType)
                .c_str()));
    self->appendChild(doc->createElement(L"tsnNumFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(dialInfo.dialTime.dialNum.tsnNum.fontName).c_str()));
    self->appendChild(doc->createElement(L"tsnNumFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialNum.tsnNum.fontSize)
                .c_str()));
    self->appendChild(doc->createElement(L"tsnNumColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialNum.tsnNum.numColor)
                .c_str()));
    self->appendChild(doc->createElement(L"tsnNumStyle"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialNum.tsnNum.numStyle)
                .c_str()));

    self->appendChild(doc->createElement(L"deltType"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialTimeDiff.deltType).c_str()));
    self->appendChild(doc->createElement(L"deltHour"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialTimeDiff.deltHour).c_str()));
    self->appendChild(doc->createElement(L"deltMinute"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialTimeDiff.deltMinute)
                .c_str()));
    self->appendChild(doc->createElement(L"deltSecond"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialTime.dialTimeDiff.deltSecond)
                .c_str()));

    self->appendChild(doc->createElement(L"enabledText"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialText.enabledText).c_str()));
    self->appendChild(doc->createElement(L"text"))->appendChild(
        doc->createTextNode(std::wstring(dialInfo.dialText.text).c_str()));
    self->appendChild(doc->createElement(L"bold"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialText.textAttr.bold).c_str()));
    self->appendChild(doc->createElement(L"italic"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialText.textAttr.italic).c_str()));
    self->appendChild(doc->createElement(L"underLine"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialText.textAttr.underLine).c_str()));
    self->appendChild(doc->createElement(L"textFontName"))
        ->appendChild(doc->createTextNode(
            std::wstring(dialInfo.dialText.textAttr.textFontName).c_str()));
    self->appendChild(doc->createElement(L"textFontSize"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialText.textAttr.textFontSize).c_str()));
    self->appendChild(doc->createElement(L"textColor"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialText.textAttr.textColor).c_str()));
    self->appendChild(doc->createElement(L"xPosRatio"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialText.textAttr.xPosRatio).c_str()));
    self->appendChild(doc->createElement(L"yPosRatio"))
        ->appendChild(doc->createTextNode(
            std::to_wstring(dialInfo.dialText.textAttr.yPosRatio).c_str()));
}
