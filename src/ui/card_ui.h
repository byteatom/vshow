#ifndef CARDUI_H
#define CARDUI_H

#include <QDialog>
#include <QListWidgetItem>

#include "data/card_manager.h"

class ProjectAttr;

namespace Ui {
class CardUi;
}

class SenderCard;
class RecverCard;

class CardUi : public QDialog
{
	Q_OBJECT

public:
	explicit CardUi(QWidget *parent);
	virtual ~CardUi();

	virtual void done(int r) override;

private:
	void newCard(Card *card);
	void currentCardChanged(QListWidgetItem *current, QListWidgetItem *previous);
	Card* currentCard();
	SenderCard* currentSenderCard();
	RecverCard* currentRecverCard();

	void screenChanged(const QString &text);
	void scanChanged(const QString &text);
	void editScan(bool checked = false);
	void addScan(bool checked = false);
	void delScan(bool checked = false);
	void freqChanged(int index);
	void grayChanged(const QString &text);
	void blankChanged(int value);
	void refreshRateChanged(int value);
	void gammaChanged(double value);
	void fadeChanged(bool checked);
	void keepChanged(bool checked);
	void edidChanged(const QString &text);
	void redChanged(int value);
	void greenChanged(int value);
	void blueChanged(int value);
	void editLink1(bool checked = false);
	void editLink2(bool checked = false);
	void read(bool checked = false);
	void send(bool checked = false);
	void save(bool checked = false);

	CardManager *cards;
	ProjectAttr *projectAttr;

private:
	void setIoUi(bool enable);
	void adjustFreq(RecverCard *recver);

	Ui::CardUi *ui;
};

class Card;

class CardItem : public QListWidgetItem
{
public:
	explicit CardItem(QListWidget *view, Card *card);
	virtual ~CardItem();

	Card *card;
};

#endif // CARD_UI_H
