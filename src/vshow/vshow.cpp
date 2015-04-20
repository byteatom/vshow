#include <QApplication>

#include <QTranslator>
#include <QLocale>
#include <QCoreApplication>

#include "io/io.h"
#include "data/data.h"
#include "ui/main_ui.h"
#include "util/task_syncer.h"
#include "util/win_util.h"

int main(int argc, char *argv[]) {
// QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
#if !defined(NDEBUG)
    ::showConsole();
#endif

    QApplication app(argc, argv);

    QString locale = QLocale::system().name();
    QTranslator qtTranslator;
    if (qtTranslator.load("qt_" + locale, "language"))
        app.installTranslator(&qtTranslator);
    QTranslator appTranslator;

    Io *io = new Io;
    Data *data = new Data;
    data->appAttr->runDir
        = QCoreApplication::applicationDirPath().toStdWString();

    io->start();
    data->load();
    if (!data->appAttr->language.empty()) {
        if (appTranslator.load(LANGUAGE_PREFIX + QString::fromStdWString(
                                                     data->appAttr->language),
                               "language"))
            app.installTranslator(&appTranslator);
    } else {
        if (appTranslator.load(LANGUAGE_PREFIX + locale, "language"))
            app.installTranslator(&appTranslator);
    }

    MainUi *ui = new MainUi;
    ui->show();
    int ret = app.exec();

    data->free();
    io->stop();

    delete ui;
    delete data;
    delete io;

    return ret;
}
