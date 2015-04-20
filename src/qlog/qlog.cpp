#include "qlog.h"

#include <cassert>

#include "util/hi_timer.h"

#include "sender.h"
#include "util/win_util.h"

QLog::QLog() :
    len{0},
    processId{::GetCurrentProcessId()},
    threadId{::GetCurrentThreadId()},
    milSecHead{HiTimer::elapse()},
    milSecTail{milSecHead},
    level{qdebug},
    stream{&text, QIODevice::WriteOnly}
{
}

QLog::~QLog()
{

}



void QLog::operator<<(const QLogLevel &level)
{
    milSecTail = HiTimer::elapse();
    this->level = level;
    if (!sender.isRunning()) {
        sender.start(QThread::LowPriority);
    }
    sender.push(this);
    return;
}

QLog& QLog::operator<<(const QPoint &point)
{
    stream << "(" << point.x() << "," << point.y() << ")";
    return *this;
}

QLog& QLog::operator<<(const QPointF &point)
{
    stream << "(" << point.x() << "," << point.y() << ")";
    return *this;
}

QLog& QLog::operator<<(const QSize &size)
{
    stream << "(" << size.width() << "," << size.height() << ")";
    return *this;
}

QLog& QLog::operator<<(const QSizeF &size)
{
    stream << "(" << size.width() << "," << size.height() << ")";
    return *this;
}

QLog& QLog::operator<<(const QRect &rect)
{
    stream << "(" << rect.x() << "," << rect.y() << "," << rect.width() << "," << rect.height() << ")";
    return *this;
}

QLog& QLog::operator<<(const QRectF &rect)
{
    stream << "(" << rect.x() << "," << rect.y() << "," << rect.width() << "," << rect.height() << ")";
    return *this;
}

QLog& QLog::operator<<(const QPixmap &pixmap)
{
    var = pixmap;
    return *this;
}

QLog& QLog::operator<<(const QImage &image)
{
    var = image;
    return *this;
}

QLog& QLog::operator<<(const std::wstring &string)
{
    stream << QString::fromStdWString(string);
    return *this;
}

QLog& QLog::operator<<(const std::string &string)
{
    stream << QString::fromStdString(string);
    return *this;
}

void QLog::write(QDataStream &output, QByteArray &bytes)
{
    bytes.clear();
    output.device()->seek(0);
    output << len <<
              processId <<
              threadId <<
              milSecHead <<
              milSecTail <<
              level <<
              var <<
              text;
    len = bytes.size() - sizeof(quint32);
    output.device()->seek(0);
    output << len;
}

void QLog::readLen(QDataStream &input)
{
    input >> len;
}

void QLog::readElse(QDataStream &input)
{
    input >> processId >>
            threadId >>
            milSecHead >>
            milSecTail >>
            level >>
            var >>
            text;
}

