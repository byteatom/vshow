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

	Io *io = new Io;
	Data *data = new Data;
	data->appAttr->runDir
		= QCoreApplication::applicationDirPath().toStdWString();

	io->start();
	data->load();

	QString language{QString::fromStdWString(data->appAttr->language)};
	if (language.isEmpty()) language = QLocale::system().name();
	QTranslator qtTranslator;
	QTranslator appTranslator;
	if (language != QLocale(QLocale::English, QLocale::UnitedStates).name()) {
		if (qtTranslator.load("qt_" + language, "language"))
			app.installTranslator(&qtTranslator);
		if (appTranslator.load(LANGUAGE_PREFIX + language, "language"))
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
