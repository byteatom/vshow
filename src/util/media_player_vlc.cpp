#include "util/media_player_vlc.h"

#include <QApplication>

VlcInstance *MediaPlayerVlc::vlcInstance = nullptr;

MediaPlayerVlc::MediaPlayerVlc() :
    VlcMediaPlayer{vlcInstance ? vlcInstance : (vlcInstance =
        new VlcInstance{VlcCommon::args(), qApp})},
    vlcMedia{nullptr}
{
}

MediaPlayerVlc::~MediaPlayerVlc()
{
    stop();
    if (vlcMedia) delete vlcMedia;
}

void MediaPlayerVlc::open(QString &path)
{
    if (vlcMedia) delete vlcMedia;
    vlcMedia = new VlcMedia(path, true, vlcInstance);
    VlcMediaPlayer::openOnly(vlcMedia);
}

void MediaPlayerVlc::play()
{
    if (Vlc::Paused == state())
        VlcMediaPlayer::resume();
    else
        VlcMediaPlayer::play();
}

void MediaPlayerVlc::stopSilent()
{
    blockSignals(true);
    stop();
    blockSignals(false);
}


