#ifndef MEDIA_PLAYER_VLC_H
#define MEDIA_PLAYER_VLC_H

#include <future>

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

#include "task_runner.h"

class MediaPlayerVlc : public VlcMediaPlayer
{
	Q_OBJECT
public:
	friend class VideoViewVlc;

	MediaPlayerVlc();
	virtual ~MediaPlayerVlc();

	virtual void open(QString &path);
	virtual void play();
	void stopSilent();

private:
	static VlcInstance *vlcInstance;
	VlcMedia *vlcMedia;
	//std::future<void> fut;
	TaskRunner tasker;
	Task stopTask;
};

#endif // MEDIA_PLAYER_VLC_H
