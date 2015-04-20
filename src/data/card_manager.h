#ifndef CARDLIST_H
#define CARDLIST_H

#include <list>
#include <functional>
#include <unordered_map>

#include "recver_card.h"
#include "sender_card.h"

class CardManager
{
public:
	typedef std::function<void (Card *)> PorbeSink;
	static const std::wstring cardsCfgFile;

	CardManager();
	~CardManager();

	void load();
	void save();
	void probe(PorbeSink probeSink);
	void store();

private:
	Card::CardList cardsProbing;
	Card::CardList cards;
	PorbeSink probeSink;
};

#endif // CARDLIST_H
