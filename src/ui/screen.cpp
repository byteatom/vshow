#include "screen.h"

#include "main_ui.h"
#include "util/qt_util.h"

const SliceTypeId Screen::typeId{SCREEN_SLICE};

Screen::Screen(Project *owner, QTreeWidgetItem *treeItem, ScreenAttr *attr)
    : Slice{owner, treeItem}, owner{owner} {
    attr_ = attr;
    view_ = new ScreenView{this};
    init(this->attr(), this->view());

    if (this->attr()->name.value.empty())
        this->attr()->name
            = tr("Screen %1").arg(owner->attr()->nextChildNum).toStdWString();
    QSize parentSize = QtUtil::parentSize(view());
    this->attr()->geo.setConstraint(parentSize.width(), parentSize.height());
    this->attr()->lock.addObserver(
        std::bind(&Screen::lockChanged, this, std::placeholders::_1));

    mode = SERIAL;
    rewindOrgan = true;

    for (auto organAttr : this->attr()->organs) {
        ProgramAttr *programAttr = static_cast<ProgramAttr *>(organAttr);
        QTreeWidgetItem *programItem = new QTreeWidgetItem{
            treeItem,
            QStringList(QString::fromStdWString(programAttr->name)),
            Program::typeId};
        Program *program = new Program(this, programItem, programAttr);
        programItem->setData(
            0, Qt::UserRole, QVariant::fromValue(static_cast<void *>(program)));
    }

    playCtrl = new PlayCtrl(view());
    programList = new ProgramList(view());
}

Screen::Screen(Project *owner, QTreeWidgetItem *treeItem)
    : Screen(owner, treeItem, new ScreenAttr{owner->attr()}) {
}

Screen::~Screen() {
    hideCfgers();
    if (playCtrl) {
        delete playCtrl;
        playCtrl = nullptr;
    }
    if (programList) {
        delete programList;
        programList = nullptr;
    }
}

void Screen::showCfgers() {
    Slice::showCfgers();
    MainUi::Obj->sliceCfger->show(attr());
    if (attr()->lock)
        MainUi::Obj->geoCfger->setEnabled(false);
    MainUi::Obj->geoCfger->show(&attr()->geo);
    MainUi::Obj->screenCfger->show(attr());
}

void Screen::raiseView() {
    showView();
}

void Screen::hideCfgers() {
    Slice::hideCfgers();
    MainUi::Obj->sliceCfger->hide();
    MainUi::Obj->geoCfger->hide();
    MainUi::Obj->geoCfger->setEnabled(true);
    MainUi::Obj->screenCfger->hide();
}

void Screen::inflate() {
    for (auto program : organs) {
        static_cast<Program *>(program)->hideView();
    }
    Slice::inflate();
}

void Screen::lockChanged(bool lock) {
    MainUi::Obj->geoCfger->setDisabled(lock);
}

void Screen::start() {
    view()->setEnabled(false);
    playCtrl->setEnabled(true);
    programList->setEnabled(true);
    view()->mouseGeo.setEnabled(false);
    Slice::start();
}

void Screen::pulse() {
    // qlog << "Project::pulse" << qdebug;
    Slice::pulse();
    repaintView();
    // qlog << "Project::pulse end" << qdebug;
}

void Screen::organEnd(Slice *organ) {
    qlog << "organEnd " << organ->attr()->name.value << qdebug;
    if (SERIAL == mode) {
        if (organs.end() != serialItor) {
            ++serialItor;
        }
        if (rewindOrgan && organs.end() == serialItor) {
            serialItor = organs.begin();
        }
        if (organs.end() != serialItor) {
            Slice *slice = static_cast<Slice *>(*serialItor);
            slice->raiseView();
            slice->play();
            programList->refreshProgramList();
        } else
            end();
    } else if (PARELLEL == mode) {
        bool allEnd = true;
        for (auto organItor : organs) {
            if (!static_cast<Slice *>(organItor)->endOnce) {
                allEnd = false;
                break;
            }
        }
        if (allEnd) {
            end();
        } else {
            if (rewindOrgan)
                organ->play();
        }
    }
}

void Screen::stop() {
    Slice::stop();
    view()->setEnabled(true);
    view()->mouseGeo.setEnabled(true);
    updateView();
}

ScreenAttr *Screen::attr() {
    return static_cast<ScreenAttr *>(attr_);
}

ScreenView *Screen::view() {
    return static_cast<ScreenView *>(view_);
}
