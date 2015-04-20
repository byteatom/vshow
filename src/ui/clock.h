#ifndef CLOCK_H
#define CLOCK_H

#include "segment.h"
#include "clock_view.h"
#include "data/clock_attr.h"

class Clock : public Segment {
	Q_DECLARE_TR_FUNCTIONS(Clock)

public:
	static const SliceTypeId typeId;
	static const QString fileFilter;

	Clock(Region *owner, QTreeWidgetItem *treeItem, ClockAttr *attr);
	Clock(Region *owner, QTreeWidgetItem *treeItem);
	virtual ~Clock();

	virtual void showCfgers() override;
	virtual void hideCfgers() override;
	virtual void cacheView() override;

	virtual void play() override;
	virtual void pulse() override;
	void makeFrame();

	ClockAttr *attr();
	ClockView *view();

	QPixmap *cache;

private:
	void attrChanged(ClockInfo &newValue);
	unsigned int pulses;
	unsigned int seconds;
	QAtomicPointer<QPixmap> frame;
	Task task;
};

#endif    // CLOCK_H
