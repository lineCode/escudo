/*
 * Copyright 2012, 2013 Esrille Inc.
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

#include "HTMLPreElementImp.h"

namespace org
{
namespace w3c
{
namespace dom
{
namespace bootstrap
{

void HTMLPreElementImp::eval()
{
    HTMLElementImp::evalWidth(this);
}

unsigned int HTMLPreElementImp::getWidth()
{
    // TODO: implement me!
    return 0;
}

void HTMLPreElementImp::setWidth(unsigned int width)
{
    // TODO: implement me!
}

}
}
}
}
