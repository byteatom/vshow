#ifndef ETHPROT_H
#define ETHPROT_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>

#include <pcap.h>

#include "util/task_syncer.h"
#include "protocol.h"
#include "eth_msg.h"

class EthProt : public Protocol
{
public:
	struct NicInfo {
		std::string id;
		std::wstring name;
		std::wstring detail;
		EthMsg::Addr addr;
	};
	static void enumNics(std::vector<NicInfo> &infos);
	static std::string PCAP_DEV_PREFIX;

	explicit EthProt(std::string &id, EthMsg::Addr &srcAddr);
	virtual ~EthProt();

	virtual void send(const CardMsgSp &cardMsg, const IoSink &sink) override;
	virtual void recv(const IoSink &sink) override;
	virtual void cancel() override;

private:
	TaskSyncer *syncer;
	std::string id;
	EthMsg::Addr srcAddr;
	char sendErr[PCAP_ERRBUF_SIZE];
	pcap_t *sendPcap;
	char recvErr[PCAP_ERRBUF_SIZE];
	pcap_t *recvPcap;
};

#endif // ETHPROT_H
