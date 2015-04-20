#include "data.h"

#include <cassert>

Data *Data::obj = nullptr;

Data::Data()
{
	assert(!obj);
	obj = this;

	appAttr = new AppAttr;
	projectAttr = new ProjectAttr;
	cards = new CardManager;
}

Data::~Data()
{
	free();

	obj = nullptr;
}

void Data::load()
{
	appAttr->load();
	projectAttr->load();
	cards->load();
}

void Data::save()
{
	projectAttr->save(nullptr, nullptr);
	appAttr->save();
	cards->save();
}

void Data::free()
{
	if (projectAttr) {
		delete projectAttr;
		projectAttr = nullptr;
	}
	if (appAttr) {
		delete appAttr;
		appAttr = nullptr;
	}
	if (cards) {
		delete cards;
		cards = nullptr;
	}
}
