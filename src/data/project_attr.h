#ifndef PROJECTATTR_H
#define PROJECTATTR_H

#include "slice_attr.h"
#include "util/xml_dom.h"

class ScreenAttr;
class ProjectAttr : public SliceAttr
{
public:
	ProjectAttr();
	virtual ~ProjectAttr();

	void load();
	void load(std::wstring file);
	virtual void load(Xml::Element *element) override;
	virtual void save(Xml::Doc *doc, Xml::Node *parent) override;
	ScreenAttr* getScreen(std::wstring &screenName);

private:
	XmlDom xml;
};

#endif // PROJECTATTR_H
