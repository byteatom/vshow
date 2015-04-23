#ifndef PICTURE_H
#define PICTURE_H

#include <string>

#include "segment.h"
#include "picture_view.h"
#include "data/picture_attr.h"
#include "animator.h"

class Picture : public Segment {
	Q_DECLARE_TR_FUNCTIONS(Picture)
public:
	static const SliceTypeId typeId;
	static const QString fileFilter;
	static const QString fileTitle;

	Picture(Region *owner, QTreeWidgetItem *treeItem, PictureAttr *attr);
	Picture(Region *owner, QTreeWidgetItem *treeItem);
	virtual ~Picture();

	virtual void showCfgers() override;
	virtual void hideCfgers() override;

	void fileChanged(std::wstring &path);
	void scaleChanged(ImageTool::Scale &scale);

	virtual void cacheView() override;

	virtual void start() override;
	virtual void play() override;
	void makeFrame();
	virtual void pulse() override;
	virtual void end() override;
	virtual void stop() override;
	PictureAttr *attr();
	PictureView *view();

	QPixmap cache;

	unsigned int pulses;
	QPixmap origin;
	QPixmap frame;
	Animator animator;
	QTimer stayTimer;
	Task task;
};

#endif    // PICTURE_H
