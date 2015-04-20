#include "recver_card.h"

#include <cassert>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace boost;

#include "sender_card.h"
#include "data.h"
#include "io/eth_caster.h"
#include "util/mem_util.h"

RecverCard::Scan::Scan()
{
	for (int i = 0; i < sizeof(lineAdjust); ++i) lineAdjust[i] = i;
}

void RecverCard::Scan::tuneInit(Scan &scan)
{
	display = DISPLAY_3A;
	dataMode = DATA_MODE_5A;
	blank = 0;
	brightMax = BRIGHT_15;
	bright = BRIGHT_8;
	if (BASE_COLOR_FULL == baseColor || BASE_COLOR_6 == baseColor)
		freq = FREQ_8;
	else
		freq = FREQ_12;
	dataType = DATA_TYPE_WHITE;

	bmask = scan.bmask & (~BMASK_PAGE);
	rgbSel = scan.rgbSel;
	intfLines = scan.intfLines;
	wrapTimes = scan.wrapTimes;
	lines = scan.lines;
	gamma = scan.gamma;
	baseColor = scan.baseColor;
	emptyDots = scan.emptyDots;
	moduleWidth = scan.moduleWidth;
	moduleHeight = scan.moduleHeight;
}

void RecverCard::Scan::tuneModule()
{
	display = DISPLAY_A5;
	dataMode = DATA_MODE_0;
}

void RecverCard::Scan::tuneDataPolarity() {
	dataMode = DATA_MODE_5A;
	bright = BRIGHT_0;
}

void RecverCard::Scan::tuneOePolarity() {
	dataType = DATA_TYPE_WHITE;
}

void RecverCard::Scan::tuneRgbSel() {
	bright = BRIGHT_8;
	dataType = DATA_TYPE_COLOR_VEC;
}

void RecverCard::Scan::tuneIntfLines() {
	display = DISPLAY_A5;
	dataType = DATA_TYPE_LINES;
}

void RecverCard::Scan::tuneWrapTimes() {
	display = DISPLAY_3A;
	dataType = DATA_TYPE_LINE;
}

void RecverCard::Scan::tuneDotmap() {
	dataType = DATA_TYPE_DOT_VEC;
	dotIndex = VALID_LEN_1024 - moduleWidth * wrapTimes;
	currentLine = 0;
}

void RecverCard::Scan::tuneEnd()
{
	display = DISPLAY_A5;
	dataMode = 0;
}

void RecverCard::Scan::tuneFinish(Scan &scan)
{
	scan.bmask = bmask;
	scan.rgbSel = rgbSel;
	scan.intfLines = intfLines;
	scan.wrapTimes = wrapTimes;
	scan.lines = lines;
	scan.baseColor = baseColor;
	scan.emptyDots = emptyDots;
	scan.moduleWidth = moduleWidth;
	scan.moduleHeight = moduleHeight;
}

int RecverCard::Scan::getColorMode()
{
	return (baseColor & BASE_COLOR_MODE_MASK);
}

void RecverCard::Scan::setColorMode(int mode)
{
	baseColor |= BASE_COLOR_MODE_MASK;
	baseColor &= (mode & RecverCard::Scan::BASE_COLOR_MODE_MASK);
}

bool RecverCard::Scan::isColorMono()
{
	return ((baseColor & RecverCard::Scan::BASE_COLOR_MODE_MASK)
				== RecverCard::Scan::BASE_COLOR_MONO);
}

bool RecverCard::Scan::isColorFull()
{
	return ((baseColor & RecverCard::Scan::BASE_COLOR_MODE_MASK)
				== RecverCard::Scan::BASE_COLOR_FULL);
}

void RecverCard::Scan::setHub(bool hub)
{
	if (hub) {
		baseColor |= BASE_COLOR_HUB_MASK;
	} else {
		baseColor &= (~BASE_COLOR_HUB_MASK);
	}
}

bool RecverCard::Scan::isHub()
{
	return (baseColor & BASE_COLOR_HUB_MASK);
}

void RecverCard::Scan::set138(bool is138)
{
	if (is138) {
		bmask |= BMASK_LINE_MODE;
	} else {
		bmask &= (~BMASK_LINE_MODE);
	}
}

bool RecverCard::Scan::is138()
{
	return (bmask & BMASK_LINE_MODE);
}

void RecverCard::Scan::setDataPolarity(bool reverse)
{
	if (reverse) {
		bmask |= BMASK_DATA_POLARITY;
	} else {
		bmask &= (~BMASK_DATA_POLARITY);
	}
}

bool RecverCard::Scan::isDataPolarityReverse()
{
	return (bmask & BMASK_DATA_POLARITY);
}

void RecverCard::Scan::setOePolarity(bool reverse)
{
	if (reverse) {
		bmask |= BMASK_OE_POLARITY;
	} else {
		bmask &= (~BMASK_OE_POLARITY);
	}
}

bool RecverCard::Scan::isOePolarityReverse()
{
	return (bmask & BMASK_OE_POLARITY);
}

int RecverCard::Scan::getRbgSel1()
{
	return (rgbSel & 0x03) >> 0;
}

int RecverCard::Scan::getRgbSel2()
{
	return (rgbSel & 0x0c) >> 2;
}

int RecverCard::Scan::getRgbSel3()
{
	return (rgbSel & 0x30) >> 4;
}

int RecverCard::Scan::getRgbSel4()
{
	return (rgbSel & 0xc0) >> 6;
}

void RecverCard::Scan::setRgbSel(int color1, int color2, int color3, int color4)
{
	rgbSel = ((color1 & 0x03) << 0) | ((color2 & 0x03) << 2)
			| ((color3 & 0x03) << 4) | ((color4 & 0x03) << 6);
}

void RecverCard::Scan::setRgbSelTune(int color)
{
	rgbSel = color;
	if (isDataPolarityReverse())
		rgbSel = ~rgbSel;
}

void RecverCard::cfgMsg()
{
	CardMsg::setTypeWishLen(RECVER_CARD, RecverCardMsg::GET_DOTMAP, sizeof(int16_t) * DOTMAP_SIZE);
	CardMsg::setTypeWishLen(RECVER_CARD, RecverCardMsg::GET_PAGE_BRIGHT, sizeof(PageBright) * Scan::PAGE_TIMES_MAX * Scan::PAGE_PAGES_MAX);
}

const std::wstring RecverCard::scanDir{L"/scan/"};
const std::wstring RecverCard::scanExt{L".scn"};
std::map<std::wstring, ByteBufSp> RecverCard::scans;

void RecverCard::loadScans()
{
	scans.clear();
	filesystem::path dir(Data::obj->appAttr->runDir + scanDir);
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
			std::wstring file(path.stem().wstring());
			scans.insert({file, buf});
		}
	} catch(...) {
	}
}

void RecverCard::loadScan(std::wstring &file)
{
	std::ifstream ifs{file, std::ifstream::binary};
	ifs.read((char *)&led, sizeof(led));
	ifs.read((char *)&scan, sizeof(scan));
	ifs.read((char *)dotmap, sizeof(dotmap));
}

void RecverCard::saveScan(std::wstring &file)
{
	std::ofstream ofs{file, std::ofstream::binary};
	ofs.write((char *)&led, sizeof(led));
	ofs.write((char *)&scan, sizeof(scan));
	ofs.write((char *)dotmap, sizeof(dotmap));
}

void RecverCard::createDummy(CardList &list)
{
	std::vector<EthProt::NicInfo> nics;

	EthProt::enumNics(nics);
	for (auto nic: nics) {
		RecverCard *card = new RecverCard(nic.id, nic.addr);
		card->name = nic.name;
		card->detail = nic.detail;
		list.push_back(card);
	}
}

RecverCard::RecverCard() :
	Card{RECVER_CARD},
	protocol{nullptr},
	caster{new EthCaster}
{
	init();
}

RecverCard::RecverCard(SenderCard *owner) :
	Card{RECVER_CARD, owner->id},
	protocol(owner->protocol ? new CardProt{RECVER_CARD, owner->protocol} : nullptr),
	caster{nullptr}
{
	init();
}

RecverCard::RecverCard(std::string &id, EthMsg::Addr &addr) :
	Card{RECVER_CARD, id},
	protocol{new CardProt{RECVER_CARD, new EthProt{id, addr}}},
	caster{new EthCaster}
{
	init();
}

RecverCard::~RecverCard()
{
	freenull(caster);
	freenull(protocol);
}

void RecverCard::init() {
	memset(dotmap, 0, sizeof(dotmap));
	memset(gamma, 0, sizeof(gamma));
	memset(colAdjust, 0, sizeof(colAdjust));
	memset(pageBright, 0xff, sizeof(pageBright));
	setTuneOkCrt = nullptr;
	getAllImplFunc = std::bind(&RecverCard::getAllImpl, this,
							 std::placeholders::_1,
							 std::placeholders::_2);
	setAllImplFunc = std::bind(&RecverCard::setAllImpl, this,
							 std::placeholders::_1,
							 std::placeholders::_2);
}

void RecverCard::loadCfg(Tlv*& tlv)
{
	Card::loadCfg(tlv);

	assert(CFG_PROXY == tlv->type);
	assert(sizeof(proxy) == tlv->len);
	memcpy(&proxy, tlv->value, tlv->len);

	tlv = tlv->next();

	startCast();
}

void RecverCard::saveCfg(std::ofstream &ofs)
{
	Card::saveCfg(ofs);

	Tlv tlv;

	tlv.type = CFG_PROXY;
	tlv.len = sizeof(proxy);
	ofs.write((char *)&tlv, sizeof(tlv));
	ofs.write((char *)&proxy, tlv.len);
}

void RecverCard::setCfg(Card *cfgCard)
{
	Card::setCfg(cfgCard);

	if (type != cfgCard->type) return;
	RecverCard *cfgRecverCard = static_cast<RecverCard *>(cfgCard);
	memcpy(&proxy, &cfgRecverCard->proxy, sizeof(proxy));

	startCast();
}

void RecverCard::getVersion(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::GET_VERSION, sink);
}

void RecverCard::getModel(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::GET_MODEL, sink);
}

void RecverCard::getScan(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::GET_SCAN, sink);
}

bool RecverCard::gotScan(size_t len, uint8_t *data)
{
	if (!data || len < sizeof(Scan)) return false;
	memcpy(&scan, data, sizeof(Scan) - 1);  //exclude moduleHeight
	return true;
}

void RecverCard::setScan(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::SET_SCAN, sizeof(scan) - 1, (uint8_t *)&scan, sink);
}

bool RecverCard::sotScan(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void RecverCard::getDotmap(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::GET_DOTMAP, sink);
}

bool RecverCard::gotDotmap(size_t len, uint8_t *data)
{
	if (!data || len < sizeof(dotmap)) return false;
	memcpy(dotmap, data, sizeof(dotmap));
	return true;
}

void RecverCard::setDotmap(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::SET_DOTMAP, sizeof(dotmap),
					 (uint8_t *)dotmap, sink);
}

bool RecverCard::sotDotmap(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void RecverCard::getGamma(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::GET_GAMMA, sink);
}

bool RecverCard::gotGamma(size_t len, uint8_t *data)
{
	if (!data || len < sizeof(gamma)) return false;
	memcpy(gamma, data, sizeof(gamma));
	return true;
}

void RecverCard::setGamma(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::SET_GAMMA, sizeof(gamma),
					 (uint8_t *)gamma, sink);
}

bool RecverCard::sotGamma(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void RecverCard::getPageBright(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::GET_PAGE_BRIGHT, sink);
}

bool RecverCard::gotPageBright(size_t len, uint8_t *data)
{
	if (!data || len < sizeof(pageBright)) return false;
	memcpy(pageBright, data, sizeof(pageBright));
	return true;
}

void RecverCard::setPageBright(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::SET_PAGE_BRIGHT, sizeof(pageBright),
					 (uint8_t *)pageBright, sink);
}

bool RecverCard::sotPageBright(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void RecverCard::setTune(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::SET_TUNE, sizeof(tune) - 1, (uint8_t *)&tune, sink);
}

bool RecverCard::sotTune(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void RecverCard::setBright(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::SET_BRIGHT, sizeof(scan.bright),
					 &scan.bright, sink);
}

bool RecverCard::sotBright(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void RecverCard::setSaveBright(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::SAVE_BRIGHT, sizeof(scan.bright),
					 &scan.bright, sink);
}

bool RecverCard::sotSaveBright(size_t len, uint8_t *data)
{
	return isOk(len, data);
}

void RecverCard::getBright(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::GET_BRIGHT, sink);
}

bool RecverCard::gotBright(size_t len, uint8_t *data)
{
	if (!data || len != sizeof(scan.bright)) return false;
	scan.bright = *(uint8_t *)data;
	return true;
}

void RecverCard::setSaveAll(const DataSink &sink)
{
	protocol->rpc(RecverCardMsg::SAVE_ALL, sink);
}

void RecverCard::setTuneOk(const DataSink &sink)
{
	setTune([this, sink](size_t len, uint8_t *data) {
		if (!sotTune(len, data)) {
			if (sink) sink(0 , nullptr);
			return;
		}
		setDotmap([this, sink](size_t len, uint8_t *data) {
			if (!sotDotmap(len, data)) {
				if (sink) sink(0 , nullptr);
				return;
			}
			setScan(sink);
		});
	});
}

void RecverCard::getAllImpl(size_t len, uint8_t *data)
{
	reenter (getAllCrt) {
		yield getVersion(getAllImplFunc);
		if (!gotVersion(len, data)) return;

		yield getModel(getAllImplFunc);
		if (!gotModel(len, data)) return;

		yield getScan(getAllImplFunc);
		if (!gotScan(len, data)) return;

		yield getDotmap(getAllImplFunc);
		if (!gotDotmap(len, data)) return;

		yield getGamma(getAllImplFunc);
		if (!gotGamma(len, data)) return;

		yield getPageBright(getAllImplFunc);
		if (!gotPageBright(len, data)) return;

		/*yield getBright(getAllImplFunc);
		if (!gotBright(len, data)) return;*/

		if (getAllSink) {
			getAllSink(this);
			getAllSink = nullptr;
		}
	}
}

void RecverCard::setAllImpl(size_t len, uint8_t *data)
{
	reenter (setAllCrt) {
		//yield setTune(setAllImplFunc);
		//if (!sotTune(len, data)) return;

		yield setScan(setAllImplFunc);
		if (!sotScan(len, data)) return;

		yield setDotmap(setAllImplFunc);
		if (!sotDotmap(len, data)) return;

		yield setGamma(setAllImplFunc);
		if (!sotGamma(len, data)) return;

		yield setPageBright(setAllImplFunc);
		if (!sotPageBright(len, data)) return;

		/*yield setBright(setAllImplFunc);
		if (!sotBright(len, data)) return;

		yield setSaveBright(setAllImplFunc);
		if (!sotSaveBright(len, data)) return;

		yield setSaveAll(setAllImplFunc);
		if (!sotSaveAll(len, data)) return;*/

		if (setAllSink) {
			setAllSink(this);
			setAllSink = nullptr;
		}
	}
}

void RecverCard::cancel()
{
	getAllSink = nullptr;
	setAllSink = nullptr;
	if (protocol) protocol->cancel();
}

void RecverCard::close()
{
	cancel();
	freenull(protocol);
}

std::wstring RecverCard::title()
{
	return name;
}

std::wstring RecverCard::tooltip()
{
	return detail;
}

bool RecverCard::adjustDotmap(DotPos *dots) {
	int moduleWidthAdjust = scan.moduleWidth + scan.emptyDots;

	int i, j, k;

	for (i = 0; i < scan.lines; ++i) {
		scan.lineAdjust[i] = (dots[i * scan.moduleWidth * scan.wrapTimes].row
							  ) % scan.lines;
	}

	int smallestMult = scan.moduleWidth * scan.wrapTimes * 32;
	smallestMult /= greatDivisor(scan.moduleWidth * scan.wrapTimes, 32);
	int emptyNum = smallestMult / scan.moduleWidth * scan.emptyDots;
	scan.dValidLen = 0;
	scan.vValidLen = 0;

	while (1) {
		scan.dValidLen += smallestMult;
		scan.vValidLen += smallestMult + emptyNum;
		if ((scan.dValidLen > (1024 - smallestMult))
			|| (scan.vValidLen > (1024 - (smallestMult + emptyNum)))) {
			break;
		}
	}

	for (i = 0; i < scan.vValidLen / (moduleWidthAdjust * scan.wrapTimes);
		 ++i) {
		for (j = 0; j < scan.wrapTimes; ++j) {
			//int y = dots[j * moduleWidthAdjust].row;
			for (k = 0; k < moduleWidthAdjust; ++k) {
				dotmap[i * moduleWidthAdjust * scan.wrapTimes
							   + j * moduleWidthAdjust + k]
					= i * scan.moduleWidth
					  + (dots[j * moduleWidthAdjust + k].row) / scan.lines
						* (1024 / scan.wrapTimes)
					  + dots[j * moduleWidthAdjust + k].col;
			}
		}
	}

	return true;
}

bool RecverCard::adjustCol()
{
	assert(screenAttr);
	size_t screnX = screenAttr->geo.x;
	int16_t i;
	int16_t maxWidth =0, maxHeight =0;
	int16_t widthAdjust =0;
	int32_t width1 =0;
	int32_t height1 =0;

	memset(colAdjust, 0, sizeof(colAdjust));
	memset(proxy.geosAdjusted, 0, sizeof(proxy.geosAdjusted));

	//height
	for(i = 0; i < RecverCard::RCVER_CARDS_MAX; ++i)
	{
		if((proxy.geos[i].y + proxy.geos[i].height) > height1)
		{
			height1 = proxy.geos[i].y + proxy.geos[i].height;
		}
		if((proxy.geos[i].x + proxy.geos[i].width) > width1)
		{
			width1 = proxy.geos[i].x + proxy.geos[i].width;
		}

		if(proxy.geos[i].width > maxWidth)
		{
			maxWidth = proxy.geos[i].width;
		}

		if(proxy.geos[i].height > maxHeight)
		{
			maxHeight = proxy.geos[i].height;
		}
	}

	for(i = 0; i < RecverCard::RCVER_CARDS_MAX; ++i)
	{
		if((proxy.geos[i].width <= 0) ||(proxy.geos[i].height <= 0))
			break;

		if(proxy.geos[i].width != 0)	//20150302 modify
		{
			proxy.geosAdjusted[i].x = proxy.geos[i].x + proxy.geos[i].width - maxWidth + screnX;
			proxy.geosAdjusted[i].y = proxy.geos[i].y;

			if(proxy.geosAdjusted[i].x < widthAdjust)
			{
				widthAdjust = proxy.geosAdjusted[i].x;
			}

			proxy.geosAdjusted[i].width = maxWidth;
			proxy.geosAdjusted[i].height = maxHeight;
		}
	}

	if(widthAdjust < 0)	//20150302 add
	{
		for(i = 0; i < RecverCard::RCVER_CARDS_MAX; ++i)
		{
			proxy.geosAdjusted[i].x += abs(widthAdjust);
		}
	}

	for(i = 0; i < sizeof(RecverCard::COL_ADJUST_SIZE); ++i)
	{
		colAdjust[i] = widthAdjust;
	}

	startCast();
	return true;
}

int RecverCard::adjustFreq()
{
	if (refreshRate <60) return FRAMEFRE_FREQUENCY_ERROR;
	if (0 == proxy.rows && 0 == proxy.cols) return FRAMEFRE_FREQUENCY_ERROR;

	memset(pageBright, 0xff, sizeof(pageBright));

	int width = proxy.geosAdjusted[0].width;
	int height = proxy.geosAdjusted[0].height;
	float onelineonepage =(float)width;
	onelineonepage =(float)((width*scan.wrapTimes +3)*4 +10)*(5+scan.freq)/5;
	float onelineonepagestr =16.0*(5+scan.freq)/5;
	int everypagebirght[16];
	int onelineclk;
	int i,j,ktime;
	int scantimes;
	int delta_scantimes;
	int max_scanclks;
	int begingray;
	BYTE beginlowerpage;
	int atleast_onelineonepage;

	int blastoprt =-1;

	if(width <=128)
		atleast_onelineonepage =4;
	else if(width <=256)
		atleast_onelineonepage =8;
	else if(width <=512)
		atleast_onelineonepage =16;
	else
		atleast_onelineonepage =32;

	atleast_onelineonepage +=18;

	atleast_onelineonepage *=(scan.baseColor &0x03) +1;

	atleast_onelineonepage *=(height/scan.intfLines)*scan.wrapTimes;

	if(atleast_onelineonepage <(onelineonepage +onelineonepagestr))
		atleast_onelineonepage =onelineonepage +onelineonepagestr;

	begingray =0;
	while(begingray >=0)
	{
		for(i =begingray, j =1; i >=0; --i, j <<=1)
		{
			everypagebirght[i] =j |0x8000;
		}

		for(i =begingray +1, j =2; i <scan.gray; ++i, j*=2)
		{
			everypagebirght[i] =j;
		}

		for(i =0, scantimes =0; i <scan.gray; ++i)
		{
			if(everypagebirght[i] >=0x8000)
				scantimes +=1;
			else
				scantimes +=everypagebirght[i];
		}
		i =(scantimes +(scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES))/((scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES)+1);
		delta_scantimes =i*((scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES)+1) -scantimes;
		scan.pagesTimes &=~(Scan::LEDPARABLK_PAGE_PAGES_MASK <<Scan::LEDPARABLK_PAGE_PAGES);
		scan.pagesTimes |=((i -1) &Scan::LEDPARABLK_PAGE_PAGES_MASK) <<Scan::LEDPARABLK_PAGE_PAGES;

		scantimes =i*((scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES)+1);
		max_scanclks =(750.0*1000000/5/scan.wrapTimes/scan.lines/refreshRate)/(scantimes);
		if(max_scanclks >=atleast_onelineonepage)
		{
			blastoprt =2;
			break;
		}
		else
		{
			begingray++;
			blastoprt =0;
		}
	}

	if(blastoprt !=2) return FRAMEFRE_WIDTHNOTREACH_ERROR;
	if (begingray < 0)
	{
		return FRAMEFRE_SYNCHRONIZENOTREACH_ERROR;
	}
	else if (blastoprt != 2)
	{
		return FRAMEFRE_WIDTHNOTREACH_ERROR;
	}

	everypagebirght[begingray] =max_scanclks;
	for(i =begingray -1; i >=0; --i)
	{
		everypagebirght[i] =everypagebirght[i +1]>>1;
	}

	if(0 >=everypagebirght[0]) return FRAMEFRE_WIDTHNOTREACH_ERROR;


	i =1;beginlowerpage=0;
	while(everypagebirght[0]/i >255)
	{
		i <<=1;beginlowerpage++;
	}
	scan.bright =everypagebirght[0]/i -1;
	scan.brightMax =everypagebirght[0]/i -1;


	scantimes =everypagebirght[0];
	for(i =0; i <begingray; ++i)
	{
		scantimes <<=1;
		scantimes++;
	}

	if(scantimes >max_scanclks)
	{
		{
			if(abs(scantimes -max_scanclks) >abs((scantimes>>1) -max_scanclks))
			{
				if(everypagebirght[0]-- <2)
					return FRAMEFRE_WIDTHNOTREACH_ERROR;

				scan.bright =everypagebirght[0] -1;
				scan.brightMax =everypagebirght[0] -1;
				max_scanclks =(max_scanclks>>1);
			}
			else
			{
				max_scanclks =scantimes;
			}
		}
	}

	scan.dotIndex =0;

	PageBright m_pagebright[1024];
	memset(m_pagebright, 0xff, sizeof(PageBright)*Scan::PAGE_TIMES_MAX*Scan::PAGE_PAGES_MAX);

	j =0;

	onelineclk =delta_scantimes;
	for(i =0; i <begingray; ++i)
	{
		m_pagebright[onelineclk].page =i;
		m_pagebright[onelineclk++].bright =i +beginlowerpage;
	}
	for(i =begingray, ktime =1; i <scan.gray; ++i, ktime <<=1)
	{
		for(j =0; j <ktime; ++j)
		{
			m_pagebright[onelineclk].page =i;
			m_pagebright[onelineclk++].bright =begingray +beginlowerpage;
		}
	}

	i =0;j =0;
	while(i <delta_scantimes)
	{
		for(j =0 ;i <delta_scantimes; ++i,j++)
		{

			if(begingray +j>=scan.gray) break;

			m_pagebright[i].page =m_pagebright[j +delta_scantimes +begingray].page;
			m_pagebright[j +delta_scantimes +begingray].bright -=1;
			m_pagebright[i].bright =m_pagebright[j +delta_scantimes +begingray].bright;

		}
	}

	delta_scantimes =(scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES) +1;
	scantimes =((scan.pagesTimes >>Scan::LEDPARABLK_PAGE_PAGES)&Scan::LEDPARABLK_PAGE_PAGES_MASK) +1;
	if(0 ==(scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES))
	{
		onelineclk =1024;
	}
	else if(1 ==(scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES))
	{
		onelineclk =512;
	}
	else if(3 >=(scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES))
	{
		onelineclk =256;
	}
	else if(7 >=(scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES))
	{
		onelineclk =128;
	}
	else if(15 >=(scan.pagesTimes >>Scan::LEDPARABLK_PAGE_TIMES))
	{
		onelineclk =64;
	}

	UINT curpage_bright =scan.brightMax +1;
	UINT totolpage_bright =0;
	const int GRAY_BRIGHT_TIMES[16] ={1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16*1024, 32*1024};

	for(i =0; i <delta_scantimes; ++i)
	{
		for(j =0; j <scantimes; ++j)
		{
			pageBright[i*onelineclk +j] =m_pagebright[delta_scantimes*j +i];
			totolpage_bright +=curpage_bright*GRAY_BRIGHT_TIMES[m_pagebright[delta_scantimes*j +i].bright];
		}
	}

	//if (brightPercent) *brightPercent =(float)totolpage_bright/(max_scanclks*delta_scantimes*(scantimes));

	return FRAMEFRE_SUCCESS;

}

void RecverCard::adjustGamma()
{
	int gray = pow(2, scan.gray) - 1;
	double exp = (double)scan.gamma / 10;
	for (int i = 0; i < GAMMA_SIZE; ++i) {
		gamma[i] = gray * pow((double)i / (GAMMA_SIZE - 1), exp);
	}
}

void RecverCard::startCast() {
	if (caster) caster->start(this);
}

int RecverCard::greatDivisor(uint16_t int1, uint16_t int2) {
	while (int1 != int2) {
		if (int1 > int2) {
			int1 = int1 - int2;
		} else {
			int2 = int2 - int1;
		}
	}

	return int1;
}
