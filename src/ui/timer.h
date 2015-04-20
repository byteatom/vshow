#ifndef TIMER_H
#define TIMER_H

#include "segment.h"
#include "timer_view.h"
#include "data/timer_attr.h"

class Timer : public Segment {
	Q_DECLARE_TR_FUNCTIONS(Timer)

public:
	static const SliceTypeId typeId;
	static const QString fileFilter;

	Timer(Region *owner, QTreeWidgetItem *treeItem, TimerAttr *attr);
	Timer(Region *owner, QTreeWidgetItem *treeItem);
	virtual ~Timer();

	virtual void showCfgers() override;
	virtual void hideCfgers() override;
	virtual void cacheView() override;

	virtual void play() override;
	virtual void pulse() override;
	void makeFrame();
	TimerAttr *attr();
	TimerView *view();

	QPixmap *cache;

private:
	void attrChanged(TimerInfo &newValue);
	unsigned int pulses;
	unsigned int seconds;
	QAtomicPointer<QPixmap> frame;
	Task task;
};

#endif    // TIMER_H
