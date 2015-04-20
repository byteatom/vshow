#ifndef TLV_H
#define TLV_H

#include <stdint.h>

struct Tlv
{
	uint32_t type;
	uint32_t len;
	uint8_t value[0];

	size_t allLen() {return sizeof(Tlv) + len;}
	Tlv* next() {return (Tlv *)((char *)this + allLen());}
};

#endif // TLV_H
