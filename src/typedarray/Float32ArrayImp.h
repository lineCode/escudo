// Generated by esidl 0.3.0.
// This file is expected to be modified for the Web IDL interface
// implementation.  Permission to use, copy, modify and distribute
// this file in any software license is hereby granted.

#ifndef ORG_W3C_DOM_BOOTSTRAP_FLOAT32ARRAYIMP_H_INCLUDED
#define ORG_W3C_DOM_BOOTSTRAP_FLOAT32ARRAYIMP_H_INCLUDED

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <org/w3c/dom/Float32Array.h>

#include <org/w3c/dom/ArrayBuffer.h>
#include <org/w3c/dom/Float32Array.h>

namespace org
{
namespace w3c
{
namespace dom
{
namespace bootstrap
{
class Float32ArrayImp : public ObjectMixin<Float32ArrayImp>
{
public:
    // Float32Array
    unsigned int getLength();
    float get(unsigned int index);
    void set(unsigned int index, float value);
    void set(Float32Array array);
    void set(Float32Array array, unsigned int offset);
    void set(ObjectArray<float> array);
    void set(ObjectArray<float> array, unsigned int offset);
    Float32Array subarray(int start, int end);
    // ArrayBufferView
    ArrayBuffer getBuffer();
    unsigned int getByteOffset();
    unsigned int getByteLength();
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv)
    {
        return Float32Array::dispatch(this, selector, id, argc, argv);
    }
    static const char* const getMetaData()
    {
        return Float32Array::getMetaData();
    }
};

}
}
}
}

#endif  // ORG_W3C_DOM_BOOTSTRAP_FLOAT32ARRAYIMP_H_INCLUDED
