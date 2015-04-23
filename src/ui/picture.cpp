#include "picture.h"

#include <functional>

#include <QPainter>

#include "main_ui.h"

#include "util/image_tool.h"

#include "qlog/qlog.h"

const SliceTypeId Picture::typeId{PICTURE_SLICE};
const QString Picture::fileFilter{Picture::tr("Image (*.bmp *.jpg *.png)")};
const QString Picture::fileTitle{Picture::tr("Picture File:")};

Picture::Picture(Region *owner, QTreeWidgetItem *treeItem, PictureAttr *attr)
	: Segment{owner, treeItem} {
	attr_ = attr;
	view_ = new PictureView{this};
	init(this->attr(), this->view());

	if (this->attr()->name.value.empty())
		this->attr()->name
			= Picture::tr("Picture %1").arg(owner->attr()->nextChildNum).toStdWString();

	this->attr()->file.addObserver(
		std::bind(&Picture::fileChanged, this, std::placeholders::_1));
	this->attr()->scale.addObserver(
		std::bind(&Picture::scaleChanged, this, std::placeholders::_1));

	stayTimer.setSingleShot(true);
	stayTimer.setTimerType(Qt::CoarseTimer);
	connect(&stayTimer, &QTimer::timeout, this, &Picture::end);

	task = std::bind(&Picture::makeFrame, this);
}

Picture::Picture(Region *owner, QTreeWidgetItem *treeItem)
	: Picture{owner, treeItem, new PictureAttr{owner->attr()}} {
}

Picture::~Picture() {
	hideCfgers();
}

void Picture::fileChanged(std::wstring &path) {
	cacheView();
	updateView();
}

void Picture::scaleChanged(ImageTool::Scale &scale) {
	cacheView();
	updateView();
}

void Picture::showCfgers() {
	Segment::showCfgers();
	MainUi::Obj->fileSrcCfger->setTitle(fileTitle);
	MainUi::Obj->fileSrcCfger->show(&attr()->file, &fileFilter);
	MainUi::Obj->pictureCfger->show(&attr()->scale);
	MainUi::Obj->animateCfger->show(&attr()->animateAttr);
}

void Picture::hideCfgers() {
	Segment::hideCfgers();
	MainUi::Obj->fileSrcCfger->hide();
	MainUi::Obj->pictureCfger->hide();
	MainUi::Obj->animateCfger->hide();
}

void Picture::cacheView() {
	if (!view())
		return;

	Slice::cacheView();
	qlog << "Picture::cacheView " << view()->frameSize() << qdebug;
	if (!attr()->file.value.empty()
		&& cache.load(QString::fromStdWString(attr()->file)))
		cache = ImageTool::scaled(cache, attr()->scale, view()->frameSize());
	else
		cache = QPixmap();
}

void Picture::start() {
	cacheView();
	origin = cache;
	Slice::start();
}

void Picture::play() {
	pulses = attr()->animateAttr.pulses;
	frameMutex.lock();
	animator.build(view()->rect(), origin, &attr()->animateAttr);
	frame = animator.frame();
	frameMutex.unlock();
	Slice::play();
}

void Picture::makeFrame() {
	// qlog << "Picture::makeFrame" << attr()->name.value << qdebug;
	frameMutex.lock();
	frame = animator.frame();
	frameMutex.unlock();
	// qlog << "Picture::makeFrame end" << attr()->name.value << qdebug;
	return;
}

void Picture::pulse() {
	if (!playing)
		return;

	--pulses;
	if (0 < pulses)
		return;

	frameMutex.lock();
	if (frame.isNull()) {
		frameMutex.unlock();
		if (attr()->animateAttr.stay > 0) {
			playing = false;
			stayTimer.start(attr()->animateAttr.stay * 1000);
		} else
			end();
	} else {
		cache = frame;
		frameMutex.unlock();
		pulses = attr()->animateAttr.pulses;
		MainUi::Obj->project->tasker.addTask(&task);
	}
}

void Picture::end() {
	Slice::end();
}

void Picture::stop() {
	stayTimer.stop();
	cacheView();
	Slice::stop();
}

PictureAttr *Picture::attr() {
	return static_cast<PictureAttr *>(attr_);
}

PictureView *Picture::view() {
	return static_cast<PictureView *>(view_);
}
