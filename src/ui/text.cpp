#include "text.h"

#include <QApplication>

#include "main_ui.h"

const SliceTypeId Text::typeId{TEXT_SLICE};

const QString Text::fileFilter{Text::tr("Texts (*.txt)")};

Text::Text(Region *owner, QTreeWidgetItem *treeItem, TextAttr *attr)
	: Segment{owner, treeItem} {
	attr_ = attr;
	view_ = new TextView{this};
	init(this->attr(), this->view());

	if (this->attr()->name.value.empty()) {
		QString name = Text::tr("Text %1");
		this->attr()->name
			= name.arg(owner->attr()->nextChildNum).toStdWString();
	}


	editor = view()->editor;
	editor->setHtml(QString::fromStdWString(this->attr()->html));

	connect(editor,
			&TextEditor::currentCharFormatChanged,
			this,
			&Text::editorFormatChanged);
	connect(editor,
			&TextEditor::cursorPositionChanged,
			this,
			&Text::editorPositionChanged);
	connect(editor, &TextEditor::textChanged, this, &Text::textChanged);

	stayTimer.setSingleShot(true);
	stayTimer.setTimerType(Qt::CoarseTimer);
	connect(&stayTimer, &QTimer::timeout, this, &Text::end);

	task = std::bind(&Text::makeFrame, this);
}

Text::Text(Region *owner, QTreeWidgetItem *treeItem)
	: Text{owner, treeItem, new TextAttr{owner->attr()}} {
}

Text::~Text() {
	hideCfgers();
}

void Text::showCfgers() {
	Segment::showCfgers();
	MainUi::Obj->textCfger->show(this);
	MainUi::Obj->animateCfger->show(&attr()->animateAttr);
}

void Text::hideCfgers() {
	Segment::hideCfgers();
	MainUi::Obj->textCfger->hide();
	MainUi::Obj->animateCfger->hide();
}

void Text::start() {
	//    qlog << "Text::start " << origin.isNull() << qdebug;
	editor->hide();
	Slice::start();
}

void Text::play() {
	// qlog << "Text::play " << attr->name.value << qdebug;

	raiseView();
	editor->show();
	// QApplication::processEvents();
	origin = editor->snapshot();
	editor->hide();

	pulses = attr()->animateAttr.pulses;
	frameMutex.lock();
	animator.build(view()->rect(), origin, &attr()->animateAttr);
	frame = animator.frame();
	frameMutex.unlock();
	Slice::play();
}

void Text::makeFrame() {
	// qlog << "makeFrame " << attr->name.value << qdebug;
	frameMutex.lock();
	frame = animator.frame();
	frameMutex.unlock();
	return;
}

void Text::pulse() {
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

void Text::end() {
	// cache = QPixmap();
	// editor->show();
	Slice::end();
}

void Text::stop() {
	stayTimer.stop();
	cache = QPixmap();
	editor->show();
	Slice::stop();
}

TextAttr *Text::attr() {
	return static_cast<TextAttr *>(attr_);
}

TextView *Text::view() {
	return static_cast<TextView *>(view_);
}

void Text::editorFormatChanged(const QTextCharFormat &format) {
	if (!isCurrent())
		return;

	MainUi::Obj->textCfger->updateBtns();
}

void Text::editorPositionChanged() {
	if (!isCurrent())
		return;

	MainUi::Obj->textCfger->updateBtns();
}

void Text::textChanged() {
	if (!isCurrent())
		return;

	attr()->html = editor->toHtml().toStdWString();
}
