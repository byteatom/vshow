#include "main_ui.h"
#include "ui_main_ui.h"

#include <QTranslator>
#include <QCloseEvent>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QCoreApplication>
#include <QDesktopServices>

#include "data/data.h"
#include "data/slice_attr.h"
#include "slice.h"
#include "screen.h"
#include "program.h"
#include "region.h"
#include "text.h"
#include "picture.h"
#include "watch.h"
#include "clock.h"
#include "timer.h"
#include "office.h"
#include "util/win_util.h"

#include "qlog/qlog.h"

MainUi *MainUi::Obj = nullptr;

MainUi::MainUi()
	: QMainWindow(nullptr /*, Qt::MSWindowsFixedSizeDialogHint*/),
	  ui(new Ui::MainUi),
	  treePopMenu{new QMenu{this}},
	  project{nullptr} {
	assert(!Obj);
	Obj = this;

	ui->setupUi(this);

	Animator::check();

	sliceCfger = new SliceCfger();
	ui->attr->layout()->addWidget(sliceCfger);

	bgFileCfger = new FileSrcCfger();
	ui->attr->layout()->addWidget(bgFileCfger);

	bgScaleCfger = new ImageCfger();
	ui->attr->layout()->addWidget(bgScaleCfger);

	geoCfger = new GeoCfger();
	ui->attr->layout()->addWidget(geoCfger);

	screenCfger = new ScreenCfger();
	ui->attr->layout()->addWidget(screenCfger);

	programCfger = new ProgramCfger();
	ui->attr->layout()->addWidget(programCfger);

	segmentCfger = new SegmentCfger();
	ui->attr->layout()->addWidget(segmentCfger);

	fileSrcCfger = new FileSrcCfger();
	ui->attr->layout()->addWidget(fileSrcCfger);

	textCfger = new TextCfger();
	ui->attr->layout()->addWidget(textCfger);

	pictureCfger = new ImageCfger();
	ui->attr->layout()->addWidget(pictureCfger);

	animateCfger = new AnimateCfger();
	ui->attr->layout()->addWidget(animateCfger);

	videoCfger = new VideoCfger();
	ui->attr->layout()->addWidget(videoCfger);

	watchCfger = new WatchCfger();
	ui->attr->layout()->addWidget(watchCfger);

	clockCfger = new ClockCfger();
	ui->attr->layout()->addWidget(clockCfger);

	timerCfger = new TimerCfger();
	ui->attr->layout()->addWidget(timerCfger);


	QSpacerItem *spacer
		= new QSpacerItem(1, 1, QSizePolicy::Preferred, QSizePolicy::Expanding);
	ui->attr->layout()->addItem(spacer);

	connect(ui->newAct, &QAction::triggered, this, &MainUi::newProject);
	connect(ui->openAct, &QAction::triggered, this, &MainUi::open);
	connect(ui->saveAct, &QAction::triggered, this, &MainUi::save);
	connect(ui->saveAsAct, &QAction::triggered, this, &MainUi::saveAs);
	connect(ui->exitAct, &QAction::triggered, this, &MainUi::close);
	connect(ui->screenAct, &QAction::triggered, this, &MainUi::addScreen);
	connect(ui->programAct, &QAction::triggered, this, &MainUi::addProgram);
	connect(ui->regionAct, &QAction::triggered, this, &MainUi::addRegion);
	connect(ui->textAct, &QAction::triggered, this, &MainUi::addText);
	connect(ui->pictureAct, &QAction::triggered, this, &MainUi::addPicture);
	connect(ui->videoAct, &QAction::triggered, this, &MainUi::addVideo);
	connect(ui->watchAct, &QAction::triggered, this, &MainUi::addWatch);
	connect(ui->clockAct, &QAction::triggered, this, &MainUi::addClock);
	connect(ui->timerAct, &QAction::triggered, this, &MainUi::addTimer);
	connect(ui->officeAct, &QAction::triggered, this, &MainUi::addOffice);
	connect(ui->delAct, &QAction::triggered, this, &MainUi::delSelectedItems);
	connect(ui->playAllAct, &QAction::triggered, this, &MainUi::playAll);
	connect(ui->stopAllAct, &QAction::triggered, this, &MainUi::stopAll);
	connect(ui->cardAct, &QAction::triggered, this, &MainUi::card);
	connect(ui->zhcnAct, &QAction::triggered, this, &MainUi::langZhCn);
	connect(ui->zhtwAct, &QAction::triggered, this, &MainUi::langZhTw);
	connect(ui->engbAct, &QAction::triggered, this, &MainUi::langEnGb);
	connect(ui->contextHelpAct, &QAction::triggered, this, &MainUi::help);
	connect(ui->aboutAct, &QAction::triggered, this, &MainUi::about);
	connect(ui->projectTree,
			&QTreeWidget::currentItemChanged,
			this,
			&MainUi::currentItemChanged);
	connect(ui->projectTree,
			&QTreeWidget::itemSelectionChanged,
			this,
			&MainUi::selectedItemChanged);
	connect(ui->expandAllAct, &QAction::triggered, this, &MainUi::expandAll);
	connect(
		ui->collapseAllAct, &QAction::triggered, this, &MainUi::collapseAll);

	ui->projectTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->projectTree,
			&QTreeWidget::customContextMenuRequested,
			this,
			&MainUi::treeMenu);
	connect(ui->moveUpAct, &QAction::triggered, this, &MainUi::moveUp);
	connect(ui->moveDownAct, &QAction::triggered, this, &MainUi::moveDown);

	treePopMenu->addAction(ui->expandAllAct);
	treePopMenu->addAction(ui->collapseAllAct);
	treePopMenu->addSeparator();
	treePopMenu->addAction(ui->screenAct);
	treePopMenu->addAction(ui->programAct);
	treePopMenu->addAction(ui->regionAct);
	treePopMenu->addSeparator();
	treePopMenu->addAction(ui->textAct);
	treePopMenu->addAction(ui->pictureAct);
	treePopMenu->addAction(ui->videoAct);
	treePopMenu->addAction(ui->watchAct);
	treePopMenu->addAction(ui->clockAct);
	treePopMenu->addAction(ui->timerAct);
	treePopMenu->addAction(ui->officeAct);
	treePopMenu->addSeparator();
	treePopMenu->addAction(ui->moveUpAct);
	treePopMenu->addAction(ui->moveDownAct);
	treePopMenu->addSeparator();
	treePopMenu->addAction(ui->delAct);
}

MainUi::~MainUi() {
	delete ui;
}

void MainUi::show() {
	project = new Project(ui->projectTree->invisibleRootItem(),
						  Data::obj->projectAttr);

	QMainWindow::show();
}

void MainUi::newProject() {
	ui->projectTree->clear();
	delete project;
	project = new Project(ui->projectTree->invisibleRootItem());
	Data::obj->appAttr->projectFile = std::wstring();
}

void MainUi::open() {
	QString file
		= QFileDialog::getOpenFileName(this,
									   tr("Open Project"),
									   QCoreApplication::applicationDirPath(),
									   tr("Project (*.xml)"));
	if (file.isEmpty())
		return;
}

void MainUi::save() {
	if (Data::obj->appAttr->projectFile.empty()) {
		Data::obj->appAttr->projectFile
			= QFileDialog::getSaveFileName(
				  this,
				  tr("Save Project"),
				  QCoreApplication::applicationDirPath(),
				  tr("Project (*.xml)")).toStdWString();
	}
	if (Data::obj->appAttr->projectFile.empty())
		return;
	project->attr()->save(nullptr, nullptr);
}

void MainUi::saveAs() {
	QString file
		= QFileDialog::getSaveFileName(this,
									   tr("Save Project"),
									   QCoreApplication::applicationDirPath(),
									   tr("Project (*.xml)"));
	if (file.isEmpty())
		return;

	Data::obj->appAttr->projectFile = file.toStdWString();
	project->attr()->save(nullptr, nullptr);
}

template <class SliceType>
QTreeWidgetItem *MainUi::getParent() {
	QTreeWidgetItem *parent = ui->projectTree->currentItem();

	// empty
	if (!parent) {
		if (SliceType::typeId == SCREEN_SLICE)
			return ui->projectTree->invisibleRootItem();
		if (SliceType::typeId > SCREEN_SLICE)
			addScreen();
		if (SliceType::typeId > PROGRAM_SLICE)
			addProgram();
		if (SliceType::typeId > REGION_SLICE)
			addRegion();
		return ui->projectTree->currentItem();
	}

	// search parent
	int type;
	while (parent) {
		type = parent->type();
		if (type > SliceType::OwnerType::typeId) {
			parent = parent->parent();
			if (!parent)
				parent = ui->projectTree->invisibleRootItem();
		} else if (type < SliceType::OwnerType::typeId) {
			if (0 == parent->childCount()) {
				if (type == SCREEN_SLICE)
					addProgram();
				if (type == PROGRAM_SLICE)
					addRegion();
			}
			parent = parent->child(parent->childCount() - 1);
		} else
			break;
	}

	return parent;
}

template <class SliceType>
typename SliceType::OwnerType *MainUi::getOwner(QTreeWidgetItem *parent) {
	return static_cast<SliceType::OwnerType *>(
		parent->data(0, Qt::UserRole).value<void *>());
}

template <class SliceType>
SliceType *MainUi::newItem(typename SliceType::OwnerType *owner,
						   QTreeWidgetItem *parent) {
	QTreeWidgetItem *item
		= new QTreeWidgetItem{parent, QStringList(""), SliceType::typeId};
	SliceType *slice = new SliceType(owner, item);
	item->setData(
		0, Qt::UserRole, QVariant::fromValue(static_cast<void *>(slice)));
	item->setText(0, QString::fromStdWString(slice->attr()->name));
	return slice;
}

template <class SliceType>
SliceType *MainUi::newFileItems(typename SliceType::OwnerType *owner,
								QTreeWidgetItem *parent) {
	SliceType *slice = nullptr;

	QStringList filesTmp = QFileDialog::getOpenFileNames(this,
														 tr("Select files"),
														 "",
														 SliceType::fileFilter,
														 nullptr,
														 QFileDialog::ReadOnly);
	QStringList files = filesTmp;    //!!! need a copy
	QStringList::iterator file = files.begin();
	while (file != files.end()) {
		slice = newItem<SliceType>(owner, parent);
		slice->attr()->file = file->toStdWString();
		++file;
	}
	return slice;
}

template <class SliceType>
void MainUi::addItem() {
	QTreeWidgetItem *parent = getParent<SliceType>();
	SliceType::OwnerType *owner = getOwner<SliceType>(parent);
	SliceType *slice = newItem<SliceType>(owner, parent);
	if (slice)
		ui->projectTree->setCurrentItem(slice->treeItem);
}

template <class SliceType>
void MainUi::addFileItems() {
	QTreeWidgetItem *parent = getParent<SliceType>();
	SliceType::OwnerType *owner = getOwner<SliceType>(parent);
	SliceType *slice = newFileItems<SliceType>(owner, parent);
	if (slice)
		ui->projectTree->setCurrentItem(slice->treeItem);
}

void MainUi::addScreen() {
	addItem<Screen>();
}

void MainUi::addProgram() {
	addItem<Program>();
}

void MainUi::addRegion() {
	addItem<Region>();
}

void MainUi::addText() {
	addItem<Text>();
}

void MainUi::addPicture() {
	addFileItems<Picture>();
}

void MainUi::addVideo() {
	addFileItems<Video>();
}

void MainUi::addWatch() {
	addItem<Watch>();
}

void MainUi::addClock() {
	addItem<Clock>();
}

void MainUi::addTimer() {
	addItem<Timer>();
}

void MainUi::addOffice() {
	addFileItems<Office>();
}

void MainUi::delItem(QTreeWidgetItem *item) {
	Slice *slice
		= static_cast<Slice *>(item->data(0, Qt::UserRole).value<void *>());
	item->setData(
		0, Qt::UserRole, QVariant::fromValue(static_cast<void *>(nullptr)));
	delete item;
	delete slice;
}

void MainUi::delSelectedItems() {
	ui->projectTree->blockSignals(true);

	QList<QTreeWidgetItem *> itemList = ui->projectTree->selectedItems();

	QList<QTreeWidgetItem *> itemListArray[SEGMENT_SLICE];
	for (auto item : itemList) {
		itemListArray[SEGMENT_SLICE > item->type()
						  ? SEGMENT_SLICE - item->type()
						  : 0].push_back(item);
	}

	QTreeWidgetItem *lastItem = nullptr;
	for (int i = SEGMENT_SLICE - 1; i >= 0; i--) {
		if (!itemListArray[i].isEmpty()) {
			lastItem = itemListArray[i].takeLast();
			break;
		}
	}

	for (int i = 0; i < SEGMENT_SLICE; ++i) {
		for (auto item : itemListArray[i]) {
			delItem(item);
		}
	}

	QTreeWidgetItem *nextItem = nullptr;
	if (lastItem) {
		// next sibling
		QTreeWidgetItem *parent = lastItem->parent();
		if (!parent)
			parent = ui->projectTree->invisibleRootItem();
		int i = parent->indexOfChild(const_cast<QTreeWidgetItem *>(lastItem));
		nextItem = parent->child(i + 1);

		if (!nextItem)
			nextItem = ui->projectTree->itemAbove(lastItem);

		delItem(lastItem);
	}

	if (nextItem) {
		ui->projectTree->setCurrentItem(nextItem);
		currentItemChanged(nextItem, nullptr);
	}

	ui->projectTree->blockSignals(false);
}

void MainUi::currentItemChanged(QTreeWidgetItem *current,
								QTreeWidgetItem *previous) {
	if (previous) {
		Slice *slicePrevious = static_cast<Slice *>(
			previous->data(0, Qt::UserRole).value<void *>());
		if (slicePrevious)
			slicePrevious->deflate();
	}
	if (current) {
		Slice *sliceCurrent = static_cast<Slice *>(
			current->data(0, Qt::UserRole).value<void *>());
		if (sliceCurrent)
			sliceCurrent->inflate();
	}
}

void MainUi::selectedItemChanged() {
	ui->delAct->setEnabled(ui->projectTree->selectedItems().count() > 0);
}

void MainUi::treeMenu(const QPoint &pos) {
	QTreeWidgetItem *currentItem = ui->projectTree->itemAt(pos);

	ui->expandAllAct->setEnabled(false);
	ui->collapseAllAct->setEnabled(false);
	for (int i = 0; i < ui->projectTree->topLevelItemCount(); ++i) {
		QTreeWidgetItem *topItem = ui->projectTree->topLevelItem(i);
		if (topItem->childCount() > 0) {
			ui->expandAllAct->setEnabled(true);
			ui->collapseAllAct->setEnabled(true);
			break;
		}
	}

	ui->moveUpAct->setEnabled(false);
	ui->moveDownAct->setEnabled(false);
	if (currentItem) {
		QTreeWidgetItem *parentItem = currentItem->parent();
		if (!parentItem)
			parentItem = ui->projectTree->invisibleRootItem();
		int itemIndex = parentItem->indexOfChild(
			const_cast<QTreeWidgetItem *>(currentItem));

		ui->moveUpAct->setEnabled(itemIndex > 0);
		ui->moveDownAct->setEnabled(itemIndex < parentItem->childCount() - 1);
	}

	treePopMenu->exec(ui->projectTree->viewport()->mapToGlobal(pos));
}

void MainUi::moveUp() {
	QTreeWidgetItem *item = ui->projectTree->currentItem();
	QTreeWidgetItem *parentItem = item->parent();
	int itemIndex = parentItem->indexOfChild(item);
	parentItem->takeChild(itemIndex);
	parentItem->insertChild(itemIndex - 1, item);
	Slice *slice
		= static_cast<Slice *>(item->data(0, Qt::UserRole).value<void *>());
	slice->fore();
	slice->attr()->fore();
}

void MainUi::moveDown() {
	QTreeWidgetItem *item = ui->projectTree->currentItem();
	QTreeWidgetItem *parentItem = item->parent();
	int itemIndex = parentItem->indexOfChild(item);
	parentItem->takeChild(itemIndex);
	parentItem->insertChild(itemIndex + 1, item);
	Slice *slice
		= static_cast<Slice *>(item->data(0, Qt::UserRole).value<void *>());
	slice->back();
	slice->attr()->back();
}

void MainUi::expandAll() {
	ui->projectTree->expandAll();
}

void MainUi::collapseAll() {
	ui->projectTree->collapseAll();
}

void MainUi::playAll() {
	ui->newAct->setEnabled(false);
	ui->openAct->setEnabled(false);
	ui->saveAct->setEnabled(false);
	ui->saveAsAct->setEnabled(false);
	ui->screenAct->setEnabled(false);
	ui->programAct->setEnabled(false);
	ui->regionAct->setEnabled(false);
	ui->textAct->setEnabled(false);
	ui->pictureAct->setEnabled(false);
	ui->videoAct->setEnabled(false);
	ui->watchAct->setEnabled(false);
	ui->clockAct->setEnabled(false);
	ui->timerAct->setEnabled(false);
	ui->officeAct->setEnabled(false);
	ui->delAct->setEnabled(false);
	ui->playAllAct->setEnabled(false);
	ui->stopAllAct->setEnabled(true);
	ui->cardAct->setEnabled(false);
	ui->projectTree->setEnabled(false);
	ui->attr->setEnabled(false);
	ui->zhcnAct->setEnabled(false);
	ui->zhtwAct->setEnabled(false);
	ui->engbAct->setEnabled(false);
	ui->contextHelpAct->setEnabled(false);
	ui->aboutAct->setEnabled(false);

	QThread::currentThread()->setPriority(QThread::HighPriority);
	project->start();
}

void MainUi::stopAll() {
	project->stop();
	QThread::currentThread()->setPriority(QThread::NormalPriority);

	ui->newAct->setEnabled(true);
	ui->openAct->setEnabled(true);
	ui->saveAct->setEnabled(true);
	ui->saveAsAct->setEnabled(true);
	ui->screenAct->setEnabled(true);
	ui->programAct->setEnabled(true);
	ui->regionAct->setEnabled(true);
	ui->textAct->setEnabled(true);
	ui->pictureAct->setEnabled(true);
	ui->videoAct->setEnabled(true);
	ui->watchAct->setEnabled(true);
	ui->clockAct->setEnabled(true);
	ui->timerAct->setEnabled(true);
	ui->officeAct->setEnabled(true);
	ui->delAct->setEnabled(true);
	ui->playAllAct->setEnabled(true);
	ui->stopAllAct->setEnabled(false);
	ui->cardAct->setEnabled(true);
	ui->projectTree->setEnabled(true);
	ui->attr->setEnabled(true);
	ui->zhcnAct->setEnabled(true);
	ui->zhtwAct->setEnabled(true);
	ui->engbAct->setEnabled(true);
	ui->contextHelpAct->setEnabled(true);
	ui->aboutAct->setEnabled(true);
}

void MainUi::card() {
	CardUi cardUi(this);
	cardUi.exec();
}

void MainUi::langZhCn() {
	QLocale china(QLocale::Chinese, QLocale::China);
	QString appDir = QCoreApplication::applicationDirPath();
	QString languageFileDir = appDir + "/" + LANGUAGE_DIR + LANGUAGE_PREFIX
							  + china.name() + LANGUAGE_SUFFIX;
	QMessageBox msg;
	if (QFile::exists(languageFileDir)) {
		Data::obj->appAttr->language = china.name().toStdWString();
		msg.setText(tr("Language has been switched to Chinese."));
		msg.setInformativeText(tr("Please restart the application!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	} else {
		msg.setText(tr("Language dones't switch to Chinese."));
		msg.setInformativeText(languageFileDir + tr("dones't exist!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	}
	msg.exec();
}

void MainUi::langZhTw() {
	QLocale china_TW(QLocale::Chinese, QLocale::Taiwan);
	QString appDir = QCoreApplication::applicationDirPath();

	QString languageFileDir = appDir + "/" + LANGUAGE_DIR + LANGUAGE_PREFIX
							  + china_TW.name() + LANGUAGE_SUFFIX;
	QMessageBox msg;
	if (QFile::exists(languageFileDir)) {
		Data::obj->appAttr->language = china_TW.name().toStdWString();
		msg.setText(tr("Language has been switched to Traditional Chinese."));
		msg.setInformativeText(tr("Please restart the application!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	} else {
		msg.setText(tr("Language dones't switch to Traditional Chinese."));
		msg.setInformativeText(languageFileDir + tr("dones't exist!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	}
	msg.exec();
}

void MainUi::langEnGb() {
	QLocale britain(QLocale::English, QLocale::UnitedKingdom);
	QString appDir = QCoreApplication::applicationDirPath();
	QString languageFileDir = appDir + "/" + LANGUAGE_DIR + LANGUAGE_PREFIX
							  + britain.name() + LANGUAGE_SUFFIX;
	QMessageBox msg;
	if (QFile::exists(languageFileDir)) {
		Data::obj->appAttr->language = britain.name().toStdWString();
		msg.setText(tr("Language has been switched to English."));
		msg.setInformativeText(tr("Please restart the application!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	} else {
		msg.setText(tr("Language dones't switch to English."));
		msg.setInformativeText(languageFileDir + tr("dones't exist!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	}
	msg.exec();
}

void MainUi::japanese() {
	QLocale japan(QLocale::Japanese, QLocale::Japan);
	QString appDir = QCoreApplication::applicationDirPath();
	QString languageFileDir = appDir + "/" + LANGUAGE_DIR + LANGUAGE_PREFIX
							  + japan.name() + LANGUAGE_SUFFIX;
	QMessageBox msg;
	if (QFile::exists(languageFileDir)) {
		Data::obj->appAttr->language = japan.name().toStdWString();
		msg.setText(tr("Language has been switched to Japanese."));
		msg.setInformativeText(tr("Please restart the application!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	} else {
		msg.setText(tr("Language dones't switch to Japanese."));
		msg.setInformativeText(languageFileDir + tr("dones't exist!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	}
	msg.exec();
}

void MainUi::korean() {
	QLocale korean(QLocale::Korean, QLocale::SouthKorea);
	QString appDir = QCoreApplication::applicationDirPath();
	QString languageFileDir = appDir + "/" + LANGUAGE_DIR + LANGUAGE_PREFIX
							  + korean.name() + LANGUAGE_SUFFIX;
	QMessageBox msg;
	if (QFile::exists(languageFileDir)) {
		Data::obj->appAttr->language = korean.name().toStdWString();
		msg.setText(tr("Language has been switched to Korean."));
		msg.setInformativeText(tr("Please restart the application!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	} else {
		msg.setText(tr("Language dones't switch to Korean."));
		msg.setInformativeText(languageFileDir + tr("dones't exist!"));
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	}
	msg.exec();
}


void MainUi::help() {
	QString helpFilePath = QCoreApplication::applicationDirPath()
						   + "/help/help.chm";
	QDesktopServices::openUrl(QUrl(helpFilePath));
}

void MainUi::about() {
	AboutDialog aboutDlg(this);
	aboutDlg.exec();
}

bool MainUi::nativeEvent(const QByteArray &eventType,
						 void *message,
						 long *result) {
	MSG *msg = static_cast<MSG *>(message);
	if (msg->message != WM_PULSE)
		return false;

	if (project)
		project->pulse();
	*result = 0;
	return true;
}

void MainUi::closeEvent(QCloseEvent *event) {
	if (project->playing)
		stopAll();
	if (Data::obj->appAttr->projectFile.empty()) {
		Data::obj->appAttr->projectFile
			= QFileDialog::getSaveFileName(
				  this,
				  tr("Save Project"),
				  QCoreApplication::applicationDirPath(),
				  tr("Project (*.xml)")).toStdWString();
	}
	Data::obj->save();
	delete project;
	project = nullptr;
	event->accept();
}
