#ifndef WATCH_H
#define WATCH_H

#include <QAtomicPointer>
#include "segment.h"
#include "watch_view.h"
#include "data/watch_attr.h"

class Watch : public Segment {
	Q_DECLARE_TR_FUNCTIONS(Watch)

public:
	static const SliceTypeId typeId;
	static const QString fileFilter;

	Watch(Region *owner, QTreeWidgetItem *treeItem, WatchAttr *attr);
	Watch(Region *owner, QTreeWidgetItem *treeItem);
	virtual ~Watch();

	virtual void showCfgers() override;
	virtual void hideCfgers() override;
	virtual void cacheView() override;

	virtual void play() override;
	void makeFrame();
	virtual void pulse() override;
	WatchAttr *attr();
	WatchView *view();

	QPixmap *cache;

private:
	void attrChanged(DialInfo &newValue);
	unsigned int pulses;
	unsigned int seconds;
	QAtomicPointer<QPixmap> frame;
	Task task;
};

#endif    // WATCH_H
