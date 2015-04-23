#include "util/media_player_vlc.h"

#include <QApplication>

VlcInstance *MediaPlayerVlc::vlcInstance = nullptr;

MediaPlayerVlc::MediaPlayerVlc() :
	VlcMediaPlayer(vlcInstance ? vlcInstance :
		(vlcInstance = new VlcInstance{VlcCommon::args(), qApp})),
	vlcMedia{nullptr}
{
	stopTask = std::bind(&MediaPlayerVlc::stop, this);
}

MediaPlayerVlc::~MediaPlayerVlc()
{
	//fut = std::async(std::launch::async, [this](){stop();});
	//fut.wait();
	stop();
	if (vlcMedia) delete vlcMedia;
}

void MediaPlayerVlc::open(QString &path)
{
	if (vlcMedia) delete vlcMedia;
	vlcMedia = new VlcMedia(path, true, vlcInstance);
	openOnly(vlcMedia);
	//fut = std::async(std::launch::async, [this](){openOnly(vlcMedia);});
	//fut.wait();
}

void MediaPlayerVlc::play()
{
	if (Vlc::Paused == state()) {
		resume();
		//fut = std::async(std::launch::async, [this](){resume();});
	} else {
		VlcMediaPlayer::play();
		//fut = std::async(std::launch::async, [this](){VlcMediaPlayer::play();});
	}
	//fut.wait();
}

void MediaPlayerVlc::stopSilent()
{
	blockSignals(true);
	stop();
	//tasker.addTask(&stopTask);
	//fut = std::async(std::launch::async, [this](){stop();});
	//fut.wait();
	blockSignals(false);
}


