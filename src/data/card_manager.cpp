#include "card_manager.h"

#include <vector>
#include <fstream>

#include "io/serial_prot.h"
#include "util/xml_dom.h"
#include "data/data.h"
#include "util/tlv.h"

const std::wstring CardManager::cardsCfgFile{L"/cards.cfg"};

CardManager::CardManager()
{
	SenderCard::cfgMsg();
	RecverCard::cfgMsg();
}

CardManager::~CardManager()
{
	for (auto card: cardsProbing)
		delete card;
	for (auto card: cards)
		delete card;
}

void CardManager::load()
{
	std::wstring file = Data::obj->appAttr->runDir + cardsCfgFile;
	std::ifstream ifs{file, std::ifstream::ate|std::ifstream::binary};
	if (!ifs.is_open() || !ifs.good()) return;
	std::ifstream::streampos size = ifs.tellg();
	ifs.seekg(0);
	char *buf = new char[size];
	ifs.read(buf, size);

	Tlv *tlv = (Tlv *)(buf);
	while ((char*)tlv < buf + size) {
		assert(Card::CFG_CARD_TYPE == tlv->type);
		assert(sizeof(CardType) == tlv->len);

		Card *card = nullptr;
		if (RECVER_CARD == *(CardType *)tlv->value) {
			card = new RecverCard;
		} else if (SENDER_CARD == *(CardType *)tlv->value) {
			card = new SenderCard;
		}
		assert(card);
		cards.push_back(card);
		tlv = tlv->next();
		card->loadCfg(tlv);
	}
}

void CardManager::save()
{
	std::wstring file = Data::obj->appAttr->runDir + cardsCfgFile;
	std::ofstream ofs{file, std::ofstream::binary};

	for (Card* card: cards) {
		card->saveCfg(ofs);
	}
}

void CardManager::probe(PorbeSink sink)
{
	for (auto card: cards)
		card->close();

	probeSink = sink;
	for (auto card: cardsProbing)
		delete card;
	cardsProbing.clear();
	SenderCard::createDummy(cardsProbing);
	RecverCard::createDummy(cardsProbing);

	//RCard::createDummy(cardsProbing);

	for (auto probing: cardsProbing) {
		probing->getAll([this](Card *probing) {
			if (!probing)
				return;
			cardsProbing.remove(probing);
			Card::CardList::iterator old = cards.begin();
			while (cards.end() != old) {
				if (probing->type == (*old)->type
					&& probing->id == (*old)->id) {
					probing->setCfg(*old);
					delete *old;
					old = cards.erase(old);
				} else
					++old;
			}
			cards.push_back(probing);
			probeSink(probing);
		});
	}
}

void CardManager::store()
{
	probeSink = nullptr;
	for (auto card: cardsProbing) {
		card->cancel();
	}
	for (auto card: cards) {
		card->cancel();
	}
}

