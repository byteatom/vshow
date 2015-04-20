#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>

#include "segment.h"
#include "media_player.h"
#include "video_view.h"
#include "data/video_attr.h"

class Video : public Segment {
    Q_OBJECT
public:
    static const SliceTypeId typeId;
    static const QString fileFilter;
    static const QString fileTitle;

    Video(Region *owner, QTreeWidgetItem *treeItem, VideoAttr *attr);
    Video(Region *owner, QTreeWidgetItem *treeItem);
    virtual ~Video();

    void videoPlay();
    void videoPause();
    void videoStop();
    void preview(bool play);

    Vlc::State videoState();
    void fileChanged(std::wstring &path);
    void playerStateChanged();
    void playerEnd();
    void playerStopped();

    virtual void showCfgers() override;
    virtual void hideCfgers() override;
    virtual void deflate() override;

    virtual void play() override;
    virtual void stop() override;
    VideoAttr *attr();
    VideoView *view();

    MediaPlayer *player;
    QTimer previewTimer;
};

#endif    // VIDEO_H
