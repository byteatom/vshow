#ifndef TEXT_H
#define TEXT_H

#include <QTimer>

#include "segment.h"
#include "text_view.h"
#include "data/text_attr.h"
#include "animator.h"

class Text : public Segment {
	Q_DECLARE_TR_FUNCTIONS(Text)

public:
	static const SliceTypeId typeId;
	static const QString fileFilter;

	Text(Region *owner, QTreeWidgetItem *treeItem, TextAttr *attr);
	Text(Region *owner, QTreeWidgetItem *treeItem);
	virtual ~Text();

	virtual void showCfgers() override;
	virtual void hideCfgers() override;

	virtual void start() override;
	virtual void play() override;
	void makeFrame();
	virtual void pulse() override;
	virtual void end() override;
	virtual void stop() override;
	TextAttr *attr();
	TextView *view();

	QPixmap cache;
	TextEditor *editor;
	unsigned int pulses;
	QPixmap origin;
	QPixmap frame;
	Animator animator;
	QTimer stayTimer;
	Task task;

private:
	void editorFormatChanged(const QTextCharFormat &format);
	void editorPositionChanged();
	void textChanged();
};

#endif    // TEXT_UI_H
