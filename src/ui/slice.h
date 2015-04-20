#ifndef SLICE_H
#define SLICE_H

#include <list>

#include <QCoreApplication>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QPixmap>
#include <QTimer>
#include <QMutex>

#include "data/slice_attr.h"

class Slice : public QObject, public TreeNode {
	Q_OBJECT
public:
	static const unsigned int TICK;
	static const unsigned int TICK_PER_SEC;
	static const QPointF POS_TOPLEFT;

	typedef enum { SELF, SERIAL, PARELLEL } Mode;

	typedef enum { IDEL, STARTING, PLAYING, STAYING, REPEATING } State;

	explicit Slice(Slice *owner, QTreeWidgetItem *treeItem);
	virtual ~Slice();

	void init(SliceAttr *attr, QWidget *view);

	// edit
	virtual void showCfgers();
	virtual void hideCfgers();
	bool isCurrent();
	bool isActive();
	virtual void showView();
	virtual void cacheView();
	virtual void updateView();
	virtual void repaintView();
	virtual void raiseView();
	virtual void hideView();
	virtual void inflate();
	virtual void deflate();

	void nameChanged(std::wstring &name);
	void colorChanged(ImageAttr::Argb &color);
	void imageChanged(std::wstring &file);
	void scaleChanged(ImageTool::Scale &scale);

	virtual void start();
	virtual void play();
	virtual void pulse();
	virtual void organEnd(Slice *organ);
	virtual void end();
	virtual void stop();

	SliceAttr *attr();
	QWidget *view();

	Slice *owner;
	SliceAttr *attr_;
	QWidget *view_;
	QTreeWidgetItem *treeItem;
	QPixmap bgCache;

	Mode mode;
	std::list<TreeNode *>::iterator serialItor;
	bool playing;
	bool endOnce;
	bool rewindOrgan;
	QMutex frameMutex;
};

#endif    // SLICE_H
