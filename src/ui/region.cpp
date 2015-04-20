#include "region.h"

#include "main_ui.h"
#include "util/qt_util.h"
#include "picture.h"
#include "office.h"

const SliceTypeId Region::typeId{REGION_SLICE};

Region::Region(Program *owner, QTreeWidgetItem *treeItem, RegionAttr *attr)
    : Slice{owner, treeItem}, owner{owner} {
    attr_ = attr;
    view_ = new RegionView{this};
    init(this->attr(), this->view());

    mode = SERIAL;

    if (this->attr()->name.value.empty())
        this->attr()->name
            = tr("Region %1").arg(owner->attr()->nextChildNum).toStdWString();

    for (auto organAttr : this->attr()->organs) {
        SliceAttr *sliceAttr = static_cast<SliceAttr *>(organAttr);
        if (sliceAttr->type == TEXT_SLICE) {
            TextAttr *textAttr = static_cast<TextAttr *>(organAttr);
            QTreeWidgetItem *textItem = new QTreeWidgetItem{
                treeItem,
                QStringList(QString::fromStdWString(textAttr->name)),
                Text::typeId};
            Text *text = new Text(this, textItem, textAttr);
            textItem->setData(0,
                              Qt::UserRole,
                              QVariant::fromValue(static_cast<void *>(text)));
        } else if (sliceAttr->type == PICTURE_SLICE) {
            PictureAttr *pictureAttr = static_cast<PictureAttr *>(organAttr);
            QTreeWidgetItem *pictureItem = new QTreeWidgetItem{
                treeItem,
                QStringList(QString::fromStdWString(pictureAttr->name)),
                Picture::typeId};
            Picture *picture = new Picture(this, pictureItem, pictureAttr);
            pictureItem->setData(
                0,
                Qt::UserRole,
                QVariant::fromValue(static_cast<void *>(picture)));
        } else if (sliceAttr->type == VIDEO_SLICE) {
            VideoAttr *videoAttr = static_cast<VideoAttr *>(organAttr);
            QTreeWidgetItem *videoItem = new QTreeWidgetItem{
                treeItem,
                QStringList(QString::fromStdWString(videoAttr->name)),
                Video::typeId};
            Video *video = new Video(this, videoItem, videoAttr);
            videoItem->setData(0,
                               Qt::UserRole,
                               QVariant::fromValue(static_cast<void *>(video)));
        } else if (sliceAttr->type == WATCH_SLICE) {
            WatchAttr *watchAttr = static_cast<WatchAttr *>(organAttr);
            QTreeWidgetItem *watchItem = new QTreeWidgetItem{
                treeItem,
                QStringList(QString::fromStdWString(watchAttr->name)),
                Watch::typeId};
            Watch *watch = new Watch(this, watchItem, watchAttr);
            watchItem->setData(0,
                               Qt::UserRole,
                               QVariant::fromValue(static_cast<void *>(watch)));
        } else if (sliceAttr->type == CLOCK_SLICE) {
            ClockAttr *clockAttr = static_cast<ClockAttr *>(organAttr);
            QTreeWidgetItem *clockItem = new QTreeWidgetItem{
                treeItem,
                QStringList(QString::fromStdWString(clockAttr->name)),
                Clock::typeId};
            Clock *clock = new Clock(this, clockItem, clockAttr);
            clockItem->setData(0,
                               Qt::UserRole,
                               QVariant::fromValue(static_cast<void *>(clock)));
        } else if (sliceAttr->type == TIMER_SLICE) {
            TimerAttr *timerAttr = static_cast<TimerAttr *>(organAttr);
            QTreeWidgetItem *timerItem = new QTreeWidgetItem{
                treeItem,
                QStringList(QString::fromStdWString(timerAttr->name)),
                Timer::typeId};
            Timer *timer = new Timer(this, timerItem, timerAttr);
            timerItem->setData(0,
                               Qt::UserRole,
                               QVariant::fromValue(static_cast<void *>(timer)));
        } else if (sliceAttr->type == OFFICE_SLICE) {
            OfficeAttr *officeAttr = static_cast<OfficeAttr *>(organAttr);
            QTreeWidgetItem *officeItem = new QTreeWidgetItem{
                treeItem,
                QStringList(QString::fromStdWString(officeAttr->name)),
                Office::typeId};
            Office *office = new Office(this, officeItem, officeAttr);
            officeItem->setData(
                0,
                Qt::UserRole,
                QVariant::fromValue(static_cast<void *>(office)));
        }
    }
}

Region::Region(Program *owner, QTreeWidgetItem *treeItem)
    : Region{owner, treeItem, new RegionAttr{owner->attr()}} {
}

Region::~Region() {
    hideCfgers();
}

void Region::showCfgers() {
    Slice::showCfgers();
    QSize parentSize = QtUtil::parentSize(view());
    attr()->geo.setConstraint(parentSize.width(), parentSize.height());
    MainUi::Obj->geoCfger->show(&attr()->geo);
}

void Region::hideCfgers() {
    Slice::hideCfgers();
    MainUi::Obj->geoCfger->hide();
}

void Region::start() {
    view()->mouseGeo.setEnabled(false);
    Slice::start();
}

void Region::stop() {
    Slice::stop();
    view()->mouseGeo.setEnabled(true);
}

RegionAttr *Region::attr() {
    return static_cast<RegionAttr *>(attr_);
}

RegionView *Region::view() {
    return static_cast<RegionView *>(view_);
}
