#include "screen_view.h"
#include "ui_screen_view.h"

#include <QPainter>
#include <QRectF>

#include "screen.h"
#include "main_ui.h"

const QPen ScreenView::BORDER_PEN{QBrush{Qt::blue}, 2};

ScreenView::ScreenView(Screen *screen)
	: QDialog(nullptr),
	  ui(new Ui::ScreenView),
	  screen(screen),
	  mouseGeo(this),
	  mouseSelect(this, screen) {
	ui->setupUi(this);
	setWindowTitle(QString::fromStdWString(screen->attr()->name));
	setLayout(&layout);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	// setAttribute(Qt::WA_NoSystemBackground, true);
	// setAutoFillBackground(true);
	// setAttribute(Qt::WA_OpaquePaintEvent, true);
	// setAttribute(Qt::WA_PaintOnScreen, true);

	// setWindowOpacity(0.5);
	// setStyleSheet("ScreenDlg {border: 1px solid red; background-color:rgba(0, 0, 0, 0)");
	/*QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(128, 0, 0, 128));
	setPalette(pal);
	*/

	setGeometry(screen->attr()->geo.x,
				screen->attr()->geo.y,
				screen->attr()->geo.width,
				screen->attr()->geo.height);

	screen->attr()->name.addObserver(
		std::bind(&ScreenView::nameChanged, this, std::placeholders::_1));
	screen->attr()->geo.x.addObserver(
		std::bind(&ScreenView::xChanged, this, std::placeholders::_1));
	screen->attr()->geo.y.addObserver(
		std::bind(&ScreenView::yChanged, this, std::placeholders::_1));
	screen->attr()->geo.width.addObserver(
		std::bind(&ScreenView::widthChanged, this, std::placeholders::_1));
	screen->attr()->geo.height.addObserver(
		std::bind(&ScreenView::heightChanged, this, std::placeholders::_1));
	screen->attr()->lock.addObserver(
		std::bind(&ScreenView::lockChanged, this, std::placeholders::_1));
	screen->attr()->top.addObserver(
		std::bind(&ScreenView::topChanged, this, std::placeholders::_1));
}

ScreenView::~ScreenView() {
	delete ui;
}

void ScreenView::paintEvent(QPaintEvent *) {
	// qlog << "ScreenView::paintEvent " <<
	// QString::fromStdWString(screen->attr()->name) << qdebug;
	QPainter painter(this);
	QRect box = rect();
	if (!screen->playing) {
		painter.setPen(BORDER_PEN);
		painter.drawRect(box);
	}
	if (ImageAttr::BG_COLOR_NONE != screen->attr()->bgColor) {
		painter.fillRect(box, QColor::fromRgba(screen->attr()->bgColor));
	} else {
		painter.fillRect(box, QColor::fromRgba(ImageAttr::BG_COLOR_NONE_TOP));
	}
	if (!screen->bgCache.isNull()) {
		//        qlog << "scaled: " << screen->bgCache << qdebug;
		painter.drawPixmap(Slice::POS_TOPLEFT, screen->bgCache);
	}
}

void ScreenView::resizeEvent(QResizeEvent *event) {
	geometryEvent();
	QRectF rect = frameGeometry();
	if (screen->playCtrl->isVisible()) {
		screen->playCtrl->hide();
		screen->playCtrl->setGeometry(rect.left(),
									  rect.bottom(),
									  rect.width(),
									  screen->playCtrl->height());
		screen->playCtrl->move(rect.bottomLeft().toPoint());
		screen->playCtrl->show();
	}
	if (screen->programList->isVisible()) {
		screen->programList->hide();
		screen->programList->setGeometry(
			rect.right(),
			rect.top(),
			screen->programList->width(),
			rect.height() + screen->playCtrl->height());
		screen->programList->move(rect.topRight().toPoint());
		screen->programList->show();
	}
	QDialog::resizeEvent(event);
}

void ScreenView::moveEvent(QMoveEvent *event) {
	geometryEvent();
	QRectF rect = frameGeometry();
	if (screen->playCtrl->isVisible()) {
		screen->playCtrl->hide();
		screen->playCtrl->setGeometry(rect.left(),
									  rect.bottom(),
									  rect.width(),
									  screen->playCtrl->height());
		screen->playCtrl->move(rect.bottomLeft().toPoint());
		screen->playCtrl->show();
	}
	if (screen->programList->isVisible()) {
		screen->programList->hide();
		screen->programList->setGeometry(
			rect.right(),
			rect.top(),
			screen->programList->width(),
			rect.height() + screen->playCtrl->height());
		screen->programList->move(rect.topRight().toPoint());
		screen->programList->show();
	}
	QDialog::moveEvent(event);
}

void ScreenView::enterEvent(QEvent *) {
	if (!screen->playing) return;

	/*QRectF rect = frameGeometry();
	if (!screen->playCtrl->isVisible()) {
		screen->playCtrl->setGeometry(rect.left(),
									  rect.bottom(),
									  rect.width(),
									  screen->playCtrl->height());
		screen->playCtrl->show();
	}
	if (!screen->programList->isVisible()) {
		screen->programList->setGeometry(
			rect.right(),
			rect.top(),
			screen->programList->width(),
			rect.height() + screen->playCtrl->height());
		screen->programList->show();
	}*/
}

void ScreenView::leaveEvent(QEvent *) {
}

void ScreenView::nameChanged(std::wstring &name) {
	setWindowTitle(QString::fromStdWString(name));
}

void ScreenView::xChanged(int x) {
	QRect rect = frameGeometry();
	rect.moveLeft(x);
	setGeometry(rect);
}

void ScreenView::yChanged(int y) {
	QRect rect = frameGeometry();
	rect.moveTop(y);
	setGeometry(rect);
}

void ScreenView::widthChanged(int width) {
	QRect rect = frameGeometry();
	rect.setWidth(width);
	setGeometry(rect);
}

void ScreenView::heightChanged(int height) {
	QRect rect = frameGeometry();
	rect.setHeight(height);
	setGeometry(rect);
}

void ScreenView::lockChanged(bool lock) {
	mouseGeo.setEnabled(!lock);
}

void ScreenView::topChanged(bool top) {
	if (top)
		setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
	else
		setWindowFlags(windowFlags() & (~Qt::WindowStaysOnTopHint));
	show();
}

void ScreenView::geometryEvent() {
	QRect rect = frameGeometry();
	screen->attr()->geo.x.value = rect.x();
	screen->attr()->geo.y.value = rect.y();
	screen->attr()->geo.width.value = rect.width();
	screen->attr()->geo.height.value = rect.height();

	if (screen->isCurrent())
		screen->showCfgers();
	screen->cacheView();
}
