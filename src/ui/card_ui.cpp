#include "card_ui.h"
#include "ui_card_ui.h"

#include <cmath>
#include <cstdio>

#include <QListWidgetItem>
#include <QFileDialog>
#include <QMessageBox>

#include "data/data.h"
#include "data/sender_card.h"
#include "scan_ui.h"
#include "link_ui.h"

CardUi::CardUi(QWidget *parent) :
	QDialog(parent),
	cards(Data::obj->cards),
	projectAttr{Data::obj->projectAttr},
	ui(new Ui::CardUi)
{
	ui->setupUi(this);

	setFixedSize(size());

	ui->cards->setEnabled(false);
	ui->attr->setEnabled(false);
	setIoUi(false);

	connect(ui->cards, &QListWidget::currentItemChanged,
			this, &CardUi::currentCardChanged);

	//screen
	for (auto screen: projectAttr->organs) {
		ui->screen->addItem(QString::fromStdWString(
								static_cast<ScreenAttr *>(screen)->name));
	}
	ui->screen->setCurrentIndex(-1);
	connect(ui->screen, static_cast<void (QComboBox::*)(const QString &text)>(&QComboBox::currentIndexChanged),
			this, &CardUi::screenChanged);

	//edid
	SenderCard::loadEdids();
	for (auto edid: SenderCard::edids) {
		ui->edid->addItem(QString::fromStdWString(edid.second));
	}
	ui->edid->setCurrentIndex(-1);
	connect(ui->edid, static_cast<void (QComboBox::*)(const QString &text)>(&QComboBox::currentTextChanged),
			this, &CardUi::edidChanged);

	//color
	connect(ui->red, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),
			this, &CardUi::redChanged);
	connect(ui->green, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),
			this, &CardUi::greenChanged);
	connect(ui->blue, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),
			this, &CardUi::blueChanged);

	//link
	connect(ui->link1, &QPushButton::clicked, this, &CardUi::editLink1);
	connect(ui->link2, &QPushButton::clicked, this, &CardUi::editLink2);

	//scan
	RecverCard::loadScans();
	for (auto scan: RecverCard::scans) {
		ui->scan->addItem(QString::fromStdWString(scan.first));
	}
	ui->scan->setCurrentIndex(-1);
	connect(ui->scan, static_cast<void (QComboBox::*)(const QString &text)>(&QComboBox::currentTextChanged),
			this, &CardUi::scanChanged);
	connect(ui->editScan, &QPushButton::clicked, this, &CardUi::editScan);
	connect(ui->addScan, &QPushButton::clicked, this, &CardUi::addScan);
	connect(ui->delScan, &QPushButton::clicked, this, &CardUi::delScan);

	//frequency
	double base = 750/4;
	for (int i = 0; i < 40; ++i) {
		double freq = base / (5 + i);
		ui->freq->addItem(QString("%1 MHz").arg(freq, 0, 'f', 1));
	}
	ui->freq->setCurrentIndex(-1);
	connect(ui->freq, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
			this, &CardUi::freqChanged);

	//gray
	for (int i = 8; i <= 65536; i*=2) {
		ui->gray->addItem(QString::number(i));
	}
	ui->gray->setCurrentIndex(-1);
	connect(ui->gray, static_cast<void (QComboBox::*)(const QString &text)>(&QComboBox::currentTextChanged),
			this, &CardUi::grayChanged);

	//blank
	connect(ui->blank, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &CardUi::blankChanged);

	connect(ui->refresh, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &CardUi::refreshRateChanged);

	//gamma
	connect(ui->gamma, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
			this, &CardUi::gammaChanged);

	//fade, keep
	connect(ui->fade, &QCheckBox::toggled, this, &CardUi::fadeChanged);
	connect(ui->keep, &QCheckBox::toggled, this, &CardUi::keepChanged);

	//button
	connect(ui->read, &QPushButton::clicked, this, &CardUi::read);
	connect(ui->send, &QPushButton::clicked, this, &CardUi::send);
	connect(ui->save, &QPushButton::clicked, this, &CardUi::save);
	connect(ui->close, &QPushButton::clicked, this, &CardUi::accept);

	cards->probe(std::bind(&CardUi::newCard, this, std::placeholders::_1));
}

CardUi::~CardUi()
{
	delete ui;
}

void CardUi::done(int r)
{
	cards->store();
	QDialog::done(r);
}

void CardUi::newCard(Card *card)
{
	setIoUi(true);
	CardItem *item = new CardItem(ui->cards, card);
	item->setText(QString::fromStdWString(card->title()));
	item->setToolTip(QString::fromStdWString(card->tooltip()));
	if (ui->cards->count() == 1) {
		ui->cards->setEnabled(true);
		ui->attr->setEnabled(true);
		ui->cards->setCurrentItem(item);
	}
}

void CardUi::currentCardChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	Card *card = static_cast<CardItem *>(current)->card;
	ui->detail->setText(QString::fromStdWString(card->description()));

	ui->screen->blockSignals(true);
	ui->freq->blockSignals(true);
	ui->gray->blockSignals(true);
	ui->blank->blockSignals(true);
	ui->refresh->blockSignals(true);
	ui->gamma->blockSignals(true);
	ui->fade->blockSignals(true);
	ui->keep->blockSignals(true);
	ui->edid->blockSignals(true);
	ui->red->blockSignals(true);
	ui->green->blockSignals(true);
	ui->blue->blockSignals(true);

	ui->screen->setCurrentIndex(ui->screen->findText(QString::fromStdWString(card->screenName)));
	RecverCard *recver = nullptr;
	if (SENDER_CARD == card->type) {
		SenderCard *sender = static_cast<SenderCard *>(card);
		ui->sendCard->show();

		std::wstring edidStem = sender->edids[sender->edid];
		if (edidStem.empty())
			ui->edid->setCurrentIndex(-1);
		else
			ui->edid->setCurrentText(QString::fromStdWString(edidStem));

		ui->red->setValue(sender->ctrl.red);
		ui->green->setValue(sender->ctrl.green);
		ui->blue->setValue(sender->ctrl.blue);
		ui->link1Tag->setText(tr("Port 1 link:"));
		ui->link1->setIcon(QIcon(QString(":/image/link%1.png").arg(sender->outPort1.proxy.style)));
		ui->link2Tag->show();
		ui->link2->show();
		ui->link2->setIcon(QIcon(QString(":/image/link%1.png").arg(sender->outPort2.proxy.style)));
		recver = &sender->outPortStub;
	} else if (RECVER_CARD == card->type) {
		recver = static_cast<RecverCard *>(card);
		ui->link1Tag->setText(tr("Link:"));
		ui->link1->setIcon(QIcon(QString(":/image/link%1.png").arg(recver->proxy.style)));
		ui->link2Tag->hide();
		ui->link2->hide();
		ui->sendCard->hide();
	}
	ui->freq->setCurrentIndex(recver->scan.freq);
	ui->gray->setCurrentText(QString::number(pow(2, recver->scan.gray)));
	ui->blank->setValue(recver->scan.blank);
	ui->refresh->setValue(recver->refreshRate);
	ui->gamma->setValue(double(recver->scan.gamma) / 10);
	ui->fade->setChecked(recver->scan.bmask & RecverCard::Scan::BMASK_FADE_IN);
	ui->keep->setChecked(recver->scan.bmask & RecverCard::Scan::BMASK_KEEP_LAST);

	ui->screen->blockSignals(false);
	ui->freq->blockSignals(false);
	ui->gray->blockSignals(false);
	ui->blank->blockSignals(false);
	ui->refresh->blockSignals(false);
	ui->gamma->blockSignals(false);
	ui->fade->blockSignals(false);
	ui->keep->blockSignals(false);
	ui->edid->blockSignals(false);
	ui->red->blockSignals(false);
	ui->green->blockSignals(false);
	ui->blue->blockSignals(false);
}

Card* CardUi::currentCard()
{
	return static_cast<CardItem *>(ui->cards->currentItem())->card;
}

SenderCard* CardUi::currentSenderCard()
{
	QListWidgetItem *item = ui->cards->currentItem();
	if (!item) return nullptr;
	Card *card = static_cast<CardItem *>(item)->card;
	if (card->type == SENDER_CARD)
		return static_cast<SenderCard *>(card);
	else
		return nullptr;
}

RecverCard* CardUi::currentRecverCard()
{
	QListWidgetItem *item = ui->cards->currentItem();
	if (!item) return nullptr;
	Card *card = static_cast<CardItem *>(item)->card;
	if (RECVER_CARD == card->type)
		return static_cast<RecverCard *>(card);
	else
		return &(static_cast<SenderCard *>(card)->outPortStub);
}

void CardUi::screenChanged(const QString &text)
{
	Card *card = currentCard();
	assert(card);
	if (text.toStdWString() == card->screenName) return;

	card->setScreen(text.toStdWString());

	if (RECVER_CARD == card->type) {
		RecverCard *recver = static_cast<RecverCard *>(card);
		recver->adjustCol();
		adjustFreq(recver);
		recver->startCast();
	}
}

void CardUi::scanChanged(const QString &text)
{
	RecverCard *recver = currentRecverCard();
	assert(recver);
	recver->scanStem = text.toStdWString();
	std::wstring file = QCoreApplication::applicationDirPath().toStdWString()
			+ RecverCard::scanDir
			+ ui->scan->currentText().toStdWString()
			+ RecverCard::scanExt;
	recver->loadScan(file);
	adjustFreq(recver);
}

void CardUi::editScan(bool checked)
{
	RecverCard *recver = currentRecverCard();
	assert(recver);
	ScanUi scanUi(this, recver);
	if (QDialog::Accepted == scanUi.exec()) {
		recver->setTuneOk(nullptr);
		std::wstring file = QCoreApplication::applicationDirPath().toStdWString()
				+ RecverCard::scanDir
				+ ui->scan->currentText().toStdWString()
				+ RecverCard::scanExt;
		recver->saveScan(file);
		adjustFreq(recver);
	} else
		recver->setTune(nullptr);
}

void CardUi::addScan(bool checked)
{
	RecverCard *recver = currentRecverCard();
	assert(recver);
	ScanUi scanUi(this, recver);
	if (QDialog::Accepted == scanUi.exec()) {
		recver->setTuneOk(nullptr);
		QString file = QFileDialog::getSaveFileName(
							this,
							tr("Save Scan"),
							QCoreApplication::applicationDirPath()
					+ QString::fromStdWString(recver->scanDir),
							QString(tr("Scan File (*%1)")).arg(QString::fromStdWString(recver->scanExt)));
		if (file.isEmpty()) return;
		recver->saveScan(file.toStdWString());
		adjustFreq(recver);
		QString stem{QFileInfo(file).baseName()};
		ui->scan->blockSignals(true);
		ui->scan->addItem(stem);
		ui->scan->setCurrentText(stem);
		ui->scan->blockSignals(false);
	} else
		recver->setTune(nullptr);
}

void CardUi::delScan(bool checked)
{
	QString file = QCoreApplication::applicationDirPath()
			+ QString::fromStdWString(RecverCard::scanDir)
			+ ui->scan->currentText()
			+ QString::fromStdWString(RecverCard::scanExt);
	QFile::remove(file);
	ui->scan->blockSignals(true);
	ui->scan->removeItem(ui->scan->currentIndex());
	ui->scan->setCurrentIndex(-1);
	ui->scan->blockSignals(false);
}


void CardUi::freqChanged(int index)
{
	RecverCard *recver = currentRecverCard();
	assert(recver);
	recver->scan.freq = index;
	adjustFreq(recver);
}

void CardUi::grayChanged(const QString &text)
{
	RecverCard *recver = currentRecverCard();
	assert(recver);
	recver->scan.gray = log2(text.toUShort());
	adjustFreq(recver);
	recver->adjustGamma();
}

void CardUi::blankChanged(int value)
{
	RecverCard *recver = currentRecverCard();
	assert(recver);
	recver->scan.blank = value;
	adjustFreq(recver);
}

void CardUi::refreshRateChanged(int value)
{
	RecverCard *recver = currentRecverCard();
	assert(recver);
	recver->refreshRate = value;
	adjustFreq(recver);
}

void CardUi::gammaChanged(double value)
{
	RecverCard *recver = currentRecverCard();
	assert(recver);
	recver->scan.gamma = value * 10;
	adjustFreq(recver);
	recver->adjustGamma();
}

void CardUi::fadeChanged(bool checked)
{
	RecverCard *card = currentRecverCard();
	assert(card);
	if (checked)
		card->scan.bmask |= RecverCard::Scan::BMASK_FADE_IN;
	else
		card->scan.bmask &= (~RecverCard::Scan::BMASK_FADE_IN);
}

void CardUi::keepChanged(bool checked)
{
	RecverCard *card = currentRecverCard();
	assert(card);
	if (checked)
		card->scan.bmask |= RecverCard::Scan::BMASK_KEEP_LAST;
	else
		card->scan.bmask &= (~RecverCard::Scan::BMASK_KEEP_LAST);
}

void CardUi::edidChanged(const QString &text)
{
	SenderCard *card = currentSenderCard();
	assert(card);
	std::wstring stem = text.toStdWString();
	for (auto pair: SenderCard::edids) {
		if (stem == pair.second) {
			card->edid = pair.first;
			break;
		}
	}
}

void CardUi::redChanged(int value)
{
	SenderCard *card = currentSenderCard();
	assert(card);
	card->ctrl.red = value;
}

void CardUi::greenChanged(int value)
{
	SenderCard *card = currentSenderCard();
	assert(card);
	card->ctrl.green = value;
}

void CardUi::blueChanged(int value)
{
	SenderCard *card = currentSenderCard();
	assert(card);
	card->ctrl.blue = value;
}

void CardUi::editLink1(bool checked)
{
	Card *card = currentCard();
	assert(card);

	if (!card->screenAttr) {
		QMessageBox box{this};
		box.setIcon(QMessageBox::Information);
		box.setText(tr("Please select source screen first."));
		box.exec();
		return;
	}

	RecverCard *recver = nullptr;
	if (RECVER_CARD == card->type) {
		recver = static_cast<RecverCard *>(card);
		assert(recver);
	} else {
		recver = &(static_cast<SenderCard *>(card)->outPort1);
	}

	LinkUi linkUi(this, recver->proxy);
	if (QDialog::Accepted != linkUi.exec()) return;
	ui->link1->setIcon(QIcon(QString(":/image/link%1.png").arg(recver->proxy.style)));
	card->adjustCol();
}

void CardUi::editLink2(bool checked)
{
	SenderCard *card = currentSenderCard();
	assert(card);

	if (!card->screenAttr) {
		QMessageBox msgBox{this};
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setText(tr("Please select source screen first."));
		msgBox.exec();
		return;
	}

	LinkUi linkUi(this, card->outPort2.proxy);
	if (QDialog::Accepted != linkUi.exec()) return;
	ui->link2->setIcon(QIcon(QString(":/image/link%1.png").arg(card->outPort2.proxy.style)));
	card->adjustCol();
}

void CardUi::read(bool checked)
{
	setIoUi(false);
	currentCard()->getAll([this](Card *card) {
		currentCardChanged(ui->cards->currentItem(), nullptr);
		setIoUi(true);
	});
}

void CardUi::send(bool checked)
{
	setIoUi(false);
	currentCard()->setAll([this](Card *card) {
		setIoUi(true);
	});
}

void CardUi::save(bool checked)
{
	setIoUi(false);
	currentCard()->setSaveAll([this](size_t len, uint8_t *data) {
		setIoUi(true);
	});
}

void CardUi::setIoUi(bool enable)
{
	ui->read->setEnabled(enable);
	ui->send->setEnabled(enable);
	ui->save->setEnabled(enable);
	ui->addScan->setEnabled(enable);
	ui->editScan->setEnabled(enable);
}

void CardUi::adjustFreq(RecverCard *recver)
{
	recver->adjustFreq();
	currentCardChanged(ui->cards->currentItem(), nullptr);
}

CardItem::CardItem(QListWidget *view, Card *card) :
	QListWidgetItem(view),
	card(card)
{
}

CardItem::~CardItem()
{
}
