#include "card_prot.h"

#include "card_msg.h"
#include "serial_prot.h"
#include "io.h"
#include "qlog/qlog.h"
#include "util/mem_util.h"

CardProt::CardProt(CardType cardType, std::string &id) :
	CardProt{cardType, new SerialProt{id}}
{
	freeCarrier = true;
}

CardProt::CardProt(CardType cardType, Protocol *carrier) :
	Protocol{carrier},
	cardType{cardType},
	outerMsgType{0},
	syncer{&Io::obj->syncer},
	freeCarrier{false},
	timer{*AsioProt::service}
{
}

CardProt::~CardProt()
{
	if (freeCarrier) freenull(carrier);
}

void CardProt::send(const CardMsgSp &msg, const IoSink &sink)
{
	CardMsgSp outerMsg = makeCardMsgSp(0, msg->outerMsgType, 0, &*msg);
	carrier->send(outerMsg, [sink](const CardMsgSp &outerMsg) {
		if (!sink) return;
		CardMsgSp innerMsg = makeCardMsgSp(&*outerMsg);
		sink(innerMsg);
	});
}

void CardProt::recv(const IoSink &sink)
{
	carrier->recv([sink](const CardMsgSp &msg) {
		if (!sink) return;
		if (msg->iok)
			msg->iok = msg->checkSum();
		CardMsgSp innerMsg = makeCardMsgSp(&*msg);
		sink(innerMsg);
	});
}

void CardProt::cancel()
{
	timer.cancel();
	Protocol::cancel();
}

void CardProt::rpc(uint16_t addr, uint8_t	type,
				   uint16_t dataLen, uint8_t  *data, const DataSink &sink)
{
	qlog<< "rpc " << qdebug;
	CardMsgSpListSp rsps = makeCardMsgSpListSp;
	CardMsgSpListSp reqs = splitMsg(cardType, addr, type, dataLen, data);
	session(reqs, rsps, sink);
}

void CardProt::session(const CardMsgSpListSp &reqs, const CardMsgSpListSp &rsps,
					   const DataSink &sink)
{
	qlog<< "session count " << reqs->size() << qdebug;
	if (!reqs->empty()) {
		CardMsgSp req = reqs->front();

		if (++(req->retryCnt) > RETRY_MAX && sink) {
			qlog<< "retry failed" << qdebug;
			sink(0, nullptr);
			return;
		}
		qlog<< "start timer, retry " << req->retryCnt << qdebug;
		timer.expires_from_now(boost::posix_time::seconds(req->retryCnt));
		timer.async_wait([this, reqs, rsps, sink](const system::error_code& err){
			if (err != asio::error::operation_aborted) {
				qlog<< "timeout error code "  << err.value() << qdebug;
				carrier->cancel();
				(*syncer)(new Task([this, reqs, rsps, sink](){session(reqs, rsps, sink);}));
			}
		});

		req->outerMsgType = outerMsgType;
		carrier->send(req, [this, reqs, rsps, sink](const CardMsgSp &req) {
			qlog<< "send code " << req->iok << qdebug;
			if (!req->iok) {
				if (sink) sink(0, nullptr);
				return;
			}
			CardMsgSp rsp = makeCardMsgSpRecv;
			carrier->recv([this, reqs, rsps, req, sink](const CardMsgSp rsp) {
				rsp->iok = rsp->iok && req->type() == rsp->type() && rsp->checkSum();
				qlog<< "recv code " << rsp->iok << qdebug;
				if (!rsp->iok) {
					if (sink) sink(0, nullptr);
				} else {
					timer.cancel();
					reqs->pop_front();
					rsps->push_back(rsp);
					session(reqs, rsps, sink);
				}
			});
		});
	} else {
		ByteBufSp bytes = joinMsgs(cardType, rsps);
		if (sink) sink(bytes->size, bytes->buf);
	}
}

void CardProt::rpc(uint8_t	type, const DataSink &sink)
{
	rpc(0, type, 0, nullptr, sink);
}

void CardProt::rpc(uint8_t type, uint16_t dataLen, uint8_t  *data, const DataSink &sink)
{
	rpc(0, type, dataLen, data, sink);
}






