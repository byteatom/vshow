#ifndef GECASTER_H
#define GECASTER_H

#include <stdint.h>
#include <string>
#include <memory>

#include "asio_prot.h"

class RecverCard;

class EthCaster
{
public:
	enum {
		CAST_CARD = 1,
		CAST_START = 2,
		CAST_STOP = 3,
		CAST_RECT = 4,
	};

	struct CardRect {
		int16_t height;
		int16_t width;
		int16_t x;
		int16_t y;
	};
	struct ScreenRect {
		int32_t left;
		int32_t top;
		int32_t right;
		int32_t bottom;
		int32_t count;
	};

	EthCaster();
	~EthCaster();

	void start(RecverCard *card);
	void stop();

private:
	void set(const std::string &cardId, const std::shared_ptr<ScreenRect> &rectsSp);
	std::shared_ptr<asio::deadline_timer> timer;
};

#endif // GECASTER_H
