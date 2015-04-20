#ifndef CARD_H
#define CARD_H

#include <string>
#include <list>
#include <functional>

#include "io/asio_prot.h"
#include "screen_attr.h"
#include "io/card_prot.h"
#include "util/tlv.h"

class ScreenAttr;

class Card
{
public:
#pragma pack(push)
#pragma pack(1)
struct Led {
	Led(){}

	char flag1{'S'};
	char flag2{'E'};
	char flag3{'F'};
	uint8_t version{1};
	uint32_t len{sizeof(Led)};
	uint16_t reserved{0};
	int16_t boxWidth{256};
	int16_t boxHeight{128};
};
#pragma pack(pop)

	enum {
		CFG_CARD_TYPE,
		CFG_ID,
		CFG_SCREEN,
		CFG_CARD_MAX,
	};
	typedef std::list<Card *> CardList;
	typedef std::function<void (Card *)> CardSink;

	Card(CardType type);
	Card(CardType type, std::string &id);
	virtual ~Card();

	virtual void loadCfg(Tlv*& tlv);
	virtual void saveCfg(std::ofstream &ofs);
	virtual void setCfg(Card *cfgCard);

	virtual std::wstring title() = 0;
	virtual std::wstring tooltip() = 0;
	std::wstring description();
	void setScreen(std::wstring &screenName);

	bool gotVersion(size_t len, uint8_t *data);
	bool gotModel(size_t len, uint8_t *data);
	virtual void setSaveAll(const DataSink &sink) = 0;
	bool sotSaveAll(size_t len, uint8_t *data);

	void getAll(const CardSink &sink);
	virtual void getAllImpl(size_t len, uint8_t *data) = 0;
	void setAll(const CardSink &sink);
	virtual void setAllImpl(size_t len, uint8_t *data) = 0;
	virtual void cancel() = 0;
	virtual void close() = 0;

	virtual bool adjustCol() = 0;

	CardType type;
	std::string id;
	std::wstring name;
	std::wstring detail;
	uint16_t version;
	std::string model;
	std::wstring screenName;
	ScreenAttr *screenAttr;

	Led led;
protected:
	CardSink getAllSink;
	asio::coroutine *getAllCrt;
	CardSink setAllSink;
	asio::coroutine *setAllCrt;

	bool isOk(size_t len, uint8_t *data);
};

#endif // CARD_H
