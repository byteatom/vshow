#ifndef MEDIA_PLAYER_VLC_H
#define MEDIA_PLAYER_VLC_H

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

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
};

#endif // MEDIA_PLAYER_VLC_H
