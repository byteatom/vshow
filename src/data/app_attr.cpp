#include "app_attr.h"

AppAttr::AppAttr() : tick{5}
{

}

AppAttr::~AppAttr()
{

}

void AppAttr::load()
{
    Xml::Doc *doc = xml.load(L"app.xml");
    if (!doc) return;
    Xml::Walker *walker = doc->createTreeWalker(
                doc->getDocumentElement(),
                XmlDom::walkType,
                nullptr,
                true);
    xml_release_guard(Xml::Walker, walker);
    Xml::Node *node;
    while (nullptr != (node = walker->nextNode())) {
        if (node->getNodeType() == Xml::DOMNode::ELEMENT_NODE) {
            std::wstring name{node->getNodeName()};
            if (name == L"project")
                projectFile = std::wstring(node->getTextContent());
            else if (name == L"tick")
                tick = std::stoi(node->getTextContent());
            else if (name==L"language")
                language = std::wstring(node->getTextContent());

        }
    }
}

void AppAttr::save()
{
    Xml::Doc *doc = xml.create(L"app");
    Xml::Element *root = doc->getDocumentElement();
    root->appendChild(doc->createElement(L"project"))
            ->appendChild(doc->createTextNode(projectFile.c_str()));
    root->appendChild(doc->createElement(L"tick"))
            ->appendChild(doc->createTextNode(std::to_wstring(tick).c_str()));
    root->appendChild(doc->createElement(L"language"))
            ->appendChild(doc->createTextNode(language.c_str()));
    xml.save(L"app.xml");
}
