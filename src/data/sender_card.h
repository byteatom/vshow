#ifndef SENDERCARD_H
#define SENDERCARD_H

#include <string>
#include <functional>
#include <stdint.h>
#include <vector>
#include <map>

#include "recver_card.h"

class SenderCard : public Card
{
public:
#pragma pack(push)
#pragma pack(1)
	struct Ctrl{
		int16_t width{64};
		int16_t height{64};
		int16_t out1BaseY{0};
		int16_t out2BaseY{512};
		int16_t red{256};
		int16_t green{256};
		int16_t blue{256};
		int16_t mask{0};
	};
#pragma pack(pop)

	static void cfgMsg();
	static std::map<ByteBufSp, std::wstring> edids;
	static void loadEdids();
	static void createDummy(CardList &list);

	explicit SenderCard();  //for cfg
	explicit SenderCard(std::string &id); //for raw
	virtual ~SenderCard();

	virtual void loadCfg(Tlv*& tlv) override;
	virtual void saveCfg(std::ofstream &ofs) override;
	virtual void setCfg(Card *cfgCard) override;

	void getVersion(const DataSink &sink);
	void getModel(const DataSink &sink);
	void getColAdjust(const DataSink &sink);
	bool gotColAdjust(size_t len, uint8_t *data);
	void setColAdjust(const DataSink &sink);
	bool sotColAdjust(size_t len, uint8_t *data);
	void getOut1Geos(const DataSink &sink);
	bool gotOut1Geos(size_t len, uint8_t *data);
	void setOut1Geos(const DataSink &sink);
	bool sotOut1Geos(size_t len, uint8_t *data);
	void getOut2Geos(const DataSink &sink);
	bool gotOut2Geos(size_t len, uint8_t *data);
	void setOut2Geos(const DataSink &sink);
	bool sotOut2Geos(size_t len, uint8_t *data);
	void getEdid(const DataSink &sink);
	bool gotEdid(size_t len, uint8_t *data);
	void setEdid(const DataSink &sink);
	bool sotEdid(size_t len, uint8_t *data);
	void getCtrl(const DataSink &sink);
	bool gotCtrl(size_t len, uint8_t *data);
	void setCtrl(const DataSink &sink);
	bool sotCtrl(size_t len, uint8_t *data);
	virtual void setSaveAll(const DataSink &sink) override;

	virtual void getAllImpl(size_t len, uint8_t *data) override;
	virtual void setAllImpl(size_t len, uint8_t *data) override;
	virtual void cancel() override;
	virtual void close() override;

	virtual std::wstring title() override;
	virtual std::wstring tooltip() override;

	virtual bool adjustCol() override;

	CardProt *protocol;
	ByteBufSp edid;
	Ctrl ctrl;

	RecverCard outPort1;
	RecverCard outPort2;

	RecverCard &outPortStub;

private:
	DataSink getAllImplFunc;
	DataSink setAllImplFunc;
};

#endif // SENDERCARD_H
