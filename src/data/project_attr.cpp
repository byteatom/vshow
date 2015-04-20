#include "project_attr.h"

#include "data.h"
#include "screen_attr.h"

ProjectAttr::ProjectAttr() :
	SliceAttr(nullptr)
{
	type = PROJECT_SLICE;
	name = L"project";
}

ProjectAttr::~ProjectAttr()
{
}

void ProjectAttr::load()
{
	Xml::Doc *doc = xml.load(Data::obj->appAttr->projectFile.c_str());
	if (nullptr == doc) return;
	load(doc->getDocumentElement());
}

void ProjectAttr::load(std::wstring file)
{
	Xml::Doc *doc = xml.load(file.c_str());
	if (nullptr == doc) return;
	load(doc->getDocumentElement());
}

void ProjectAttr::load(Xml::Element *element)
{
	for (Xml::Element *child = element->getFirstElementChild();
		 nullptr != child;
		 child = child->getNextElementSibling()) {
		std::wstring tag{child->getTagName()};
		if (tag == L"screen") {
			ScreenAttr *attr = new ScreenAttr(this);
			attr->load(child);
		}
	}
}

void ProjectAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
	if (Data::obj->appAttr->projectFile.empty()) return;

	doc = xml.create(L"project");
	parent = doc->getDocumentElement();
	SliceAttr::saveOrgans(doc, parent);
	xml.save(Data::obj->appAttr->projectFile.c_str());
}

ScreenAttr* ProjectAttr::getScreen(std::wstring &screenName)
{
	for (auto organ: organs) {
		ScreenAttr *screenAttr = static_cast<ScreenAttr *>(organ);
		if (screenName == screenAttr->name.value) {
			return screenAttr;
		}
	}
	return nullptr;
}
