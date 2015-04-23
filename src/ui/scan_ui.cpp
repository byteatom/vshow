#include "scan_ui.h"
#include "ui_scan_ui.h"

#include <QTimer>

#include "util/mem_util.h"

const ScanUi::PageAction ScanUi::enterActions[] = {
	&ScanUi::enterModule,
	&ScanUi::enterDataPolarity,
	&ScanUi::enterOePolarity,
	&ScanUi::enterRgbSel,
	&ScanUi::enterIntfLines,
	&ScanUi::enterWrapTimes,
	&ScanUi::enterDotmap,
};

ScanUi::ScanUi(QWidget *parent, RecverCard *card)
	: QWizard(parent),
	  card{card},
	  scan{card->scan},
	  tune{card->tune},
	  dotsPos{nullptr},
	  nextDot{0},
	  ui(new Ui::ScanUi) {
	ui->setupUi(this);

	tune.tuneInit(scan);
	// setFixedSize(size());

	// module
	ui->baseColor->setCurrentIndex(tune.getColorMode() - 1);
	connect(ui->baseColor,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this,
		&ScanUi::baseColorChanged);
	ui->width->setValue(tune.moduleWidth);
	connect(ui->width,
			static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this,
			&ScanUi::widthChanged);
	ui->height->setValue(tune.moduleHeight);
	connect(ui->height,
			static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this,
			&ScanUi::heightChanged);
	ui->hub->setChecked(tune.isHub());
	connect(ui->hub, &QCheckBox::toggled, this, &ScanUi::hubChanged);
	ui->chip138->setChecked(tune.is138());
	connect(ui->chip138, &QCheckBox::toggled, this, &ScanUi::chip138Changed);

	// data polarity
	ui->dataPolarity->setChecked(tune.isDataPolarityReverse());
	connect(ui->dataPolarity,
			&QCheckBox::toggled,
			this,
			&ScanUi::dataPolarityChanged);
	dataPolarityBtns.addButton(ui->white, RecverCard::Scan::DATA_TYPE_WHITE);
	dataPolarityBtns.addButton(ui->black, RecverCard::Scan::DATA_TYPE_BLACK);
	ui->white->setChecked(tune.dataType == RecverCard::Scan::DATA_TYPE_WHITE);
	ui->black->setChecked(tune.dataType == RecverCard::Scan::DATA_TYPE_BLACK);
	connect(&dataPolarityBtns,
			static_cast<void (QButtonGroup::*)(int id)>(
				&QButtonGroup::buttonClicked),
			this,
			&ScanUi::dataColorChanged);

	// oe polarity
	ui->oePolarity->setChecked(tune.isOePolarityReverse());
	connect(ui->oePolarity, &QCheckBox::toggled, this, &ScanUi::oePolarityChanged);
	oePolarityBtns.addButton(ui->bright, RecverCard::Scan::BRIGHT_8);
	oePolarityBtns.addButton(ui->dark, RecverCard::Scan::BRIGHT_0);
	ui->bright->setChecked(tune.bright == RecverCard::Scan::BRIGHT_8);
	ui->dark->setChecked(tune.bright == RecverCard::Scan::BRIGHT_0);
	connect(&oePolarityBtns,
			static_cast<void (QButtonGroup::*)(int id)>(
				&QButtonGroup::buttonClicked),
			this,  &ScanUi::oeBrightChanged);

	// color
	QStringList colors;
	colors << ScanUi::tr("Red") << ScanUi::tr("Green") << ScanUi::tr("Blue") << ScanUi::tr("Black");
	ui->color1Tag->setChecked(true);
	colorBtns.addButton(ui->color1Tag, RecverCard::Scan::RGB_SEL_TUNE_COLOR1);
	ui->color1->addItems(colors);
	ui->color1->setCurrentIndex(tune.getRbgSel1());
	connect(ui->color1,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this,
		&ScanUi::rgbSelChanged);
	colorBtns.addButton(ui->color2Tag, RecverCard::Scan::RGB_SEL_TUNE_COLOR2);
	ui->color2->addItems(colors);
	ui->color2->setCurrentIndex(tune.getRgbSel2());
	connect(ui->color2,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this,
		&ScanUi::rgbSelChanged);
	colorBtns.addButton(ui->color3Tag, RecverCard::Scan::RGB_SEL_TUNE_COLOR3);
	ui->color3->addItems(colors);
	ui->color3->setCurrentIndex(tune.getRgbSel3());
	connect(ui->color3,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this,
		&ScanUi::rgbSelChanged);
	colorBtns.addButton(ui->color4Tag, RecverCard::Scan::RGB_SEL_TUNE_COLOR4);
	ui->color4->addItems(colors);
	ui->color4->setCurrentIndex(tune.getRgbSel4());
	connect(ui->color4,
		static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this,
		&ScanUi::rgbSelChanged);
	connect(&colorBtns,
			static_cast<void (QButtonGroup::*)(int id)>(
				&QButtonGroup::buttonClicked),
			this,
			&ScanUi::rgbSelTuneChanged);

	// interface lines
	ui->intfLines->setValue(tune.intfLines);
	connect(ui->intfLines,
			static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this,
			&ScanUi::intfLinesChanged);

	// wrap times
	ui->wrapTimes->setValue(tune.wrapTimes);
	connect(ui->wrapTimes,
			static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this,
			&ScanUi::wrapTimesChanged);

	// dotmap
	connect(ui->dotmap, &QTableWidget::cellClicked, this, &ScanUi::clickDot);
	connect(ui->undo, &QPushButton::clicked, this, &ScanUi::undoDot);
	connect(ui->reset, &QPushButton::clicked, this, &ScanUi::resetDotMap);
}

ScanUi::~ScanUi() {
	if (dotsPos)
		delete dotsPos;
	delete ui;
}

void ScanUi::done(int r)
{
	tune.tuneEnd();
	if (QDialog::Accepted == r) {
		tune.tuneFinish(scan);
		card->adjustDotmap(dotsPos);
	}
	QDialog::done(r);
}

void ScanUi::initializePage(int id) {
	pageEnter(id);
}

void ScanUi::cleanupPage(int id) {
	pageLeave(id);
	if (id > 0)
		pageEnter(id - 1);
}

bool ScanUi::validateCurrentPage() {
	pageLeave(currentId());
	return true;
}

void ScanUi::pageEnter(int id) {
	(this->*(enterActions[id]))();
}

void ScanUi::pageLeave(int id) {
}

void ScanUi::setTune() {
	if (card)
		card->setTune(nullptr);
}

void ScanUi::enterModule() {
	tune.tuneModule();
	setTune();
}

void ScanUi::baseColorChanged(int index) {
	tune.setColorMode(index + 1);
	if (tune.isColorMono()) {
		ui->color3->setEnabled(false);
		ui->black->setEnabled(false);
		setTune();
	} else if (tune.isColorFull()) {
		ui->color3->setEnabled(true);
		hubChanged(ui->hub->isChecked());
	}
}

void ScanUi::widthChanged(int value) {
	tune.moduleWidth = value;
}

void ScanUi::heightChanged(int value) {
	tune.moduleHeight = value;
}

void ScanUi::hubChanged(bool checked) {
	ui->color4->setEnabled(!checked);
	tune.setHub(checked);
	setTune();
}

void ScanUi::chip138Changed(bool checked) {
	tune.set138(checked);
	setTune();
}

void ScanUi::enterDataPolarity() {
	tune.tuneDataPolarity();
	tune.dataType = dataPolarityBtns.checkedId();
	setTune();
}

void ScanUi::dataPolarityChanged(bool checked) {
	tune.setDataPolarity(checked);
	setTune();
}

void ScanUi::dataColorChanged(int id) {
	tune.dataType = id;
	setTune();
}

void ScanUi::enterOePolarity() {
	tune.tuneOePolarity();
	tune.bright = oePolarityBtns.checkedId();
	setTune();
}

void ScanUi::oePolarityChanged(bool checked) {
	tune.setOePolarity(checked);
	setTune();
}

void ScanUi::oeBrightChanged(int id) {
	tune.bright = id;
	setTune();
}

void ScanUi::enterRgbSel() {
	tune.tuneRgbSel();
	tune.setRgbSelTune(colorBtns.checkedId());
	setTune();
	tune.setRgbSel(ui->color1->currentIndex(),
				   ui->color2->currentIndex(),
				   ui->color3->currentIndex(),
				   ui->color4->currentIndex());
}

void ScanUi::rgbSelTuneChanged(int id) {
	tune.setRgbSelTune(id);
	setTune();
	tune.setRgbSel(ui->color1->currentIndex(),
				   ui->color2->currentIndex(),
				   ui->color3->currentIndex(),
				   ui->color4->currentIndex());
}

void ScanUi::rgbSelChanged(int index) {
	tune.setRgbSel(ui->color1->currentIndex(),
				   ui->color2->currentIndex(),
				   ui->color3->currentIndex(),
				   ui->color4->currentIndex());
}

void ScanUi::enterIntfLines() {
	tune.tuneIntfLines();
	setTune();
}

void ScanUi::intfLinesChanged(int value) {
	tune.intfLines = value;
	tune.lines = tune.intfLines / tune.wrapTimes;
	setTune();
}

void ScanUi::enterWrapTimes() {
	tune.tuneWrapTimes();
	setTune();
}

void ScanUi::wrapTimesChanged(int value) {
	tune.wrapTimes = value;
	tune.lines = tune.intfLines / tune.wrapTimes;
	setTune();
}

void ScanUi::enterDotmap() {
	ui->undo->setEnabled(false);
	ui->reset->setEnabled(false);
	QTimer::singleShot(10, [this](){
		button(QWizard::FinishButton)->setEnabled(false);
	});

	tune.tuneDotmap();
	setTune();

	size_t dim = tune.moduleWidth * tune.intfLines;
	freenull(dotsPos);
	dotsPos = new RecverCard::DotPos[dim];
	nextDot = 0;

	ui->dotmap->clear();
	QTableWidgetItem *item;

	ui->dotmap->setColumnCount(tune.moduleWidth);
	ui->dotmap->setRowCount(tune.intfLines);
	for (int col = 0; col < tune.moduleWidth; ++col) {
		item = new QTableWidgetItem(QString::number(col + 1));
		ui->dotmap->setHorizontalHeaderItem(col, item);

		for (int row = 0; row < tune.intfLines; ++row) {
			item = new QTableWidgetItem();
			ui->dotmap->setItem(row, col, item);
		}
	}
}

void ScanUi::setDot() {
	int dots = tune.moduleWidth * tune.wrapTimes;

	tune.dotIndex = tune.VALID_LEN_1024 - dots + nextDot % dots;
	tune.currentLine = nextDot / dots;

	setTune();

	ui->undo->setDisabled(nextDot == 0);
	ui->reset->setDisabled(nextDot == 0);
	button(QWizard::FinishButton)->setEnabled(nextDot == tune.moduleWidth * tune.intfLines);
}

void ScanUi::addDot(int row, int col) {
	dotsPos[nextDot].row = row;
	dotsPos[nextDot].col = col;
	ui->dotmap->item(row, col)->setText(QString::number(nextDot + 1));
	++nextDot;
}

void ScanUi::clickDot(int row, int col) {
	if (ui->dotmap->item(row, col)->text() != QString()) return;

	bool newLine = true;
	for (int i = 0; i < ui->dotmap->columnCount(); ++i) {
		if (ui->dotmap->item(row, i)->text() != QString()) {
			newLine = false;
			break;
		}
	}

	if (nextDot <  (tune.moduleWidth + tune.emptyDots) * tune.wrapTimes || !newLine) {
		addDot(row, col);
	} else {
		int rowNext = (row - dotsPos[0].row) % tune.lines;
		for (int i = 0; i < tune.wrapTimes; ++i) {
			for (int j = 0; j < tune.moduleWidth + tune.emptyDots * tune.wrapTimes; ++j) {
				int offset = i * tune.moduleWidth + j;
				addDot(rowNext + dotsPos[offset].row, dotsPos[offset].col);
			}
		}
	}

	setDot();
}

void ScanUi::undoDot(bool checked) {
	--nextDot;
	ui->dotmap->item(dotsPos[nextDot].row, dotsPos[nextDot].col)->setText(QString());
	dotsPos[nextDot].row = 0;
	dotsPos[nextDot].col = 0;
	setDot();
}

void ScanUi::resetDotMap(bool checked) {
	enterDotmap();
}
