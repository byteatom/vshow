#ifndef IMAGE_TOOL_H
#define IMAGE_TOOL_H

#include <QPixmap>

class ImageTool {
public:
    typedef enum {
        STRECH,
        FILL,
        FIT,
        TILE,
        CENTER
    }Scale;

    ImageTool();
    ~ImageTool();

    static QPixmap scaled(QPixmap &source,
                          Scale scale,
                          QSize &targetSize);
};

#endif    // IMAGE_TOOL_H
