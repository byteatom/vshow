#include "eth_caster.h"

#include "data/data.h"
#include "data/recver_card.h"

EthCaster::EthCaster()
{
}

EthCaster::~EthCaster()
{
	if (timer) timer->cancel();
}

void EthCaster::start(RecverCard *card)
{
	if (!card->screenAttr) return;
	RecverCard::Geo *geos = card->proxy.geos;

	int count = 0;
	for (count = 0; count < RecverCard::RCVER_CARDS_MAX; ++count) {
		if (0 == geos[count].x
			&& 0 == geos[count].y
			&& 0 == geos[count].width
			&& 0 == geos[count].height) {
			break;
		}
	}

	if (0 == count) return;

	GeoAttr screenGeo = card->screenAttr->geo;
	ScreenRect *screenRect = (ScreenRect *)(new uint8_t[sizeof(ScreenRect) + sizeof(CardRect) * count]);
	screenRect->left = screenGeo.x;
	screenRect->top = screenGeo.y;
	screenRect->right = screenGeo.x + screenGeo.width;
	screenRect->bottom = screenGeo.y + screenGeo.height;
	screenRect->count = count;
	CardRect *cardRects = (CardRect *)(screenRect + 1);
	for (int i = 0; i < count; i++) {
		cardRects[i].x = geos[i].x;
		cardRects[i].y = geos[i].y;
		cardRects[i].width = geos[i].width;
		cardRects[i].height = geos[i].height;
	}
	std::shared_ptr<ScreenRect> rectsSp{screenRect};
	set(card->id, rectsSp);
}

void EthCaster::set(const std::string &cardId, const std::shared_ptr<ScreenRect> &rectsSp)
{
	//return;
	HWND castWnd = 0;
	HANDLE mapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, L"vshow_cast");
	if (nullptr == mapFile) {
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		std::wstring castExe = Data::obj->appAttr->runDir + L"/vshow_cast.exe";
		if (!CreateProcess(nullptr,
							const_cast<LPWSTR>(castExe.c_str()),
							nullptr, nullptr, false, CREATE_NO_WINDOW,
							nullptr, nullptr, &si, &pi)) {
			return;
		}
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		mapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, L"vshow_cast");
		if (nullptr == mapFile) {
			if (!timer) timer = std::make_shared<asio::deadline_timer>(*AsioProt::service);
			timer->expires_from_now(boost::posix_time::seconds(1));
			timer->async_wait([this, cardId, rectsSp](const system::error_code& err){
				if (err != asio::error::operation_aborted) {
					set(cardId, rectsSp);
				}
			});
			return;
		} else if (timer)
			timer->cancel();
	}
	LPVOID mapMem = MapViewOfFile(mapFile, FILE_MAP_ALL_ACCESS, 0, 0, 512);
	if (nullptr == mapMem) {
		CloseHandle(mapFile);
		return;
	}
	castWnd = *(HWND *)mapMem;
	UnmapViewOfFile(mapMem);
	CloseHandle(mapFile);

	if (0 == castWnd) return;

	COPYDATASTRUCT copyData;

	copyData.dwData = CAST_STOP;
	copyData.cbData = 0;
	copyData.lpData = nullptr;
	SendMessage(castWnd, WM_COPYDATA, 0, LPARAM(&copyData));

	copyData.dwData = CAST_RECT;
	copyData.cbData = sizeof(ScreenRect) + sizeof(CardRect) * rectsSp->count;
	copyData.lpData = rectsSp.get();
	SendMessage(castWnd, WM_COPYDATA, 0, LPARAM(&copyData));

	copyData.dwData = CAST_CARD;
	copyData.cbData = cardId.size() + 1;
	copyData.lpData = (PVOID)(cardId.c_str());
	SendMessage(castWnd, WM_COPYDATA, 0, LPARAM(&copyData));

	copyData.dwData = CAST_START;
	copyData.cbData = 0;
	copyData.lpData = nullptr;
	SendMessage(castWnd, WM_COPYDATA, 0, LPARAM(&copyData));
}

void EthCaster::stop()
{
	if (timer) timer->cancel();
}

