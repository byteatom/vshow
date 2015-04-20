#include "image_tool.h"

#include <QPainter>

ImageTool::ImageTool() {
}

ImageTool::~ImageTool() {
}

QPixmap ImageTool::scaled(QPixmap &source,
                          ImageTool::Scale scale,
                          QSize &targetSize) {
    QPixmap scaled;

    switch (scale) {
    case STRECH:
        scaled = source.scaled(
            targetSize,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation /*Qt::FastTransformation*/);
        break;
    case FILL:
        scaled = source.scaled(targetSize,
                               Qt::KeepAspectRatioByExpanding,
                               Qt::SmoothTransformation);
        break;
    case FIT:
        scaled = source.scaled(
            targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        break;
    case TILE:
        scaled = source;
        break;
    case CENTER:
        scaled = source;
        break;
    default:
        break;
    }

    QPixmap target(targetSize);
    target.fill(QColor::fromRgba(Qt::transparent));
    QPainter painter(&target);

    if (TILE == scale) {
        painter.drawTiledPixmap(target.rect(), scaled);
    } else {
        QSizeF offset = (targetSize - scaled.size()) / 2;
        QPointF pos = QPointF(offset.rwidth(), offset.rheight());
        painter.drawPixmap(pos, scaled);
    }

    return target;
}
