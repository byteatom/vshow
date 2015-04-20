#ifndef RECVERCARD_H
#define RECVERCARD_H

#include <string>
#include <vector>

#include "card.h"
#include "io/card_prot.h"
#include "io/eth_prot.h"

class SenderCard;
class EthCaster;

class RecverCard : public Card {
public:
#pragma pack(push)
#pragma pack(1)
	struct Scan {
		enum {
			DISPLAY_3A = 0x3A,
			DISPLAY_A5 = 0xA5,
			DATA_MODE_0 = 0,
			DATA_MODE_5A = 0x5A,
			BMASK_KEEP_LAST = (1 << 0),
			BMASK_FADE_IN = (1 << 1),
			BMASK_SCAN_MODE_MASK = (3 << 2),
			BMASK_SCAN_MODE_FRAME = (0 << 2),
			BMASK_SCAN_MODE_BRIGHT = (1 << 2),
			BMASK_PAGE = (1 << 4),
			BMASK_LINE_MODE = (1 << 5),
			BMASK_OE_POLARITY = (1 << 6),
			BMASK_DATA_POLARITY = (1 << 7),
			RGB_SEL_TUNE_COLOR1 = 1,
			RGB_SEL_TUNE_COLOR2 = 2,
			RGB_SEL_TUNE_COLOR3 = 4,
			RGB_SEL_TUNE_COLOR4 = 8,
			RGB_SEL_RED = 0,
			RGB_SEL_GREEN = 1,
			RGB_SEL_BLUE = 2,
			RGB_SEL_BLACK = 3,
			RGB_SEL_E4 = 0xE4,
			RBG_SEL_24 = 0x24,
			INTF_LINES_16 = 16,
			WRAP_TIMES_1 = 1,
			GRAY_8 = 8,
			BASE_COLOR_MONO = 1,
			BASE_COLOR_FULL = 2,
			BASE_COLOR_MODE_MASK = 3,
			BASE_COLOR_HUB_MASK = 4,
			BASE_COLOR_6 = 6,
			BLANK_20 = 20,
			BRIGHT_0 = 0,
			BRIGHT_1 = 1,
			BRIGHT_8 = 8,
			BRIGHT_15 = 15,
			GAMMA_15 = 15,
			FREQ_8 = 8,
			FREQ_12 = 12,
			DATA_TYPE_WHITE = 1,
			DATA_TYPE_BLACK = 2,
			DATA_TYPE_LINES = 3,
			DATA_TYPE_LINE = 4,
			DATA_TYPE_DOT_VEC = 5,
			DATA_TYPE_COLOR_VEC = 6,
			PAGE_TIMES_7 = 7,
			PAGE_TIMES_MAX = 32,
			PAGE_PAGES_MAX = 32,
			VALID_LEN_1024 = 1024,
			MODULE_WIDTH_32 = 32,
			MODULE_HEIGHT_16 = 16,
			LEDPARABLK_PAGE_PAGES = 0,
			LEDPARABLK_PAGE_TIMES = 10,
			LEDPARABLK_PAGE_PAGES_MASK = 0x3ff,
			LEDPARABLK_PAGE_TIMES_MASK = 0x3f,
		};

		Scan();
		void tuneInit(Scan &scan);
		void tuneModule();
		void tuneDataPolarity();
		void tuneOePolarity();
		void tuneRgbSel();
		void tuneIntfLines();
		void tuneWrapTimes();
		void tuneDotmap();
		void tuneEnd();
		void tuneFinish(Scan &scan);

		int getColorMode();
		void setColorMode(int mode);
		bool isColorMono();
		bool isColorFull();
		void setHub(bool hub);
		bool isHub();
		void set138(bool hub);
		bool is138();
		void setDataPolarity(bool reverse);
		bool isDataPolarityReverse();
		void setOePolarity(bool reverse);
		bool isOePolarityReverse();
		int getRbgSel1();
		int getRgbSel2();
		int getRgbSel3();
		int getRgbSel4();
		void setRgbSel(int color1, int color2, int color3, int color4);
		void setRgbSelTune(int color);

		uint8_t display{0};
		uint8_t dataMode{0};
		uint8_t lineAdjust[32];
		uint8_t bmask{BMASK_DATA_POLARITY | BMASK_OE_POLARITY | BMASK_PAGE};
		uint8_t rgbSel{RGB_SEL_E4};
		uint8_t lines{INTF_LINES_16};
		uint8_t intfLines{INTF_LINES_16};
		uint8_t wrapTimes{WRAP_TIMES_1};
		uint8_t gray{GRAY_8};
		uint8_t baseColor{BASE_COLOR_MODE_MASK};
		uint8_t blank{BLANK_20};
		uint8_t brightMax{BRIGHT_1};
		uint8_t bright{BRIGHT_1};
		uint8_t gamma{GAMMA_15};
		uint8_t freq{0};
		uint8_t dataType{0};
		uint8_t currentLine{0};
		int16_t dotIndex{0};
		int16_t pagesTimes{PAGE_TIMES_7};
		int16_t vValidLen{VALID_LEN_1024};
		int16_t dValidLen{VALID_LEN_1024};
		uint8_t emptyDots{0};
		uint8_t moduleWidth{MODULE_WIDTH_32};
		uint8_t moduleHeight{MODULE_HEIGHT_16};
	};
#pragma pack(pop)
	enum {
		WIDTH_DEFAULT = 64,
		WIDTH_MAX = 1024,
		HEIGHT_DEFAULT = 64,
		HEIGHT_MAX = 512,
		DOTMAP_SIZE = 1024,
		COL_ADJUST_SIZE = 2048,
		RCVER_CARDS_MAX = 256,
		GAMMA_SIZE = 256,
	};
	struct DotPos{
		int row{0};
		int col{0};
	};
	enum {
		LINK_0 = 0,
		LINK_1,
		LINK_2,
		LINK_3,
		LINK_4,
		LINK_5,
		LINK_6,
		LINK_7,
		LINK_8,
		STYLE_DEFAULT = LINK_1,
	};
	struct Dim {
		int16_t row{0};
		int16_t col{0};
	};
	struct Geo {
		int16_t x{0};
		int16_t y{0};
		int16_t width{0};
		int16_t height{0};
	};
	struct Proxy {
		int style{STYLE_DEFAULT};
		size_t rows{0};
		size_t cols{0};
		Dim dims[RCVER_CARDS_MAX];
		Geo geos[RCVER_CARDS_MAX];
		Geo geosAdjusted[RCVER_CARDS_MAX];
	};

	enum {
		FRAMEFRE_SUCCESS,
		FRAMEFRE_FREQUENCY_ERROR,
		FRAMEFRE_WIDTHNOTREACH_ERROR,
		FRAMEFRE_SYNCHRONIZENOTREACH_ERROR,
	};
	struct PageBright
	{
		uint8_t page;
		uint8_t bright;
	};
	enum {
		CFG_PROXY = CFG_CARD_MAX,
	};

	static void cfgMsg();
	static const std::wstring scanDir;
	static const std::wstring scanExt;
	static std::map<std::wstring, ByteBufSp> scans;
	static void loadScans();
	void loadScan(std::wstring &file);
	void saveScan(std::wstring &file);
	static void createDummy(CardList &list);

	explicit RecverCard();  //for cfg
	explicit RecverCard(SenderCard *owner); //for stub
	explicit RecverCard(std::string &id, EthMsg::Addr &addr); //for raw
	virtual ~RecverCard();

	virtual void loadCfg(Tlv*& tlv) override;
	virtual void saveCfg(std::ofstream &ofs) override;
	virtual void setCfg(Card *cfgCard) override;

	void getVersion(const DataSink &sink);
	void getModel(const DataSink &sink);
	void getScan(const DataSink &sink);
	bool gotScan(size_t len, uint8_t *data);
	void setScan(const DataSink &sink);
	bool sotScan(size_t len, uint8_t *data);
	void getDotmap(const DataSink &sink);
	bool gotDotmap(size_t len, uint8_t *data);
	void setDotmap(const DataSink &sink);
	bool sotDotmap(size_t len, uint8_t *data);
	void getGamma(const DataSink &sink);
	bool gotGamma(size_t len, uint8_t *data);
	void setGamma(const DataSink &sink);
	bool sotGamma(size_t len, uint8_t *data);
	void getPageBright(const DataSink &sink);
	bool gotPageBright(size_t len, uint8_t *data);
	void setPageBright(const DataSink &sink);
	bool sotPageBright(size_t len, uint8_t *data);
	void setTune(const DataSink &sink);
	bool sotTune(size_t len, uint8_t *data);
	void setBright(const DataSink &sink);
	bool sotBright(size_t len, uint8_t *data);
	void setSaveBright(const DataSink &sink);
	bool sotSaveBright(size_t len, uint8_t *data);
	void getBright(const DataSink &sink);
	bool gotBright(size_t len, uint8_t *data);
	virtual void setSaveAll(const DataSink &sink) override;
	void setTuneOk(const DataSink &sink);

	virtual void getAllImpl(size_t len, uint8_t *data) override;
	virtual void setAllImpl(size_t len, uint8_t *data) override;
	virtual void cancel() override;
	virtual void close() override;

	virtual std::wstring title() override;
	virtual std::wstring tooltip() override;

	bool adjustDotmap(DotPos *dots);
	virtual bool adjustCol() override;
	int adjustFreq();
	void adjustGamma();

	void startCast();

	Scan scan;
	Scan tune;
	std::wstring scanStem;
	CardProt *protocol;
	int16_t dotmap[DOTMAP_SIZE];
	uint16_t gamma[GAMMA_SIZE];
	Proxy proxy;
	int16_t colAdjust[COL_ADJUST_SIZE];
	PageBright pageBright[Scan::PAGE_TIMES_MAX * Scan::PAGE_PAGES_MAX];
	int refreshRate = 120;

private:
	void init();

	int greatDivisor(uint16_t int1, uint16_t int2);

	asio::coroutine *setTuneOkCrt;
	DataSink getAllImplFunc;
	DataSink setAllImplFunc;
	EthCaster *caster;
};

#endif    // RECVERCARD_H
