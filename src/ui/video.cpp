#include "video.h"

#include "main_ui.h"

const SliceTypeId Video::typeId{VIDEO_SLICE};
const QString Video::fileFilter{
	Video::tr("Video (*.mp4 *.mpg *.wmv *.avi *.mkv *.swf *.flv *.3gp)")};
const QString Video::fileTitle{Video::tr("Video File:")};

Video::Video(Region *owner, QTreeWidgetItem *treeItem, VideoAttr *attr)
	: Segment{owner, treeItem} {
	attr_ = attr;
	view_ = new VideoView{this};
	init(this->attr(), this->view());

	if (this->attr()->name.value.empty())
		this->attr()->name
			= Video::tr("Video %1").arg(owner->attr()->nextChildNum).toStdWString();

	player = new MediaPlayer;
	this->view()->setPlayer(player);
	previewTimer.setInterval(40);

	this->attr()->file.addObserver(
		std::bind(&Video::fileChanged, this, std::placeholders::_1));

	connect(player,
			&MediaPlayerVlc::stateChanged,
			this,
			&Video::playerStateChanged,
			Qt::QueuedConnection);
	connect(player,
			&MediaPlayerVlc::end,
			this,
			&Video::playerEnd,
			Qt::QueuedConnection);
	connect(player,
			&MediaPlayerVlc::stopped,
			this,
			&Video::playerStopped,
			Qt::QueuedConnection);

	connect(&previewTimer,
			&QTimer::timeout,
			view(),
			static_cast<void (VideoView::*)()>(&VideoView::update));
}

Video::Video(Region *owner, QTreeWidgetItem *treeItem)
	: Video{owner, treeItem, new VideoAttr{owner->attr()}} {
}

Video::~Video() {
	hideCfgers();
	delete player;
}

void Video::videoPlay() {
	if (attr()->file.value.empty())
		return;
	if (Vlc::Paused != videoState())
		player->open(QString::fromStdWString(attr()->file));
	player->play();
	if (playing)
		previewTimer.stop();
	else
		previewTimer.start();
}

void Video::videoPause() {
	previewTimer.stop();
	player->pause();
}

void Video::videoStop() {
	previewTimer.stop();
	player->stopSilent();
}

Vlc::State Video::videoState() {
	return player->state();
}

void Video::fileChanged(std::wstring &path) {
	// player->open(QString::fromStdWString(path));
	// player->play();
	// view()->update();
}

void Video::playerStateChanged() {
	if (isCurrent())
		MainUi::Obj->videoCfger->show(this);
}

void Video::playerEnd() {
	previewTimer.stop();
	player->stop();
}

void Video::playerStopped() {
	if (playing)
		end();
}

void Video::showCfgers() {
	Segment::showCfgers();
	MainUi::Obj->fileSrcCfger->setTitle(fileTitle);
	MainUi::Obj->fileSrcCfger->show(&attr()->file, &fileFilter);
	MainUi::Obj->videoCfger->show(this);
}

void Video::hideCfgers() {
	Segment::hideCfgers();
	MainUi::Obj->fileSrcCfger->hide();
	MainUi::Obj->videoCfger->hide();
}

void Video::deflate() {
	videoPause();
	Slice::deflate();
}

void Video::play() {
	Slice::play();
	videoStop();
	videoPlay();
}

void Video::stop() {
	videoStop();
	Slice::stop();
}

VideoAttr *Video::attr() {
	return static_cast<VideoAttr *>(attr_);
}

VideoView *Video::view() {
	return static_cast<VideoView *>(view_);
}
