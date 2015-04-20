#include "project.h"

#include <cassert>

#include "data/data.h"
#include "qlog/qlog.h"
#include "screen.h"
#include "main_ui.h"
#include "util/win_util.h"

const SliceTypeId Project::typeId{PROJECT_SLICE};

void Project::atomPulse(void *data) {
    // qlog << "Project::atomPulse" << qdebug;
    // QCoreApplication::postEvent(
    //    (QObject *)data, new QEvent(QEvent::User), INT_MAX);
    ::SendMessage((HWND)MainUi::Obj->winId(), WM_PULSE, 0, 0);
}

Project::Project(QTreeWidgetItem *treeItem, ProjectAttr *attr)
    : Slice{nullptr, treeItem}, pulser{TICK, &atomPulse, this} {
    attr_ = attr;
    Data::obj->projectAttr = this->attr();
    init(this->attr(), nullptr);

    mode = PARELLEL;

    treeItem->setData(
        0, Qt::UserRole, QVariant::fromValue(static_cast<void *>(this)));

    QTreeWidget *treeWidget = treeItem->treeWidget();
    treeWidget->blockSignals(true);
    for (auto organAttr : this->attr()->organs) {
        ScreenAttr *screenAttr = static_cast<ScreenAttr *>(organAttr);
        QTreeWidgetItem *screenItem = new QTreeWidgetItem{
            treeItem,
            QStringList(QString::fromStdWString(screenAttr->name)),
            Screen::typeId};
        Screen *screen = new Screen(this, screenItem, screenAttr);
        screenItem->setData(
            0, Qt::UserRole, QVariant::fromValue(static_cast<void *>(screen)));
    }
    treeWidget->blockSignals(false);
}

Project::Project(QTreeWidgetItem *treeItem)
    : Project{treeItem, new ProjectAttr} {
}

Project::~Project() {
    Data::obj->projectAttr = nullptr;
}

bool Project::event(QEvent *e) {
    if (e->type() == QEvent::User) {
        pulse();
        return true;
    } else {
        return Slice::event(e);
    }
}

void Project::start() {
    tasker.start();
    Slice::start();
    Slice::play();
    pulser.start();
}

void Project::stop() {
    pulser.stop();
    tasker.clear();
    Slice::stop();
}

void Project::startScreen(Slice *screen) {
    if (!playing) {
        QThread::currentThread()->setPriority(QThread::HighPriority);
        tasker.start();
        pulser.start();
    }
    screen->start();
    playScreen(screen);
}

void Project::stopScreen(Slice *screen) {

    bool allStop = true;
    for (auto screenOrgan : organs) {
        if (static_cast<Screen *>(screenOrgan) != static_cast<Screen *>(screen)
            && static_cast<Screen *>(screenOrgan)->playing) {
            allStop = false;
            break;
        }
    }
    if (allStop) {
        pulser.stop();
        tasker.clear();
        QThread::currentThread()->setPriority(QThread::NormalPriority);
    }
    static_cast<Screen *>(screen)->stop();
    if (allStop) {
        Slice::end();
    }
}

void Project::playScreen(Slice *screen) {
    if (!playing) {
        playing = true;
    }
    static_cast<Screen *>(screen)->play();
}

ProjectAttr *Project::attr() {
    return static_cast<ProjectAttr *>(attr_);
}
