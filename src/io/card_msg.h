#ifndef CARDMSG_H
#define CARDMSG_H

#include <cstdint>
#include <memory>
#include <list>
#include <algorithm>

#include "msg_buf.h"
#include "util/endian.h"

typedef enum {
	SENDER_CARD,
	RECVER_CARD,
	CARD_TYPE_MAX
}CardType;

namespace RecverCardMsg {
enum {
	GET_VERSION = 0x02,
	GET_MODEL = 0x03,
	GET_SCAN = 0x8,
	SET_SCAN = 0x9,
	GET_DOTMAP = 0x10,
	SET_DOTMAP = 0x11,
	GET_GAMMA = 0x12,
	SET_GAMMA = 0x13,
	GET_PAGE_BRIGHT = 0x14,
	SET_PAGE_BRIGHT = 0x15,
	SET_TUNE = 0x30,
	SET_BRIGHT = 0x40,
	SAVE_BRIGHT = 0x41,
	GET_BRIGHT = 0x42,
	SAVE_ALL = 0x43,
	GET_BIN_FLAG = 0x55,
	TYPE_MAX,
};
}

namespace SenderCardMsg {
enum {
	GET_VERSION = 0x02,
	GET_MODEL = 0x03,
	GET_COL_ADJUST = 0x10,
	SET_COL_ADJUST = 0x11,
	GET_OUT1_GEOS = 0x20,
	SET_OUT1_GEOS = 0x21,
	GET_OUT2_GEOS = 0x22,
	SET_OUT2_GEOS = 0x23,
	GET_EDID = 0x30,
	SET_EDID = 0x31,
	GET_CTRL = 0x40,
	SET_CTRL = 0x41,
	SAVE_ALL = 0x43,
	OUTPORT1 = 0x80,
	OUTPORT2 = 0x81,
	TYPE_MAX,
};
}

enum {
	DATA_LEN_1 = 1,
	DATA_OK = 0,
	DATA_ERR = 1,
	DATA_LEN_MAX = 512,
};

struct MsgCfg
{
	uint32_t	wishLen{DATA_LEN_MAX};
};

class EthMsg;

class CardMsg : public MsgBuf
{
public:
	enum {
		PREAMBLE = 2,
		POSTAMBLE = 3,
	};
#pragma pack(push)
#pragma pack(1)
	struct Head
	{
		uint8_t		preamble;
		uint16_t	addr;
		uint8_t		type;
		uint32_t	offset;
		uint16_t	dataLen;
#pragma warning(push)
#pragma warning(disable : 4200)
		uint8_t     data[0];
#pragma warning(pop)
	};

	struct Tail
	{
		uint8_t		postamble;
		uint8_t		sum;
	};
#pragma pack(pop)

	static MsgCfg cfg[CARD_TYPE_MAX][SenderCardMsg::TYPE_MAX];
	static void setTypeWishLen(CardType role, int type, uint32_t len);
	static uint32_t getTypeWishLen(CardType role, int type);
	static size_t headLen(){return sizeof(Head);}
	static size_t tailLen(){return sizeof(Tail);}

	//send
	explicit CardMsg(uint16_t addr, uint8_t type, uint32_t	offset,
					 size_t dataLen, void *data = nullptr);
	//recv
	explicit CardMsg(size_t dataLen);
	//expand
	explicit CardMsg(uint16_t addr, uint8_t type, uint32_t	offset,
					 MsgBuf *msgBuf);
	//shrink
	explicit CardMsg(CardMsg *outerMsg);
	explicit CardMsg(EthMsg *ethMsg);
	CardMsg& operator=(const CardMsg&) = delete;
	virtual ~CardMsg();

	void set(uint16_t addr, uint8_t type, uint32_t	offset,
				 uint16_t dataLen, void *data = nullptr);
	uint8_t type() {return ((Head*)msg)->type;}
	uint16_t dataLen() {return ntoh2(((Head*)msg)->dataLen);}
	uint8_t* data() {return ((Head*)msg)->data;}
	void sum();
	bool checkSum();

	uint8_t innerType();
	uint16_t innerDataLen();

	uint8_t outerMsgType;
	int retryCnt;
};


typedef std::shared_ptr<CardMsg> CardMsgSp;
#define makeCardMsgSp(...) std::make_shared<CardMsg>(__VA_ARGS__)
#define makeCardMsgSpRecv std::make_shared<CardMsg>(1024)
typedef std::list<CardMsgSp> CardMsgSpList;
typedef std::shared_ptr<CardMsgSpList> CardMsgSpListSp;
#define makeCardMsgSpListSp std::make_shared<CardMsgSpList>()
CardMsgSpListSp splitMsg(CardType role, uint16_t addr, uint8_t type,
				uint16_t dataLen, uint8_t *data);
ByteBufSp joinMsgs(CardType role, const CardMsgSpListSp &msgs);


#endif // CARDMSG_H
