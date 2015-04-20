#include "timer.h"
#include <QtMath>
#include "main_ui.h"


const SliceTypeId Timer::typeId{TIMER_SLICE};
const QString Timer::fileFilter{Timer::tr("Images (*.bmp *.jpg *.png)")};

Timer::Timer(Region *owner, QTreeWidgetItem *treeItem, TimerAttr *attr)
	: Segment{owner, treeItem}, cache{nullptr} {
	attr_ = attr;
	view_ = new TimerView{this};
	init(this->attr(), this->view());
	this->attr()->timer.addObserver(
		std::bind(&Timer::attrChanged, this, std::placeholders::_1));
	task = std::bind(&Timer::makeFrame, this);
	QPixmap *cacheTimer = this->view()->drawTimer();
	cache = cacheTimer;
	QPixmap *frameTimer = this->view()->drawTimer();
	frame.store(frameTimer);
}

Timer::Timer(Region *owner, QTreeWidgetItem *treeItem)
	: Segment{owner, treeItem}, cache{nullptr} {
	attr_ = new TimerAttr{owner->attr()};
	view_ = new TimerView{this};
	init(attr(), view());
	this->attr()->name = MainUi::tr("Timer %1")
							 .arg(owner->attr()->nextChildNum)
							 .toStdWString();
	this->attr()->timer.addObserver(
		std::bind(&Timer::attrChanged, this, std::placeholders::_1));
	task = std::bind(&Timer::makeFrame, this);
	QPixmap *cacheTimer = this->view()->drawTimer();
	cache = cacheTimer;
	QPixmap *frameTimer = this->view()->drawTimer();
	frame.store(frameTimer);
}

Timer::~Timer() {
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

void Timer::showCfgers() {
	Segment::showCfgers();
	MainUi::Obj->timerCfger->show(this);
}

void Timer::hideCfgers() {
	Segment::hideCfgers();
	MainUi::Obj->timerCfger->hide();
}

void Timer::cacheView() {
	if (!view())
		return;
	Slice::cacheView();
	QPixmap *preFrame = nullptr, *newFrame = view()->drawTimer();
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

void Timer::play() {
	pulses = TICK_PER_SEC;
	seconds = 0;
	Slice::play();
}

void Timer::makeFrame() {
	QPixmap *preFrame = nullptr, *newFrame = view()->drawTimer();
	if (newFrame) {
		preFrame = frame.fetchAndStoreOrdered(newFrame);
		if (preFrame) {
			delete preFrame;
			preFrame = nullptr;
		}
	}
}

TimerAttr *Timer::attr() {
	return static_cast<TimerAttr *>(attr_);
}

TimerView *Timer::view() {
	return static_cast<TimerView *>(view_);
}

void Timer::pulse() {
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

void Timer::attrChanged(TimerInfo &newValue) {
	QPixmap *preFrame = nullptr, *newFrame = view()->drawTimer();
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
