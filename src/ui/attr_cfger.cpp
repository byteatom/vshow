#include "attr_cfger.h"

#include <cassert>

AttrCfger::AttrCfger() : QFrame(nullptr) {
	hide();
	setSizePolicy(sizePolicy().horizontalPolicy(), QSizePolicy::Fixed);
}

AttrCfger::~AttrCfger() {
}

QSize AttrCfger::sizeHint() const {
	return frameSize();
}
