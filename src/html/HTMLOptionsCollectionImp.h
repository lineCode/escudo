/*
 * Copyright 2013 Esrille Inc.
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

#ifndef ORG_W3C_DOM_BOOTSTRAP_HTMLOPTIONSCOLLECTIONIMP_H_INCLUDED
#define ORG_W3C_DOM_BOOTSTRAP_HTMLOPTIONSCOLLECTIONIMP_H_INCLUDED

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <org/w3c/dom/html/HTMLOptionsCollection.h>
#include "HTMLCollectionImp.h"

#include <org/w3c/dom/html/HTMLCollection.h>
#include <org/w3c/dom/html/HTMLElement.h>
#include <org/w3c/dom/html/HTMLOptGroupElement.h>
#include <org/w3c/dom/html/HTMLOptionElement.h>

namespace org
{
namespace w3c
{
namespace dom
{
namespace bootstrap
{

class HTMLSelectElementImp;
typedef std::shared_ptr<HTMLSelectElementImp> HTMLSelectElementPtr;

class HTMLOptionsCollectionImp : public ObjectMixin<HTMLOptionsCollectionImp, HTMLCollectionImp>
{
    std::weak_ptr<HTMLSelectElementImp> select;

public:
    HTMLOptionsCollectionImp(const HTMLSelectElementPtr& select);

    // HTMLCollection
    virtual Element item(unsigned int index);

    // HTMLOptionsCollection
    virtual unsigned int getLength();
    void setLength(unsigned int length);
    virtual Object namedItem(const std::u16string& name);
    void setElement(unsigned int index, html::HTMLOptionElement option);
    void add(Any element);
    void add(Any element, Any before);
    void remove(int index);
    int getSelectedIndex();
    void setSelectedIndex(int selectedIndex);
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv)
    {
        return html::HTMLOptionsCollection::dispatch(this, selector, id, argc, argv);
    }
    static const char* const getMetaData()
    {
        return html::HTMLOptionsCollection::getMetaData();
    }
};

}
}
}
}

#endif  // ORG_W3C_DOM_BOOTSTRAP_HTMLOPTIONSCOLLECTIONIMP_H_INCLUDED
