#ifndef OFFICE_H
#define OFFICE_H

#include "segment.h"
#include "office_view.h"
#include "data/office_attr.h"

class Office : public Segment {
	Q_DECLARE_TR_FUNCTIONS(Office)

public:
	static const SliceTypeId typeId;
	static const QString fileFilter;
	static const QString fileTitle;
	static const int tipFontSize;
	static const QString wordPixPrefix;
	static const QString excelPixPrefix;
	static const QString powerPointPixPrefix;

	explicit Office(Region *owner, QTreeWidgetItem *treeItem, OfficeAttr *attr);
	explicit Office(Region *owner, QTreeWidgetItem *treeItem);
	virtual ~Office();

	virtual void showCfgers() override;
	virtual void hideCfgers() override;
	virtual void cacheView() override;

	virtual void play() override;
	void makeFrame();
	virtual void pulse() override;
	OfficeAttr *attr();
	OfficeView *view();

	QPixmap *cache;

private:
	void fileChanged(std::wstring &path);
	void scaleChanged(ImageTool::Scale &scale);
	void attrChanged();
	unsigned int pulses;
	QAtomicPointer<QPixmap> frame;
	Task task;
};

#endif    // OFFICE_H
