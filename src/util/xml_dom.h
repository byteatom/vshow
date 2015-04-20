#ifndef XMLDOM_H
#define XMLDOM_H

#include <string>
#include <memory>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

class XmlStr {
public:
    XmlStr(const char* const str);
    XmlStr(const XMLCh* const str);
    ~XmlStr();

    operator XMLCh*() const;
    operator char*() const;
private:
    XMLCh* xml;
    bool freeXml;
    char* native;
    bool freeNative;
};

class XmlDomErrorHandler : public xercesc::DOMErrorHandler
{
public:
    XmlDomErrorHandler(){}
    ~XmlDomErrorHandler(){}

    bool handleError(const xercesc::DOMError& error);

private :
    /* Unimplemented constructors and operators */
    XmlDomErrorHandler(const DOMErrorHandler&);
    void operator=(const DOMErrorHandler&);
};

template <typename T> struct Releaser {
    void operator()(T *v) {
        v->release();
    }
};

#define xml_release_guard(type, var) \
    std::unique_ptr<type, Releaser<type>> var##__LINE__{(var)}

namespace Xml {
XERCES_CPP_NAMESPACE_USE
typedef xercesc::DOMNode Node;
typedef xercesc::DOMDocument Doc;
typedef xercesc::DOMElement Element;
typedef xercesc::DOMAttr Attr;
typedef xercesc::DOMText Text;
typedef xercesc::DOMTreeWalker Walker;
typedef xercesc::DOMCDATASection CData;
}

class XmlDom
{
public:
    static const int walkType =
            Xml::DOMNodeFilter::SHOW_ELEMENT
            | Xml::DOMNodeFilter::SHOW_ATTRIBUTE
            | Xml::DOMNodeFilter::SHOW_TEXT
            | Xml::DOMNodeFilter::SHOW_CDATA_SECTION;

    XmlDom();
    ~XmlDom();

    Xml::Doc* load(const XMLCh *file);
    Xml::Doc* create(const XMLCh *name);
    bool save(const XMLCh *file);
private:
    Xml::Doc *doc;
    XmlDomErrorHandler errorHandler;
};



#endif // XMLDOM_H
