#ifndef MAIN_UI_H
#define MAIN_UI_H

#include <list>
#include <unordered_map>

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>

#include "io/io.h"
#include "project.h"

#include "slice_cfger.h"
#include "screen_cfger.h"
#include "program_cfger.h"
#include "geo_cfger.h"
#include "segment_cfger.h"
#include "filesrc_cfger.h"
#include "text_cfger.h"
#include "image_cfger.h"
#include "animate_cfger.h"
#include "video_cfger.h"
#include "watch_cfger.h"
#include "clock_cfger.h"
#include "timer_cfger.h"
#include "card_ui.h"
#include "about_dialog.h"

#include "qlog/qlog.h"

#define LANGUAGE_DIR "language/"
#define LANGUAGE_PREFIX "vshow_"
#define LANGUAGE_SUFFIX ".qm"

namespace Ui {
class MainUi;
}

class MainUi : public QMainWindow {
    Q_OBJECT

public:
    static MainUi *Obj;

    explicit MainUi();
    virtual ~MainUi();

    void show();
    void newProject();
    void open();
    void save();
    void saveAs();

    template <class SliceType>
    QTreeWidgetItem *getParent();
    template <class SliceType>
    typename SliceType::OwnerType *getOwner(QTreeWidgetItem *parent);
    template <class SliceType>
    SliceType *newItem(typename SliceType::OwnerType *owner,
                       QTreeWidgetItem *parent);
    template <class SliceType>
    SliceType *newFileItems(typename SliceType::OwnerType *owner,
                            QTreeWidgetItem *parent);
    template <class SliceType>
    void addItem();
    template <class SliceType>
    void addFileItems();
    void addScreen();
    void addProgram();
    void addRegion();
    void addText();
    void addPicture();
    void addVideo();
    void addWatch();
    void addClock();
    void addTimer();
    void addOffice();
    void delItem(QTreeWidgetItem *item);
    void delSelectedItems();
    void currentItemChanged(QTreeWidgetItem *current,
                            QTreeWidgetItem *previous);
    void selectedItemChanged();
    void treeMenu(const QPoint &pos);
    void moveUp();
    void moveDown();
    void expandAll();
    void collapseAll();
    void playAll();
    void stopAll();
    void card();
    void langZhCn();
    void langZhTw();
    void langEnGb();
    void japanese();
    void korean();
    void help();
    void about();

    Project *project;

    SliceCfger *sliceCfger;
    FileSrcCfger *bgFileCfger;
    ImageCfger *bgScaleCfger;
    GeoCfger *geoCfger;
    ScreenCfger *screenCfger;
    ProgramCfger *programCfger;
    SegmentCfger *segmentCfger;
    FileSrcCfger *fileSrcCfger;
    TextCfger *textCfger;
    ImageCfger *pictureCfger;
    AnimateCfger *animateCfger;
    VideoCfger *videoCfger;
    WatchCfger *watchCfger;
    ClockCfger *clockCfger;
    TimerCfger *timerCfger;

protected:
    bool nativeEvent(const QByteArray &eventType,
                     void *message,
                     long *result) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainUi *ui;
    QMenu *treePopMenu;
};

#endif    // MAIN_UI_H
