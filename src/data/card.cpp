#include "card.h"

#include <sstream>
#include <locale>
#include <codecvt>
#include <fstream>
//#include <boost/algorithm/string/trim.hpp>

#include "data/data.h"

Card::Card(CardType type) :
	type(type),
	version{0},
	screenAttr(nullptr),
	getAllSink(nullptr),
	getAllCrt{nullptr},
	setAllSink{nullptr},
	setAllCrt{nullptr}
{

}

Card::Card(CardType type, std::string &id) :
	Card{type}
{
	this->id = id;
}

Card::~Card()
{
	if (getAllCrt) {
		delete getAllCrt;
	}
	if (setAllCrt) {
		delete setAllCrt;
	}
}

void Card::loadCfg(Tlv*& tlv)
{
	assert(CFG_ID == tlv->type);
	id.assign((char *)tlv->value, tlv->len);

	tlv = tlv->next();

	assert(CFG_SCREEN == tlv->type);
	setScreen(std::wstring((wchar_t *)tlv->value, tlv->len / sizeof(wchar_t)));

	tlv = tlv->next();
}

void Card::saveCfg(std::ofstream &ofs)
{
	Tlv tlv;

	tlv.type = CFG_CARD_TYPE;
	tlv.len = sizeof(type);
	ofs.write((char *)&tlv, sizeof(tlv));
	ofs.write((char *)&type, tlv.len);

	tlv.type = CFG_ID;
	tlv.len = id.size();
	ofs.write((char *)&tlv, sizeof(tlv));
	ofs.write(id.c_str(), tlv.len);

	tlv.type = CFG_SCREEN;
	tlv.len = screenName.size() * sizeof(wchar_t);
	ofs.write((char *)&tlv, sizeof(tlv));
	ofs.write((char *)screenName.c_str(), tlv.len);
}

void Card::setCfg(Card *cfgCard)
{
	if (type != cfgCard->type) return;
	id = cfgCard->id;
	setScreen(cfgCard->screenName);
}

std::wstring Card::title()
{
	return name;
}

std::wstring Card::description()
{
	std::wstringstream description;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	description << converter.from_bytes(model) << "-"
				<< std::to_wstring((uint8_t)(version>>8)) << "."
				<< std::to_wstring((uint8_t)(version&0x00FF));

	return description.str();
}

void Card::setScreen(std::wstring &screenName)
{
	this->screenName = screenName;
	screenAttr = Data::obj->projectAttr->getScreen(screenName);
}

bool Card::gotVersion(size_t len, uint8_t *data)
{
	if (!data || sizeof(version) != len) return false;
	version = ntoh2(*(uint16_t *)data);
	return true;
}

bool Card::gotModel(size_t len, uint8_t *data)
{
	if (!data || !len) return false;
	model = std::string((char *)data);
	return true;
	//algorithm::trim(model);
}

bool Card::sotSaveAll(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void Card::getAll(const CardSink &sink)
{
	getAllSink = sink;
	if (getAllCrt) {
		delete getAllCrt;
	}
	getAllCrt = new asio::coroutine;
	getAllImpl(0, nullptr);
}

void Card::setAll(const CardSink &sink)
{
	setAllSink = sink;
	if (setAllCrt) {
		delete setAllCrt;
	}
	setAllCrt = new asio::coroutine;
	setAllImpl(0, nullptr);
}

bool Card::isOk(size_t len, uint8_t *data)
{
	return (data && len == DATA_LEN_1
			&& *(uint8_t *)data == DATA_OK);
}
