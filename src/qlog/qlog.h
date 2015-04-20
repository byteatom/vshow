#ifndef QLOG_H
#define QLOG_H

#include <QTextStream>
#include <QVariant>
#include <QPoint>
#include <QRect>
#include <QPixmap>
#include <QImage>
#include <QDataStream>

typedef enum {
    qdebug,
    qinfo,
    qwarn,
    qerror
}QLogLevel;

class QLog
{
public:
    static QLog *get();

    QLog();
    virtual ~QLog();

    template <typename T> QLog& operator<<(T val) {
        stream << val;
        return *this;
    }
    void operator<<(const QLogLevel &level);
    QLog& operator<<(const QPoint &point);
    QLog& operator<<(const QPointF &point);
    QLog& operator<<(const QSize &rect);
    QLog& operator<<(const QSizeF &rect);
    QLog& operator<<(const QRect &rect);
    QLog& operator<<(const QRectF &rect);
    QLog& operator<<(const QPixmap &pixmap);
    QLog& operator<<(const QImage &image);
    QLog& operator<<(const std::wstring &string);
    QLog& operator<<(const std::string &string);

    void write(QDataStream &stream, QByteArray &bytes);
    void readLen(QDataStream &stream);
    void readElse(QDataStream &stream);

    quint32 len;
    quint32 processId;
    quint32 threadId;
    qint64 milSecHead;
    qint64 milSecTail;
    qint32 level;
    QVariant var;
    QString text;

    QTextStream stream;
};

#define qlog (*new QLog)

#endif // QLOG_H
