#include "office.h"
#include <QtMath>
#include "main_ui.h"
#include "data/font_attr.h"


const SliceTypeId Office::typeId{OFFICE_SLICE};
const QString Office::fileFilter{
	Office::tr("Microsoft Office (*.doc *.docx *.xls *.xlsx *.ppt *.pptx)")};
const QString Office::fileTitle{Office::tr("Office File:")};
const int Office::tipFontSize = FontAttr::FONT_SIZE_DEFAULT;
const QString Office::wordPixPrefix = "page_";
const QString Office::excelPixPrefix = "sheet_";
const QString Office::powerPointPixPrefix = "slide_";

Office::Office(Region *owner, QTreeWidgetItem *treeItem, OfficeAttr *attr)
	: Segment{owner, treeItem}, cache{nullptr} {
	attr_ = attr;
	view_ = new OfficeView{this};
	init(this->attr(), this->view());
	this->attr()->file.addObserver(
		std::bind(&Office::fileChanged, this, std::placeholders::_1));
	this->attr()->scale.addObserver(
		std::bind(&Office::scaleChanged, this, std::placeholders::_1));
	task = std::bind(&Office::makeFrame, this);
	pulses = qFloor(1000 / TICK);
	QPixmap *cacheOffice = this->view()->drawOffice();
	cache = cacheOffice;
	QPixmap *frameOffice = this->view()->drawOffice();
	frame.store(frameOffice);
}

Office::Office(Region *owner, QTreeWidgetItem *treeItem)
	: Segment{owner, treeItem}, cache{nullptr} {
	attr_ = new OfficeAttr{owner->attr()};
	view_ = new OfficeView{this};
	init(this->attr(), this->view());
	this->attr()->name = Office::tr("Office %1")
							 .arg(owner->attr()->nextChildNum)
							 .toStdWString();
	this->attr()->file.addObserver(
		std::bind(&Office::fileChanged, this, std::placeholders::_1));
	this->attr()->scale.addObserver(
		std::bind(&Office::scaleChanged, this, std::placeholders::_1));
	task = std::bind(&Office::makeFrame, this);
	pulses = qFloor(1000 / TICK);
	QPixmap *cacheOffice = this->view()->drawOffice();
	cache = cacheOffice;
	QPixmap *frameOffice = this->view()->drawOffice();
	frame.store(frameOffice);
}

Office::~Office() {
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

void Office::showCfgers() {
	Segment::showCfgers();
	MainUi::Obj->fileSrcCfger->setTitle(fileTitle);
	MainUi::Obj->fileSrcCfger->show(&attr()->file, &fileFilter);
	MainUi::Obj->pictureCfger->show(&attr()->scale);
}

void Office::hideCfgers() {
	Segment::hideCfgers();
	MainUi::Obj->fileSrcCfger->hide();
	MainUi::Obj->pictureCfger->hide();
}

void Office::cacheView() {
	if (!view())
		return;
	Slice::cacheView();

	QPixmap *preFrame = nullptr, *newFrame = view()->drawOffice();
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

void Office::play() {
	raiseView();
	if (cache) {
		return;
	} else {
		view()->update();
	}
	Slice::play();
}

void Office::makeFrame() {
	if (pulses > 0) {
		return;
	} else {
		pulses = qFloor(1000 / TICK);
		QPixmap *preFrame = nullptr, *newFrame = view()->drawOffice();
		if (newFrame) {
			preFrame = frame.fetchAndStoreOrdered(newFrame);
			if (preFrame) {
				delete preFrame;
				preFrame = nullptr;
			}
		}
	}
}

void Office::pulse() {
	--pulses;
	if (0 < pulses) {
		return;
	} else {
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

OfficeAttr *Office::attr() {
	return static_cast<OfficeAttr *>(attr_);
}

OfficeView *Office::view() {
	return static_cast<OfficeView *>(view_);
}

void Office::fileChanged(std::wstring &path) {
	attrChanged();
}

void Office::scaleChanged(ImageTool::Scale &scale) {
	attrChanged();
}

void Office::attrChanged() {
	QPixmap *preFrame = nullptr, *newFrame = view()->drawOffice();
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
