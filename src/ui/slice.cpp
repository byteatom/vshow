#include "slice.h"

#include <list>
#include <cassert>

#include <QPainter>

#include "qlog/qlog.h"
#include "util/image_tool.h"
#include "main_ui.h"
#include "picture.h"

const unsigned int Slice::TICK{10};
const unsigned int Slice::TICK_PER_SEC{100};
const QPointF Slice::POS_TOPLEFT{0, 0};

Slice::Slice(Slice *owner, QTreeWidgetItem *treeItem)
	: TreeNode{owner},
	  owner(owner),
	  attr_{nullptr},
	  view_{nullptr},
	  treeItem{treeItem},
	  mode{SELF},
	  serialItor{organs.end()},
	  playing{false},
	  endOnce{false},
	  rewindOrgan{false} {
}

Slice::~Slice() {
	hideCfgers();
	freeOrgans();
	if (view_)
		delete view_;
	if (attr_)
		delete attr_;
}

void Slice::init(SliceAttr *attr, QWidget *view) {
	this->attr_ = attr;
	this->view_ = view;
	attr->name.addObserver(
		std::bind(&Slice::nameChanged, this, std::placeholders::_1));
	attr->bgColor.addObserver(
		std::bind(&Slice::colorChanged, this, std::placeholders::_1));
	attr->bgImage.addObserver(
		std::bind(&Slice::imageChanged, this, std::placeholders::_1));
	attr->bgScale.addObserver(
		std::bind(&Slice::scaleChanged, this, std::placeholders::_1));
}

void Slice::showCfgers() {
	MainUi::Obj->sliceCfger->show(attr_);
	MainUi::Obj->bgFileCfger->show(&attr_->bgImage, &Picture::fileFilter);
	MainUi::Obj->bgScaleCfger->show(&attr_->bgScale);
}

void Slice::hideCfgers() {
	MainUi::Obj->sliceCfger->hide();
	MainUi::Obj->bgFileCfger->hide();
	MainUi::Obj->bgScaleCfger->hide();
}

bool Slice::isCurrent() {
	return (MainUi::Obj->sliceCfger->attr == attr_);
}

bool Slice::isActive() {
	SliceAttr *currentAttr = MainUi::Obj->sliceCfger->attr;
	while (currentAttr) {
		if (currentAttr == attr_)
			return true;
		currentAttr = currentAttr->owner;
	}
	return false;
}

void Slice::showView() {
	if (view_)
		view_->show();
}

void Slice::cacheView() {
	if (!view_)
		return;
	if (!attr_->bgImage.value.empty()
		&& bgCache.load(QString::fromStdWString(attr_->bgImage))) {
		bgCache
			= ImageTool::scaled(bgCache, attr_->bgScale, view_->frameSize());
	} else {
		bgCache = QPixmap();
	}
}

void Slice::updateView() {
	//    qlog << "updateView " << attr->name.value << qdebug;
	if (view_)
		view_->update();
}

void Slice::repaintView() {
	//    qlog << "repaintView " << attr->name.value << qdebug;
	if (view_)
		view_->repaint();
}

void Slice::raiseView() {
	// qlog << "raiseView " << attr->name.value << qdebug;
	showView();
	if (view_) {
		// view->activateWindow();
		view_->raise();
	}
}

void Slice::hideView() {
	// bgCache = QPixmap();
	if (view_)
		view_->hide();
}

void Slice::inflate() {
	// qlog << "inflate " << qdebug;
	showCfgers();

	raiseView();

	Slice *iter = this;
	while (iter = iter->owner) {
		iter->raiseView();
		iter->updateView();
	}
}

void Slice::deflate() {
	// qlog << "deflate " << qdebug;
	hideCfgers();

	updateView();

	Slice *iter = this;
	while (iter = iter->owner) {
		iter->updateView();
	}

	// hideView(); //let real slice to decide whether hideView
}

void Slice::nameChanged(std::wstring &name) {
	if (treeItem)
		treeItem->setText(0, QString::fromStdWString(name));
}

void Slice::colorChanged(ImageAttr::Argb &color) {
	updateView();
}

void Slice::imageChanged(std::wstring &file) {
	cacheView();
	updateView();
}

void Slice::scaleChanged(ImageTool::Scale &scale) {
	cacheView();
	updateView();
}

void Slice::start() {
	//    qlog << "start " << attr->name.value << qdebug;
	for (auto organ : organs) {
		Slice *slice = static_cast<Slice *>(organ);
		if (PARELLEL == mode)
			slice->raiseView();
		slice->start();
	}
}

void Slice::play() {
	//    qlog << "play " << attr->name.value << qdebug;
	playing = true;
	if (SERIAL == mode) {
		serialItor = organs.begin();
		if (organs.end() != serialItor) {
			Slice *slice = static_cast<Slice *>(*serialItor);
			slice->raiseView();
			slice->play();
		}
	} else if (PARELLEL == mode) {
		for (auto organ : organs) {
			Slice *slice = static_cast<Slice *>(organ);
			slice->endOnce = false;
			slice->play();
		}
	}
}

void Slice::pulse() {
	//    qlog << "pulse " << attr->name.value << "playing "<<playing<<qdebug;
	if (!playing)
		return;

	if (SERIAL == mode) {
		if (organs.end() != serialItor)
			static_cast<Slice *>(*serialItor)->pulse();
	} else if (PARELLEL == mode) {
		for (auto organ : organs)
			static_cast<Slice *>(organ)->pulse();
	}
}

void Slice::organEnd(Slice *organ) {
	//    qlog << "organEnd " << organ->attr->name.value << qdebug;
	if (SERIAL == mode) {
		if (organs.end() != serialItor) {
			++serialItor;
		}
		if (rewindOrgan && organs.end() == serialItor) {
			serialItor = organs.begin();
		}
		if (organs.end() != serialItor) {
			Slice *slice = static_cast<Slice *>(*serialItor);
			slice->raiseView();
			slice->play();
		} else
			end();
	} else if (PARELLEL == mode) {
		bool allEnd = true;
		for (auto organItor : organs) {
			if (!static_cast<Slice *>(organItor)->endOnce) {
				allEnd = false;
				break;
			}
		}
		if (allEnd) {
			end();
		} else {
			if (rewindOrgan)
				organ->play();
		}
	}
}

void Slice::end() {
	//    qlog << "end " << attr->name.value << qdebug;

	endOnce = true;
	playing = false;
	if (owner)
		owner->organEnd(this);
}

void Slice::stop() {
	//    qlog << "stop " << attr->name.value << qdebug;
	endOnce = true;
	playing = false;
	for (auto organ : organs) {
		Slice *slice = static_cast<Slice *>(organ);
		slice->stop();
	}
}

SliceAttr *Slice::attr() {
	return attr_;
}

QWidget *Slice::view() {
	return view_;
}
