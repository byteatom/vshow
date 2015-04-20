#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <QButtonGroup>

#include "attr_cfger.h"
#include "text.h"

namespace Ui {
class TextCfger;
}

class Text;

class TextCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit TextCfger();
    virtual ~TextCfger();

    void show(Text *text);
    void updateBtns();

private slots:
    void family(const QFont &font);
    void textSize(int textSize);
    void wrap(bool checked);
    void left(bool checked);
    void center(bool checked);
    void right(bool checked);
    void justify(bool checked);
    void backColor();
    void foreColor();
    void bold(bool checked);
    void underLine(bool checked);
    void italic(bool checked);

public:
    Text *text;

private:
    QButtonGroup lineBtns;
    QButtonGroup alignBtns;
    Ui::TextCfger *ui;
};

#endif    // TEXT_EDITOR_H
