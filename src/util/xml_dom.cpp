#include "xml_dom.h"

#include <cassert>
#include <fstream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

XERCES_CPP_NAMESPACE_USE

XmlStr::XmlStr(const char* const str)
{
    native = const_cast<char*>(str);
    freeNative = false;
    xml = XMLString::transcode(str);
    freeXml = true;
}

XmlStr::XmlStr(const XMLCh* const str)
{
    native = XMLString::transcode(str);
    freeNative = true;
    xml = const_cast<XMLCh*>(str);
    freeXml = false;
}

XmlStr::~XmlStr()
{
    if (freeXml) XMLString::release(&xml);
    if (freeNative) XMLString::release(&native);
}

XmlStr::operator XMLCh*() const
{
    return xml;
}

XmlStr::operator char*() const
{
    return native;
}

bool XmlDomErrorHandler::handleError(const DOMError &error)
{
    char *msg = XMLString::transcode(error.getMessage());
    XMLString::release(&msg);

    return false;
}

XmlDom::XmlDom() : doc{ nullptr }
{
    XMLPlatformUtils::Initialize();
}

XmlDom::~XmlDom()
{
    XMLPlatformUtils::Terminate();
}

Xml::Doc* XmlDom::load(const XMLCh *file)
{
    {
        std::ifstream ifs((const char*)XmlStr(file));
        if (!ifs.good()) return nullptr;
    }
    try {
        DOMImplementationLS *lsImpl = DOMImplementationRegistry::getDOMImplementation(XmlStr("LS"));
        DOMLSParser* parser = lsImpl->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
        xml_release_guard(DOMLSParser, parser);
        parser->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler, &errorHandler);
        Xml::Doc *docTmp = parser->parseURI(file);
        if (nullptr == docTmp) return nullptr;
        doc = static_cast<Xml::Doc *>(docTmp->cloneNode(true));
        return doc;
    } catch (...) {
        return nullptr;
    }
}

Xml::Doc* XmlDom::create(const XMLCh *name)
{
    if (doc)
        doc->release();
    DOMImplementation *coreImpl = DOMImplementationRegistry::getDOMImplementation(XmlStr("Core"));
    doc = coreImpl->createDocument(0, name, 0);
    return doc;
}

bool XmlDom::save(const XMLCh *file)
{
    assert(doc);

    DOMImplementationLS *lsImpl = DOMImplementationRegistry::getDOMImplementation(XmlStr("LS"));
    DOMLSSerializer* serializer = lsImpl->createLSSerializer();
    xml_release_guard(DOMLSSerializer, serializer);
    DOMConfiguration* config = serializer->getDomConfig();
    config->setParameter(XMLUni::fgDOMErrorHandler, &errorHandler);
    if (config->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
        config->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    if (config->canSetParameter(XMLUni::fgDOMWRTBOM, true))
        config->setParameter(XMLUni::fgDOMWRTBOM, true);
    std::unique_ptr<XMLFormatTarget> target = std::make_unique<LocalFileFormatTarget>(file);
    if (!target) return false;
    DOMLSOutput* output = lsImpl->createLSOutput();
    xml_release_guard(DOMLSOutput, output);
    output->setByteStream(target.get());
    serializer->write(doc, output);
    xml_release_guard(Xml::Doc, doc);
    doc = nullptr;
    return true;
}

