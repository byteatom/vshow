#include "watch.h"
#include "main_ui.h"
#include <QtMath>
#include <QPainter>


const SliceTypeId Watch::typeId{WATCH_SLICE};
const QString Watch::fileFilter{Watch::tr("Images (*.bmp *.jpg *.png)")};

Watch::Watch(Region *owner, QTreeWidgetItem *treeItem, WatchAttr *attr)
	: Segment{owner, treeItem}, cache{nullptr} {
	attr_ = attr;
	view_ = new WatchView{this};
	init(this->attr(), this->view());
	this->attr()->dial.addObserver(
		std::bind(&Watch::attrChanged, this, std::placeholders::_1));
	task = std::bind(&Watch::makeFrame, this);
	QPixmap *cacheWatch = this->view()->drawWatch();
	cache = cacheWatch;
	QPixmap *frameWatch = this->view()->drawWatch();
	frame.store(frameWatch);
}

Watch::Watch(Region *owner, QTreeWidgetItem *treeItem)
	: Segment{owner, treeItem}, cache{nullptr} {
	attr_ = new WatchAttr{owner->attr()};
	view_ = new WatchView{this};
	init(this->attr(), this->view());
	this->attr()->name = MainUi::tr("Watch %1")
							 .arg(owner->attr()->nextChildNum)
							 .toStdWString();
	this->attr()->dial.addObserver(
		std::bind(&Watch::attrChanged, this, std::placeholders::_1));
	task = std::bind(&Watch::makeFrame, this);
	QPixmap *cacheWatch = this->view()->drawWatch();
	cache = cacheWatch;
	QPixmap *frameWatch = this->view()->drawWatch();
	frame.store(frameWatch);
}

Watch::~Watch() {
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

void Watch::showCfgers() {
	Segment::showCfgers();
	MainUi::Obj->watchCfger->show(this);
}

void Watch::hideCfgers() {
	Segment::hideCfgers();
	MainUi::Obj->watchCfger->hide();
}

void Watch::cacheView() {
	if (!view())
		return;
	Slice::cacheView();
	QPixmap *preFrame = nullptr, *newFrame = view()->drawWatch();
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

void Watch::play() {
	pulses = TICK_PER_SEC;
	seconds = 0;
	Slice::play();
}

void Watch::makeFrame() {
	QPixmap *preFrame = nullptr, *newFrame = view()->drawWatch();
	if (newFrame) {
		preFrame = frame.fetchAndStoreOrdered(newFrame);
		if (preFrame) {
			delete preFrame;
			preFrame = nullptr;
		}
	}
}

void Watch::pulse() {
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

WatchAttr *Watch::attr() {
	return static_cast<WatchAttr *>(attr_);
}

WatchView *Watch::view() {
	return static_cast<WatchView *>(view_);
}

void Watch::attrChanged(DialInfo &newValue) {
	QPixmap *preFrame = nullptr, *newFrame = view()->drawWatch();
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
