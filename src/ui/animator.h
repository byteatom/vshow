#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <QCoreApplication>
#include <QRectF>
#include <QPixmap>
#include <QString>


#include "data/animate_attr.h"

class Animator {
    Q_DECLARE_TR_FUNCTIONS(Animator)

public:
    static const int pageTurnRange;
    static const int fanRange;
    static const int spinRange;
    static const double PI;
    typedef QPixmap (Animator::*Action)();
    struct StyleAction {
        AnimateAttr::Style style;
        Action action;
        const char *text;
    };
    static const StyleAction actions[AnimateAttr::STYLE_MAX];
    static void check();

    Animator();
    ~Animator();

    void build(QRectF region, QPixmap source, AnimateAttr *attr);
    QPixmap frame();

private:
    AnimateAttr::Style produceRandomStyle();
    QPixmap getCurrentPagePixmap();
    QPixmap random();
    QPixmap motionless();
    QPixmap moveUp();
    QPixmap moveDown();
    QPixmap moveLeft();
    QPixmap moveRight();
    QPixmap overlayUp();
    QPixmap overlayDown();
    QPixmap overlayLeft();
    QPixmap overlayRight();
    QPixmap horizontalInaugurate();
    QPixmap horizontalConclude();
    QPixmap verticalInaugurate();
    QPixmap verticalConclude();
    QPixmap shutterUp();
    QPixmap shutterDown();
    QPixmap shutterLeft();
    QPixmap shutterRight();
    QPixmap blink();
    QPixmap brushUp();
    QPixmap brushDown();
    QPixmap brushLeft();
    QPixmap brushRight();
    QPixmap pageLeft();
    QPixmap pageRight();
    QPixmap fanInaugurate();
    QPixmap fanConclude();
    QPixmap spinLeft();
    QPixmap spinRight();
    QPixmap rectangleInaugurate();
    QPixmap rectangleConclude();
    QPixmap curtainDown();
    QPixmap curtainRight();
    QPixmap rectangleTopLeft();
    QPixmap rectangleTopRight();
    QPixmap rectangleBottomLeft();
    QPixmap rectangleBottomRight();
    QPixmap diagonalTopLeft();
    QPixmap diagonalTopRight();
    QPixmap diagonalBottomLeft();
    QPixmap diagonalBottomRight();
    QPixmap crossExpand();
    QPixmap crossShrinkage();
    QPixmap moveLeftContinuous();
    QPixmap moveRightContinuous();

    QRectF region;
    QPixmap source;
    AnimateAttr *attr;
    AnimateAttr::Style randomStyle;
    int sourcePixmapWidth;
    int sourcePixmapHeight;
    int xPageCount;
    int yPageCount;
    int pageCount;
    int currentPage;
    bool pageCountCycleComplete;
    int currentStepLocation;
    unsigned int blinkCount;
    int curtainMoveIndex;
    bool brushVeil;
};

#endif    // ANIMATOR_H
