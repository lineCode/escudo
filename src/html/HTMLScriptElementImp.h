/*
 * Copyright 2010-2013 Esrille Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ORG_W3C_DOM_BOOTSTRAP_HTMLSCRIPTELEMENTIMP_H_INCLUDED
#define ORG_W3C_DOM_BOOTSTRAP_HTMLSCRIPTELEMENTIMP_H_INCLUDED

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <org/w3c/dom/html/HTMLScriptElement.h>

#include "HTMLElementImp.h"
#include "http/HTTPRequest.h"

namespace org { namespace w3c { namespace dom { namespace bootstrap {

class HTMLScriptElementImp : public ObjectMixin<HTMLScriptElementImp, HTMLElementImp>
{
    // type values
    enum {
        Blocking,
        Defer,
        Async,
        Ordered
    };

    Retained<EventListenerImp> mutationListener;

    bool alreadyStarted;
    bool parserInserted;
    bool wasParserInserted;
    bool forceAsync;
    bool readyToBeParserExecuted;
    HttpRequestPtr request;
    unsigned type;

    void handleMutation(EventListenerImp* listener, events::Event event);

public:
    HTMLScriptElementImp(DocumentImp* ownerDocument, const std::u16string& localName = u"script");
    HTMLScriptElementImp(const HTMLScriptElementImp& org);

    void markAsParserInserted() {
        parserInserted = true;
        forceAsync = false;
    }
    bool isReadyToBeParserExecuted() const {
        return readyToBeParserExecuted;
    }

    bool execute();
    bool prepare();
    void notify();

    // Node - override
    virtual Node cloneNode(bool deep = true) {
        auto node = std::make_shared<HTMLScriptElementImp>(*this);
        node->cloneAttributes(this);
        if (deep)
            node->cloneChildren(this);
        return node;
    }

    // HTMLScriptElement
    virtual std::u16string getSrc();
    virtual void setSrc(const std::u16string& src);
    virtual bool getAsync();
    virtual void setAsync(bool async);
    virtual bool getDefer();
    virtual void setDefer(bool defer);
    virtual std::u16string getType();
    virtual void setType(const std::u16string& type);
    virtual std::u16string getCharset();
    virtual void setCharset(const std::u16string& charset);
    virtual std::u16string getText();
    virtual void setText(const std::u16string& text);
    // HTMLScriptElement-29
    virtual std::u16string getEvent();
    virtual void setEvent(const std::u16string& event);
    virtual std::u16string getHtmlFor();
    virtual void setHtmlFor(const std::u16string& htmlFor);

    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return html::HTMLScriptElement::dispatch(this, selector, id, argc, argv);
    }
    static const char* const getMetaData()
    {
        return html::HTMLScriptElement::getMetaData();
    }
};

typedef std::shared_ptr<HTMLScriptElementImp> HTMLScriptElementPtr;

}}}}  // org::w3c::dom::bootstrap

#endif  // ORG_W3C_DOM_BOOTSTRAP_HTMLSCRIPTELEMENTIMP_H_INCLUDED
