#ifndef OFFICEVIEW_H
#define OFFICEVIEW_H

#include "mouse_select.h"
#include <QAxObject>


class Office;

class OfficeView : public QWidget {
    Q_DECLARE_TR_FUNCTIONS(OfficeView)

public:
    explicit OfficeView(Office *office);
    virtual ~OfficeView();
    QPixmap *drawOffice();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void drawOffice(QPainter &painter, const QRectF &region);
    void drawWord(QPainter &painter, const QRectF &region, const QString &file);
    void drawExcel(QPainter &painter,
                   const QRectF &region,
                   const QString &file);
    void drawPowerPoint(QPainter &painter,
                        const QRectF &region,
                        const QString &file);
    void wordToGraphics(const QString &file, const QRectF &region);
    void excelToGraphics(const QString &file, const QRectF &region);
    void powerPointToGraphics(const QString &file, const QRectF &region);
    QImage getClipboardData(void **buff);
    void drawTipText(QPainter &painter,
                     const QRectF &region,
                     const QString &tip);
    QString getGraphicsStoreDir();
    QString gerOfficeFilePath();

    Office *office;
    QAxObject *word;
    QAxObject *excel;
    QAxObject *powerPoint;
    bool wordInitSucceed;
    bool excelInitSucceed;
    bool powerPointInitSucceed;
    MouseSelect mouseSelect;
    QString currentOfficeFile;
    QString currentGraphicsStorePath;
    int currentGraphicsCount;
    int currentGraphicsIndex;
};

#endif    // OFFICEVIEW_H
