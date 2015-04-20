#include "eth_prot.h"

#include <locale>
#include <codecvt>
#include <set>
#include <functional>
#include <cassert>

#include <ntddndis.h>
#include <Iphlpapi.h>
#include <Netcfgx.h>
//#include <Wdm.h> //for INTERFACE_TYPE
#include <packet32.h>

#include "io.h"
#include "util/registry.h"
#include "util/mem_util.h"
#include "qlog/qlog.h"

std::string EthProt::PCAP_DEV_PREFIX{"\\Device\\NPF_"};

void EthProt::enumNics(std::vector<NicInfo> &infos)
{
	std::set<std::string> ids;
	Registry nics(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}");
	DWORD subKeyCount = 0;
	DWORD subKeyLen = 0;
	if (ERROR_SUCCESS != nics.getKeyInfo(&subKeyCount, &subKeyLen))
		return;
	wchar_t *subKeyName = new wchar_t[subKeyLen + 1];
	std::unique_ptr<wchar_t> subKeyNameGuard{subKeyName};
	for (DWORD i = 0; i < subKeyCount; ++i) {
		DWORD nameLen = subKeyLen;
		if (ERROR_SUCCESS != nics.enumSubKey(i, subKeyName, &nameLen))
			break;
		Registry nic(nics.key, subKeyName);
		DWORD characteristics = 0;
		DWORD characteristicsLen = sizeof(characteristics);
		wchar_t id[64];
		DWORD idLen = sizeof(id);
		if (ERROR_SUCCESS != nic.getValue(L"Characteristics", (LPBYTE)&characteristics, &characteristicsLen))
			continue;
		if (!(characteristics & NCF_PHYSICAL))
			continue;
		if (ERROR_SUCCESS != nic.getValue(L"NetCfgInstanceId", (LPBYTE)&id, &idLen))
			continue;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		ids.insert(converter.to_bytes(std::wstring(id, idLen/2 - 1)));
	}

	ULONG addrSize = 0;
	GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_UNICAST | GAA_FLAG_SKIP_ANYCAST
						 | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER,
						 nullptr, nullptr, &addrSize);
	IP_ADAPTER_ADDRESSES *addrInfo = (IP_ADAPTER_ADDRESSES *)new char[addrSize];
	std::unique_ptr<IP_ADAPTER_ADDRESSES> addrGuard{addrInfo};
	if (ERROR_SUCCESS != GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_UNICAST | GAA_FLAG_SKIP_ANYCAST
						 | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER,
						 nullptr, addrInfo, &addrSize))
		return;

	for (IP_ADAPTER_ADDRESSES *addrItor = addrInfo; addrItor; addrItor = addrItor->Next)
	{
		if (IF_TYPE_ETHERNET_CSMACD != addrItor->IfType)
			continue;
		if (IfOperStatusUp != addrItor->OperStatus)
			continue;
		std::string id(addrItor->AdapterName);
		std::set<std::string>::iterator idItor = ids.find(id);
		if (idItor == ids.end())
			continue;
		//if (id != "{522DA763-B6C3-46D6-A43F-2F91D8CFE361}") continue;
		infos.push_back(NicInfo{PCAP_DEV_PREFIX + id,
								std::wstring{addrItor->FriendlyName},
								std::wstring{addrItor->Description},
								*(EthMsg::Addr*)(addrItor->PhysicalAddress)});
	}

#if 0
	std::vector<std::string> names;
	std::vector<std::wstring> details;
	std::vector<EthMsg::Addr> addrs;
	ULONG namesBufLen = 8192;
	char *namesBuf = new char[namesBufLen];
	std::unique_ptr<char> namesBufGuard{namesBuf};
	if (0 == PacketGetAdapterNames((wchar_t*)namesBuf, &namesBufLen)) return;
	char *head = namesBuf;
	char *tail = namesBuf;
	while (0 != *tail || 0 != *(tail - 1)) {
		if (0 == *tail) {
			names.push_back(std::string(head));
			head = tail + 1;
		}
		++tail;
	}
	if (0 == names.size()) return;
	head = ++tail;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	while (0 != *tail || 0 != *(tail - 1)) {
		if (0 == *tail) {
			details.push_back(converter.from_bytes(head));
			head = tail + 1;
		}
		++tail;
	}

	for (std::string name: names) {
		LPADAPTER adapter = PacketOpenAdapter(const_cast<char *>(name.c_str()));
		if (!adapter || adapter->hFile == INVALID_HANDLE_VALUE) continue;
		PACKET_OID_DATA  *oidData = (PACKET_OID_DATA *)new char[sizeof(PACKET_OID_DATA) + EthMsg::ADDR_SIZE];
		oidData->Oid = OID_802_3_CURRENT_ADDRESS;
		oidData->Length = EthMsg::ADDR_SIZE;
		memset(oidData->Data, 0, EthMsg::ADDR_SIZE);
		if (PacketRequest(adapter, false, oidData))
			addrs.push_back(*(EthMsg::Addr *)(oidData->Data));
		delete oidData;
		PacketCloseAdapter(adapter);
	}
#endif
}

EthProt::EthProt(std::string &id, EthMsg::Addr &srcAddr) :
	Protocol{nullptr},
	syncer{&Io::obj->syncer},
	id(id),
	srcAddr(srcAddr),
	sendPcap{nullptr},
	recvPcap{nullptr}
{
	sendPcap = pcap_open_live(this->id.c_str(), 65536, false, 0, sendErr);
	assert(sendPcap);
	recvPcap = pcap_open_live(this->id.c_str(), 65536, true, 250, recvErr);
	assert(recvPcap);
	bpf_program bpf;
	int ret = -1;
	ret = pcap_compile(recvPcap, &bpf,
			"ether dst ff:ff:ff:ff:ff:ff and ether src 88:88:88:88:88:88 and ether proto 0xaaa2",
			true, 0);
	assert(0 == ret);
	ret = pcap_setfilter(recvPcap, &bpf);
	assert(0 == ret);
	pcap_setmintocopy(recvPcap, EthMsg::MSG_LEN_MIN);
}

EthProt::~EthProt()
{
	if (sendPcap) pcap_close(sendPcap);
	if (recvPcap) {
		pcap_breakloop(recvPcap);
		pcap_close(recvPcap);
	}
}

void EthProt::send(const CardMsgSp &cardMsg, const IoSink &sink)
{
	EthMsgSp ethMsg = makeEthMsgSp(&*cardMsg, srcAddr);
	cardMsg->iok = ( 0 == pcap_sendpacket(sendPcap, ethMsg->msg, ethMsg->size));
	if (sink) (*syncer)(new Task([cardMsg, sink](){sink(cardMsg);}));
}

void EthProt::recv(const IoSink &sink)
{
	struct pcap_pkthdr *head = nullptr;
	const u_char *data = nullptr;
	int ret = pcap_next_ex(recvPcap, &head, &data);
	if (1 == ret) {
		if (sink) {
			EthMsgSp ethMsg = makeEthMsgSp((size_t)head->len, (void *)data);
			CardMsgSp cardMsg = makeCardMsgSp(&*ethMsg);
			(*syncer)(new Task([sink, cardMsg](){sink(cardMsg);}));
		}
	}
}

void EthProt::cancel()
{
	if (recvPcap) {
		pcap_breakloop(recvPcap);
	}
}
