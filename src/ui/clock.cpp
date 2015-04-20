#include "clock.h"
#include "main_ui.h"
#include <QtMath>


const SliceTypeId Clock::typeId{CLOCK_SLICE};
const QString Clock::fileFilter{Clock::tr("Images (*.bmp *.jpg *.png)")};

Clock::Clock(Region *owner, QTreeWidgetItem *treeItem, ClockAttr *attr)
	: Segment{owner, treeItem}, cache{nullptr} {
	attr_ = attr;
	view_ = new ClockView{this};
	init(this->attr(), this->view());
	this->attr()->clock.addObserver(
		std::bind(&Clock::attrChanged, this, std::placeholders::_1));
	task = std::bind(&Clock::makeFrame, this);
	QPixmap *cacheClock = this->view()->drawClock();
	cache = cacheClock;
	QPixmap *frameClock = this->view()->drawClock();
	frame.store(frameClock);
}

Clock::Clock(Region *owner, QTreeWidgetItem *treeItem)
	: Segment{owner, treeItem}, cache{nullptr} {
	attr_ = new ClockAttr{owner->attr()};
	view_ = new ClockView{this};
	init(this->attr(), this->view());
	this->attr()->name = MainUi::tr("Clock %1")
							 .arg(owner->attr()->nextChildNum)
							 .toStdWString();
	this->attr()->clock.addObserver(
		std::bind(&Clock::attrChanged, this, std::placeholders::_1));
	task = std::bind(&Clock::makeFrame, this);
	QPixmap *cacheClock = this->view()->drawClock();
	cache = cacheClock;
	QPixmap *frameClock = this->view()->drawClock();
	frame.store(frameClock);
}

Clock::~Clock() {
	hideCfgers();
	if (cache) {
		delete cache;
		cache = nullptr;
	}
	QPixmap *remainderPix = frame.fetchAndStoreOrdered(nullptr);
	if (remainderPix) {
		delete remainderPix;
		remainderPix = nullptr;
	}
}

void Clock::showCfgers() {
	Segment::showCfgers();
	MainUi::Obj->clockCfger->show(this);
}

void Clock::hideCfgers() {
	Segment::hideCfgers();
	MainUi::Obj->clockCfger->hide();
}

void Clock::cacheView() {
	if (!view())
		return;
	Slice::cacheView();
	QPixmap *preFrame = nullptr, *newFrame = view()->drawClock();
	if (newFrame) {
		preFrame = frame.fetchAndStoreOrdered(newFrame);
		if (preFrame) {
			delete preFrame;
			preFrame = nullptr;
		}
	}
	QPixmap *preCache = cache;
	QPixmap *newCache = frame.fetchAndStoreOrdered(nullptr);
	if (newCache) {
		cache = newCache;
		if (preCache) {
			delete preCache;
			preCache = nullptr;
		}
	}
}

void Clock::play() {
	pulses = TICK_PER_SEC;
	seconds = 0;
	Slice::play();
}

void Clock::makeFrame() {
	QPixmap *preFrame = nullptr, *newFrame = view()->drawClock();
	if (newFrame) {
		preFrame = frame.fetchAndStoreOrdered(newFrame);
		if (preFrame) {
			delete preFrame;
			preFrame = nullptr;
		}
	}
}

ClockAttr *Clock::attr() {
	return static_cast<ClockAttr *>(attr_);
}

ClockView *Clock::view() {
	return static_cast<ClockView *>(view_);
}

void Clock::pulse() {
	if (!playing)
		return;

	--pulses;
	if (0 < pulses) {
		return;
	} else {
		pulses = TICK_PER_SEC;
		/*++seconds;
		if (5 == seconds) {
			end();
			return;
		}*/
		QPixmap *preCache = cache;
		QPixmap *newCache = frame.fetchAndStoreOrdered(nullptr);
		if (newCache) {
			cache = newCache;
			if (preCache) {
				delete preCache;
				preCache = nullptr;
			}
		}
		MainUi::Obj->project->tasker.addTask(&task);
	}
}

void Clock::attrChanged(ClockInfo &newValue) {
	QPixmap *preFrame = nullptr, *newFrame = view()->drawClock();
	if (newFrame) {
		preFrame = frame.fetchAndStoreOrdered(newFrame);
		if (preFrame) {
			delete preFrame;
			preFrame = nullptr;
		}
	}
	QPixmap *preCache = cache;
	QPixmap *newCache = frame.fetchAndStoreOrdered(nullptr);
	if (newCache) {
		cache = newCache;
		if (preCache) {
			delete preCache;
			preCache = nullptr;
		}
	}
	view()->update();
}
