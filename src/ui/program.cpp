#include "program.h"

#include "main_ui.h"

const SliceTypeId Program::typeId{PROGRAM_SLICE};

Program::Program(Screen *owner, QTreeWidgetItem *treeItem, ProgramAttr *attr)
	: Slice{owner, treeItem}, owner{owner} {
	attr_ = attr;
	view_ = new ProgramView{this};
	init(this->attr(), this->view());

	mode = PARELLEL;
	rewindOrgan = true;

	if (this->attr()->name.value.empty())
		this->attr()->name
			= Program::tr("Program %1").arg(owner->attr()->nextChildNum).toStdWString();

	for (auto organAttr : this->attr()->organs) {
		RegionAttr *regionAttr = static_cast<RegionAttr *>(organAttr);
		QTreeWidgetItem *regionItem = new QTreeWidgetItem{
			treeItem,
			QStringList(QString::fromStdWString(regionAttr->name)),
			Region::typeId};
		Region *region = new Region(this, regionItem, regionAttr);
		regionItem->setData(
			0, Qt::UserRole, QVariant::fromValue(static_cast<void *>(region)));
	}
}

Program::Program(Screen *owner, QTreeWidgetItem *treeItem)
	: Program{owner, treeItem, new ProgramAttr{owner->attr()}} {
}

Program::~Program() {
	hideCfgers();
}

void Program::showCfgers() {
	Slice::showCfgers();
	// MainUi::Obj->programCfger->show(attr);
}

void Program::hideCfgers() {
	Slice::hideCfgers();
	MainUi::Obj->programCfger->hide();
}

void Program::raiseView() {
	Slice::raiseView();
	owner->view()->layout.setCurrentWidget(view());
}

ProgramAttr *Program::attr() {
	return static_cast<ProgramAttr *>(attr_);
}

ProgramView *Program::view() {
	return static_cast<ProgramView *>(view_);
}
