#include "sender_card.h"

#include <locale>
#include <codecvt>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace boost;

#include "io/serial_prot.h"
#include "data.h"
#include "util/mem_util.h"

std::map<ByteBufSp, std::wstring> SenderCard::edids;

void SenderCard::cfgMsg()
{
	CardMsg::setTypeWishLen(SENDER_CARD, SenderCardMsg::GET_COL_ADJUST, sizeof(int16_t) * RecverCard::COL_ADJUST_SIZE);
	CardMsg::setTypeWishLen(SENDER_CARD, SenderCardMsg::GET_OUT1_GEOS, sizeof(RecverCard::Geo) * RecverCard::RCVER_CARDS_MAX);
	CardMsg::setTypeWishLen(SENDER_CARD, SenderCardMsg::GET_OUT2_GEOS, sizeof(RecverCard::Geo) * RecverCard::RCVER_CARDS_MAX);
}

void SenderCard::loadEdids()
{
	edids.clear();
	filesystem::path dir(Data::obj->appAttr->runDir + L"/edid");
	try {
		if (!filesystem::exists(dir) || !filesystem::is_directory(dir)) return;
		for (filesystem::directory_iterator itor(dir);
			 itor != filesystem::directory_iterator();
			 ++itor)
		{
			filesystem::path path = itor->path();
			filesystem::ifstream ifs(path, filesystem::ifstream::ate|filesystem::ifstream::binary);
			filesystem::ifstream::streampos size = ifs.tellg();
			ByteBufSp buf = makeByteBufSp(size);
			ifs.seekg(0);
			ifs.read((char *)(buf->buf), size);
			std::wstring stem(path.stem().wstring());
			edids.insert({buf, stem});
		}
	} catch(...) {
	}
}

void SenderCard::createDummy(CardList &list)
{
	std::vector<SerialProt::SerialInfo> infos;

	SerialProt::enumPorts(infos);
	SenderCard *card = nullptr;
	for (size_t i = 0; i < infos.size(); ++i) {
		try {
			card = new SenderCard(infos[i].id);
		} catch (...) {
			continue;
		}
		card->name = infos[i].name;
		list.push_back(card);
	}
}

SenderCard::SenderCard():
	Card(SENDER_CARD),
	protocol{nullptr},
	outPortStub{outPort2},
	getAllImplFunc{nullptr},
	setAllImplFunc{nullptr}
{
}

SenderCard::SenderCard(std::string &id):
	Card(SENDER_CARD, id),
	protocol{new CardProt{SENDER_CARD, id}},
	outPort1{this},
	outPort2{this},
	outPortStub{outPort2},
	getAllImplFunc{std::bind(&SenderCard::getAllImpl, this,
							 std::placeholders::_1,
							 std::placeholders::_2)},
	setAllImplFunc{std::bind(&SenderCard::setAllImpl, this,
							 std::placeholders::_1,
							 std::placeholders::_2)}
{
	outPort1.protocol->outerMsgType = SenderCardMsg::OUTPORT1;
	outPort2.protocol->outerMsgType = SenderCardMsg::OUTPORT2;
}

SenderCard::~SenderCard()
{
	freenull(protocol);
}

void SenderCard::loadCfg(Tlv*& tlv)
{
	Card::loadCfg(tlv);
}

void SenderCard::saveCfg(std::ofstream &ofs)
{
	Card::saveCfg(ofs);
}

void SenderCard::setCfg(Card *cfgCard)
{
	Card::setCfg(cfgCard);
}

void SenderCard::getVersion(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::GET_VERSION, sink);
}

void SenderCard::getModel(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::GET_MODEL, sink);
}

void SenderCard::getColAdjust(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::GET_COL_ADJUST, sink);
}

bool SenderCard::gotColAdjust(size_t len, uint8_t *data)
{
	if (!data || len < sizeof(outPortStub.colAdjust)) return false;
	memcpy(outPortStub.colAdjust, data, sizeof(outPortStub.colAdjust));
	return true;
}

void SenderCard::setColAdjust(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::SET_COL_ADJUST, sizeof(outPortStub.colAdjust),
					 (uint8_t *)outPortStub.colAdjust, sink);
}

bool SenderCard::sotColAdjust(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void SenderCard::getOut1Geos(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::GET_OUT1_GEOS, sink);
}

bool SenderCard::gotOut1Geos(size_t len, uint8_t *data)
{
	if (!data || len < sizeof(outPort1.proxy.geosAdjusted)) return false;
	memcpy(outPort1.proxy.geosAdjusted, data, sizeof(outPort1.proxy.geosAdjusted));
	return true;
}

void SenderCard::setOut1Geos(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::SET_OUT1_GEOS, sizeof(outPort1.proxy.geosAdjusted),
					 (uint8_t *)outPort1.proxy.geosAdjusted, sink);
}

bool SenderCard::sotOut1Geos(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void SenderCard::getOut2Geos(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::GET_OUT2_GEOS, sink);
}

bool SenderCard::gotOut2Geos(size_t len, uint8_t *data)
{
	if (!data || len < sizeof(outPort2.proxy.geosAdjusted)) return false;
	memcpy(outPort2.proxy.geosAdjusted, data, sizeof(outPort2.proxy.geosAdjusted));
	return true;
}

void SenderCard::setOut2Geos(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::SET_OUT2_GEOS, sizeof(outPort2.proxy.geosAdjusted),
					 (uint8_t *)outPort2.proxy.geosAdjusted, sink);
}

bool SenderCard::sotOut2Geos(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void SenderCard::getEdid(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::GET_EDID, sink);
}

bool SenderCard::gotEdid(size_t len, uint8_t *data)
{
	if (!data || !len) return false;
	for (auto pair: edids) {
		if (len == pair.first->size && 0 == memcmp(data, pair.first->buf, len)) {
			edid = pair.first;
			return true;
		}
	}
	edid = makeByteBufSp(len, data);
	return true;
}

void SenderCard::setEdid(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::SET_EDID, edid->size, edid->buf, sink);
}

bool SenderCard::sotEdid(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void SenderCard::getCtrl(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::GET_CTRL, sink);
}

bool SenderCard::gotCtrl(size_t len, uint8_t *data)
{
	if (!data || sizeof(Ctrl) > len) return false;
	memcpy(&ctrl, data, sizeof(Ctrl));
	return true;
}

void SenderCard::setCtrl(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::SET_CTRL, sizeof(ctrl), (uint8_t *)&ctrl, sink);
}

bool SenderCard::sotCtrl(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void SenderCard::setSaveAll(const DataSink &sink)
{
	protocol->rpc(SenderCardMsg::SAVE_ALL, sink);
}

void SenderCard::getAllImpl(size_t len, uint8_t *data)
{
	reenter (getAllCrt) {
		yield getVersion(getAllImplFunc);
		if (!gotVersion(len, data)) return;

		yield getModel(getAllImplFunc);
		if (!gotModel(len, data)) return;

		yield getColAdjust(getAllImplFunc);
		if (!gotColAdjust(len, data)) return;

		yield getOut1Geos(getAllImplFunc);
		if (!gotOut1Geos(len, data)) return;

		yield getOut2Geos(getAllImplFunc);
		if (!gotOut2Geos(len, data)) return;

		yield getEdid(getAllImplFunc);
		if (!gotEdid(len, data)) return;

		yield getCtrl(getAllImplFunc);
		if (!gotCtrl(len, data)) return;

		outPortStub.getAll([this](Card *){
			if (getAllSink) {
				getAllSink(this);
				getAllSink = nullptr;
			}
		});
	}
}

void SenderCard::setAllImpl(size_t len, uint8_t *data)
{
	reenter (setAllCrt) {
		yield setColAdjust(setAllImplFunc);
		if (!sotColAdjust(len, data)) return;

		yield setOut1Geos(setAllImplFunc);
		if (!sotOut1Geos(len, data)) return;

		yield setOut2Geos(setAllImplFunc);
		if (!sotOut2Geos(len, data)) return;

		yield setEdid(setAllImplFunc);
		if (!sotEdid(len, data)) return;

		yield setCtrl(setAllImplFunc);
		if (!sotCtrl(len, data)) return;

		/*yield setSaveAll(setAllImplFunc);
		if (!sotSaveAll(len, data)) return;*/

		outPortStub.setAll([this](Card *){
			if (setAllSink) {
				setAllSink(this);
				setAllSink = nullptr;
			}
		});
	}
}

void SenderCard::cancel()
{
	getAllSink = nullptr;
	setAllSink = nullptr;
	if (protocol) protocol->cancel();
}

void SenderCard::close()
{
	cancel();
	outPort1.close();
	outPort2.close();
	freenull(protocol);
}

std::wstring SenderCard::title()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return std::wstring(converter.from_bytes(id.c_str()));
}

std::wstring SenderCard::tooltip()
{
	return name;
}

bool SenderCard::adjustCol()
{
	RecverCard::Proxy &proxy1 = outPort1.proxy;
	RecverCard::Proxy &proxy2 = outPort2.proxy;
	size_t screnX = screenAttr->geo.x;
	size_t screnY = screenAttr->geo.y;
	int16_t i;
	int16_t maxWidth =0, maxHeight =0;
	int16_t widthAdjust =0;
	int32_t width1 =0, width2 =0;
	int32_t height1 =0, height2 =0;

	memset(outPortStub.colAdjust, 0, sizeof(outPortStub.colAdjust));
	memset(proxy1.geosAdjusted, 0, sizeof(proxy1.geosAdjusted));
	memset(proxy2.geosAdjusted, 0, sizeof(proxy2.geosAdjusted));

	//height
	for(i = 0; i < RecverCard::RCVER_CARDS_MAX; ++i)
	{
		if((proxy1.geos[i].y + proxy1.geos[i].height) > height1)
		{
			height1 = proxy1.geos[i].y + proxy1.geos[i].height;
		}
		if((proxy1.geos[i].x + proxy1.geos[i].width) > width1)
		{
			width1 = proxy1.geos[i].x + proxy1.geos[i].width;
		}

		if((proxy2.geos[i].y + proxy2.geos[i].height) > height2)
		{
			height2 = proxy2.geos[i].y + proxy2.geos[i].height;
		}
		if((proxy2.geos[i].x + proxy2.geos[i].width) > width2)
		{
			width2 = proxy2.geos[i].x + proxy2.geos[i].width;
		}

		if(proxy1.geos[i].width > maxWidth)
		{
			maxWidth = proxy1.geos[i].width;
		}
		if(proxy2.geos[i].width > maxWidth)
		{
			maxWidth = proxy2.geos[i].width;
		}
		if(proxy1.geos[i].height > maxHeight)
		{
			maxHeight = proxy1.geos[i].height;
		}
		if(proxy2.geos[i].height > maxHeight)
		{
			maxHeight = proxy2.geos[i].height;
		}
	}

	//width, height, baseY
	ctrl.width = std::max(width1, width2) + screnX;
	ctrl.height = std::max(height1, height2);
	ctrl.out1BaseY = screnY;
	ctrl.out2BaseY = screnY + height1;

	for(i = 0; i < RecverCard::RCVER_CARDS_MAX; ++i)
	{
		if((proxy1.geos[i].width <= 0) ||(proxy1.geos[i].height <= 0))
			break;

		if(proxy1.geos[i].width != 0)	//20150302 modify
		{
			proxy1.geosAdjusted[i].x = proxy1.geos[i].x + proxy1.geos[i].width - maxWidth + screnX;
			proxy1.geosAdjusted[i].y = proxy1.geos[i].y;

			if(proxy1.geosAdjusted[i].x < widthAdjust)
			{
				widthAdjust = proxy1.geosAdjusted[i].x;
			}

			proxy1.geosAdjusted[i].width = maxWidth;
			proxy1.geosAdjusted[i].height = maxHeight;
		}
	}

	for(i =0; i <RecverCard::RCVER_CARDS_MAX; ++i)
	{
		if((proxy2.geos[i].width <= 0) ||(proxy2.geos[i].height <= 0))
			break;

		if(proxy2.geos[i].width !=0)	//20150302 modify
		{
			proxy2.geosAdjusted[i].x = proxy2.geos[i].x + proxy2.geos[i].width - maxWidth + screnX;
			proxy2.geosAdjusted[i].y = proxy2.geos[i].y;


			if(proxy2.geosAdjusted[i].x < widthAdjust)
			{
				widthAdjust = proxy2.geosAdjusted[i].x;
			}

			proxy2.geosAdjusted[i].width = maxWidth;
			proxy2.geosAdjusted[i].height = maxHeight;
		}
	}

	if(widthAdjust < 0)	//20150302 add
	{
		for(i = 0; i < RecverCard::RCVER_CARDS_MAX; ++i)
		{
			proxy1.geosAdjusted[i].x += abs(widthAdjust);
			proxy2.geosAdjusted[i].x += abs(widthAdjust);
		}
	}

	for(i = 0; i < sizeof(RecverCard::COL_ADJUST_SIZE); ++i)
	{
		outPortStub.colAdjust[i] = widthAdjust;
	}

	return true;
}
