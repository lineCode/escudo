/*
 * Copyright 2011-2013 Esrille Inc.
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

#ifndef ES_OBJECT_H
#define ES_OBJECT_H

#include <cstring>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <type_traits>

class Any;
class Imp;
class Object;

template <typename T>
class Nullable
{
    T    value_;
    bool hasValue_;

    template <typename U>
    Nullable(const Nullable<U>&) = delete;

public:
    Nullable() :
        hasValue_(false) {}
    Nullable(const Nullable<T>& nullable) :
        value_(nullable.value_), hasValue_(nullable.hasValue_) {}
    Nullable(Nullable<T>&& nullable) :
        value_(std::move(nullable.value_)), hasValue_(nullable.hasValue_) {}
    Nullable(const T& value) :
        value_(value), hasValue_(true) {}
    Nullable(T&& value) :
        value_(std::move(value)), hasValue_(true) {}
    Nullable(const Any& any);
    Nullable(Any&& any);
    template<typename U>
    Nullable(U* str);

    Nullable& operator=(const Nullable<T>& other) {
        value_ = other.value_;
        hasValue_ = other.hasValue_;
        return *this;
    }
    Nullable& operator=(Nullable<T>&& other) {
        value_ = std::move(other.value_);
        hasValue_ = other.hasValue_;
        return *this;
    }
    Nullable& operator=(const T& other) {
        value_ = other;
        hasValue_ = true;
        return *this;
    }
    Nullable& operator=(T&& other) {
        value_ = std::move(other);
        hasValue_ = true;
        return *this;
    }

    bool operator==(const Nullable<T>& other) const {
        if (!hasValue_)
            return !other.hasValue_;
        if (!other.hasValue_)
            return false;
        return value_ == other.value_;
    }
    bool operator!=(const Nullable<T>& other) const {
        return !(*this == other);
    }

    operator std::u16string() const;

    bool hasValue() const {
        return hasValue_;
    }

    T value() const {
        if (!hasValue_) {
            // TODO: Raise an exception
        }
        return value_;
    }
};

class Any
{
    unsigned type;
    union {
        int32_t              i32;
        uint32_t             u32;
        int64_t              i64;
        uint64_t             u64;
        float                f32;
        double               f64;
        std::u16string       string;
        std::shared_ptr<Imp> pimpl;
    };

    template<typename T>
    T cast(typename std::enable_if<std::is_same<bool, T>::value>::type* = 0) const {
        return toBoolean();
    }

    template<typename T>
    T cast(typename std::enable_if<std::is_same<std::u16string, T>::value>::type* = 0) const {
        return toString();
    }

    template<typename T>
    T cast(typename std::enable_if<std::is_base_of<Object, T>::value>::type* = 0) const {
        return toObject();
    }

    template<typename T>
    T cast(typename std::enable_if<std::is_arithmetic<T>::value>::type* = 0,
           typename std::enable_if<!std::is_same<bool, T>::value>::type* = 0) const {
        switch (type) {
        case Bool:
        case Int32:
            return static_cast<T>(i32);
        case Uint32:
            return static_cast<T>(u32);
        case Int64:
            return static_cast<T>(i64);
        case Uint64:
            return static_cast<T>(u64);
        case Float32:
            return static_cast<T>(f32);
        case Float64:
            return static_cast<T>(f64);
        default:
            return static_cast<T>(toNumber());
        }
    }

    void destruct() {
        switch (type) {
        case String:
            string.~basic_string<char16_t>();
            break;
        case Shared:
            pimpl.~shared_ptr<Imp>();
            break;
        default:
            break;
        }
    }

public:
    enum {
        Undefined,
        Null,
        Bool,
        Int32,
        Uint32,
        Int64,
        Uint64,
        Float32,
        Float64,
        String,
        Shared,   // Object
    };

    Any() :
        type(Undefined) {}
    Any(const Any& value) :
        type{value.type} {
        switch (type) {
        case Bool:
            i32 = value.i32;
            break;
        case Int32:
            i32 = value.i32;
            break;
        case Uint32:
            u32 = value.u32;
            break;
        case Int64:
            i64 = value.i64;
            break;
        case Uint64:
            u64 = value.u64;
            break;
        case Float32:
            f32 = value.f32;
            break;
        case Float64:
            f64 = value.f64;
            break;
        case String:
            new(&string) std::u16string(value.string);
            break;
        case Shared:
            new(&pimpl) std::shared_ptr<Imp>(value.pimpl);
            break;
        default:
            break;
        }
    }
    Any(Any&& value) :
        type{value.type} {
        switch (type) {
        case Bool:
            i32 = value.i32;
            break;
        case Int32:
            i32 = value.i32;
            break;
        case Uint32:
            u32 = value.u32;
            break;
        case Int64:
            i64 = value.i64;
            break;
        case Uint64:
            u64 = value.u64;
            break;
        case Float32:
            f32 = value.f32;
            break;
        case Float64:
            f64 = value.f64;
            break;
        case String:
            new(&string) std::u16string(std::move(value.string));
            break;
        case Shared:
            new(&pimpl) std::shared_ptr<Imp>(std::move(value.pimpl));
            break;
        default:
            break;
        }
    }
    Any(std::nullptr_t) :
        type(Null) {}
    Any(bool value) :
        type{Bool},
        i32{value} {}
    Any(unsigned char value) :
        type{Uint32},
        u32{value} {}
    Any(signed char value) :
        type{Int32},
        i32{value} {}
    Any(unsigned short value) :
        type{Uint32},
        u32{value} {}
    Any(signed short value) :
        type{Int32},
        i32{value} {}
    Any(unsigned int value) :
        type{Uint32},
        u32{value} {}
    Any(signed int value) :
        type{Int32},
        i32{value} {}
    Any(unsigned long value) :
        type{Uint64},
        u64{value} {}
    Any(signed long value) :
        type{Int64},
        i64{value} {}
    Any(unsigned long long value) :
        type{Uint64},
        u64{value} {}
    Any(signed long long value) :
        type{Int64},
        i64{value} {}
    Any(float value) :
        type{Float32},
        f32{value} {}
    Any(double value) :
        type{Float64},
        f64{value} {}
    Any(const std::u16string& value) :
        type{String} {
        new(&string) std::u16string(value);
    }
    Any(std::u16string&& value) :
        type{String} {
        new(&string) std::u16string(std::move(value));
    }
    Any(const char16_t* value) :
        type{String} {
        new(&string) std::u16string(value);
    }
    Any(const Object& value);
    Any(Object&& value);
    Any(const std::shared_ptr<Imp>& value) :
        type{Shared} {
        new(&pimpl) std::shared_ptr<Imp>(value);
    }
    Any(std::shared_ptr<Imp>&& value) :
        type{Shared} {
        new(&pimpl) std::shared_ptr<Imp>(std::move(value));
    }
    template <class IMP>
    Any(const std::shared_ptr<IMP>& value) :
        type{Shared} {
        new(&pimpl) std::shared_ptr<Imp>(std::static_pointer_cast<Imp>(value));
    }
    template <class IMP>
    Any(std::shared_ptr<IMP>&& value) :
        type{Shared} {
        new(&pimpl) std::shared_ptr<Imp>(std::move(std::static_pointer_cast<Imp>(value)));
    }
    template <typename T>
    Any(const Nullable<T>& nullable) :
        type{Null} {
        if (nullable.hasValue())
            operator=(nullable.value());
    }
    template <typename T>
    Any(Nullable<T>&& nullable) :
        type{Null} {
        if (nullable.hasValue())
            operator=(std::move(nullable.value()));
    }

    ~Any() {
        destruct();
    }

    Any& operator=(const Any& value) {
        destruct();
        type = value.type;
        switch (type) {
        case Bool:
            i32 = value.i32;
            break;
        case Int32:
            i32 = value.i32;
            break;
        case Uint32:
            u32 = value.u32;
            break;
        case Int64:
            i64 = value.i64;
            break;
        case Uint64:
            u64 = value.u64;
            break;
        case Float32:
            f32 = value.f32;
            break;
        case Float64:
            f64 = value.f64;
            break;
        case String:
            new(&string) std::u16string(value.string);
            break;
        case Shared:
            new(&pimpl) std::shared_ptr<Imp>(value.pimpl);
            break;
        default:
            break;
        }
        return *this;
    }
    Any& operator=(Any&& value) {
        destruct();
        type = value.type;
        switch (type) {
        case Bool:
            i32 = value.i32;
            break;
        case Int32:
            i32 = value.i32;
            break;
        case Uint32:
            u32 = value.u32;
            break;
        case Int64:
            i64 = value.i64;
            break;
        case Uint64:
            u64 = value.u64;
            break;
        case Float32:
            f32 = value.f32;
            break;
        case Float64:
            f64 = value.f64;
            break;
        case String:
            new(&string) std::u16string(std::move(value.string));
            break;
        case Shared:
            new(&pimpl) std::shared_ptr<Imp>(std::move(value.pimpl));
            break;
        default:
            break;
        }
        return *this;
    }
    Any& operator=(std::nullptr_t) {
        destruct();
        type = Null;
        return *this;
    }
    Any& operator=(bool value) {
        destruct();
        type = Bool;
        i32 = value;
        return *this;
    }
    Any& operator=(unsigned char value) {
        destruct();
        type = Uint32;
        u32 = value;
        return *this;
    }
    Any& operator=(signed char value) {
        destruct();
        type = Int32;
        i32 = value;
        return *this;
    }
    Any& operator=(unsigned short value) {
        destruct();
        type = Uint32;
        u32 = value;
        return *this;
    }
    Any& operator=(signed short value) {
        destruct();
        type = Int32;
        i32 = value;
        return *this;
    }
    Any& operator=(unsigned int value) {
        destruct();
        type = Uint32;
        u32 = value;
        return *this;
    }
    Any& operator=(signed int value) {
        destruct();
        type = Int32;
        i32 = value;
        return *this;
    }
    Any& operator=(unsigned long value) {
        destruct();
        type = Uint64;
        u64 = value;
        return *this;
    }
    Any& operator=(signed long value) {
        destruct();
        type = Int64;
        i64 = value;
        return *this;
    }
    Any& operator=(unsigned long long value) {
        destruct();
        type = Uint64;
        u64 = value;
        return *this;
    }
    Any& operator=(signed long long value) {
        destruct();
        type = Int64;
        i64 = value;
        return *this;
    }
    Any& operator=(float value) {
        destruct();
        type = Float32;
        f32 = value;
        return *this;
    }
    Any& operator=(double value) {
        destruct();
        type = Float64;
        f64 = value;
        return *this;
    }
    Any& operator=(const std::u16string& value) {
        if (type == String)
            string = value;
        else {
            destruct();
            type = String;
            new(&string) std::u16string(value);
        }
        return *this;
    }
    Any& operator=(std::u16string&& value) {
        if (type == String)
            string = value;
        else {
            destruct();
            type = String;
            new(&string) std::u16string(std::move(value));
        }
        return *this;
    }
    Any& operator=(const char16_t* value) {
        if (type == String)
            string = value;
        else {
            destruct();
            type = String;
            new(&string) std::u16string(value);
        }
        return *this;
    }
    Any& operator=(const Object& value);
    Any& operator=(Object&& value);
    template <typename T>
    Any& operator=(const Nullable<T>& nullable) {
        if (nullable.hasValue())
            return operator=(nullable.value());
        destruct();
        type = Null;
        return *this;
    }
    template <typename T>
    Any& operator=(Nullable<T>&& nullable) {
        if (nullable.hasValue())
            return operator=(std::move(nullable.value()));
        destruct();
        type = Null;
        return *this;
    }

    template<typename T>
    T as() const {
        return cast<T>();
    }
    template<typename T>
    operator T() const {
        return cast<T>();
    }

    unsigned getType() const {
        return type;
    }
    bool isPrimitive() const {
        switch (type) {
        case Bool:
        case Int32:
        case Uint32:
        case Int64:
        case Uint64:
        case Float32:
        case Float64:
            return true;
        default:
            return false;
        }
    }
    bool isObject() const {
        return type == Shared;
    }
    bool isString() const {
        return type == String;
    }
    bool isUndefined() const {
        return type == Undefined;
    }
    bool isNull() const {
        return type == Null;
    }

    bool toBoolean() const;
    double toNumber() const;
    std::u16string toString() const;

    std::shared_ptr<Imp> toObject() const {
        return isObject() ? pimpl : nullptr;
    }

    unsigned int toArrayIndex() const;  // cf. [ECMA-262], section 15.4
};

class Imp : public std::enable_shared_from_this<Imp>
{
public:
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return nullptr;
    }
    // Note this->self() cannot be used inside the constructor.
    std::shared_ptr<Imp> self() {
        return shared_from_this();
    }
};

template<class O> class Weak
{
    std::weak_ptr<Imp> pimpl;
public:
    Weak() = default;
    Weak(std::nullptr_t) {}
    Weak(O const& other) : pimpl(other.self()) {}
    Weak& operator=(O const& other) {
        pimpl = other.self();
        return *this;
    }

    void reset() {
        pimpl.reset();
    }
    bool expired() const {
        return pimpl.expired();
    }
    O lock() const {
        return O(pimpl.lock());
    }
};

class Object
{
protected:
    std::shared_ptr<Imp> pimpl;

public:
    Object() = default;
    Object(const Object& object) = default;
    Object(Object&& object) = default;
    Object& operator=(const Object& other) = default;
    Object& operator=(Object&& other) = default;
    ~Object() = default;

    Object(const Object* object) {
        if (object && object->pimpl)
            pimpl = object->pimpl;
    }
    explicit Object(Imp* pimpl) : pimpl(pimpl) {}
    Object(std::nullptr_t) {}

    template <class IMP>
    Object(const std::shared_ptr<IMP>& pimpl) : pimpl(std::static_pointer_cast<Imp>(pimpl)) {}
    Object(const std::shared_ptr<Imp>& pimpl) : pimpl(pimpl) {}
    template <class IMP>
    Object(std::shared_ptr<IMP>&& pimpl) : pimpl(std::static_pointer_cast<Imp>(pimpl)) {}
    Object(std::shared_ptr<Imp>&& pimpl) : pimpl(pimpl) {}

    template <class IMP>
    Object& operator=(const std::shared_ptr<IMP>& other) {
        pimpl = other;
        return *this;
    }
    Object& operator=(const std::shared_ptr<Imp>& other) {
        pimpl = other;
        return *this;
    }
    template <class IMP>
    Object& operator=(std::shared_ptr<IMP>&& other) {
        pimpl = other;
        return *this;
    }
    Object& operator=(std::shared_ptr<Imp>&& other) {
        pimpl = other;
        return *this;
    }

    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        if (pimpl)
            return pimpl->message_(selector, id, argc, argv);
        return Any();
    }
    bool operator!() const {
        return !pimpl;
    }
    explicit operator bool() const {
        return pimpl.get();
    }
    const std::shared_ptr<Imp>& self() const {
        return pimpl;
    }

    bool operator==(const Object& other) const
    {
        return self() == other.self();
    }
    bool operator!=(const Object& other) const
    {
        return self() != other.self();
    }
    bool operator<(const Object& other) const
    {
        return self() < other.self();
    }

    // Predefined argument count
    static const int GETTER_ = -1;
    static const int SETTER_ = -2;
    static const int SPECIAL_GETTER_ = -3;
    static const int SPECIAL_DELETER_ = -4;
    static const int SPECIAL_SETTER_ = -5;
    static const int SPECIAL_CREATOR_ = -6;
    static const int SPECIAL_SETTER_CREATOR_ = -7;
    static const int HAS_PROPERTY_ = -8;
    static const int HAS_OPERATION_ = -9;
    static const int IS_KIND_OF_ = -10;
    static const int STRINGIFY_ = -11;
    static const int CALLBACK_ = 0x4000000;

    // Max # of variadic arguments
    static const int MAX_VARIADIC_ = 16;
};

template <typename X>
X interface_cast(const Object& object)
{
    return X(object.self());
}

template <typename X>
X interface_cast(const std::shared_ptr<Imp>& pimpl)
{
    return X(pimpl);
}

class ObjectImp : public Imp
{
    void* privateDate;

public:
    ObjectImp() : privateDate(nullptr) {}
    ObjectImp(const ObjectImp& org) : privateDate(nullptr) {}
    void* getPrivate() const {
        return privateDate;
    }
    void setPrivate(void* data) {
        privateDate = data;
    }
    virtual void* getStaticPrivate() const = 0;
};

template <typename T, typename B = ObjectImp>
class ObjectMixin : public B
{
    static void* staticPrivate;
protected:
    ObjectMixin() :
        B()
    {
    }

    template<class... As>
    ObjectMixin(As&&... as) :
        B(std::forward<As>(as)...)
    {
    }

public:
    virtual void* getStaticPrivate() const {
        return staticPrivate;
    }

    static void setStaticPrivate(void* p) {
        staticPrivate = p;
    }
};

template <typename T, typename B>
void* ObjectMixin<T, B>::staticPrivate = 0;

template <typename IMP>
class Retained : public std::shared_ptr<IMP>
{
public:
    Retained() :
        std::shared_ptr<IMP>(std::make_shared<IMP>())
    {
        static_assert(std::is_base_of<Imp, IMP>::value, "not Imp");
    }
    template<class... As>
    Retained(As&&... as) :
        std::shared_ptr<IMP>(std::make_shared<IMP>(std::forward<As>(as)...))
    {
        static_assert(std::is_base_of<Imp, IMP>::value, "not Imp");
    }
};

template <typename T>
class Variadic
{
    std::initializer_list<T> list;
    const Any* variadic;
    size_t length;

public:
    Variadic() :
        variadic(0),
        length(0) {}
    Variadic(const Any* variadic, size_t length) :
        variadic(length ? variadic : 0),
        length(length) {}
    Variadic(std::initializer_list<T> list) :
        list(list),
        variadic(0),
        length(list.size()) {}

    T operator[](unsigned int index) const {
        if (!variadic)
            return *(list.begin() + index);
        return variadic[index];
    }

    size_t size() const {
        return length;
    }
};

//
// Nullable
//

template <typename T>
Nullable<T>::Nullable(const Any& any)
{
    hasValue_ = !any.isNull();
    if (hasValue_)
        value_ = any.as<T>();
}

template <typename T>
Nullable<T>::Nullable(Any&& any)
{
    hasValue_ = !any.isNull();
    if (hasValue_)
        value_ = std::move(any.as<T>());
}

template<>
template<>
inline Nullable<std::u16string>::Nullable(const char16_t* str)
{
    if (!str)
        hasValue_ = false;
    else {
        hasValue_ = true;
        value_ = str;
    }
}

// Note maybe it is a bit arguable whether we should have this
// conversion operator. The primary use case of this is to call
// ElementImp::getAttribute from Element's subclasses with a
// specific qualified name.
template<>
inline Nullable<std::u16string>::operator std::u16string() const
{
    if (hasValue_)
        return value_;
    else
        return u"";
}

//
// Any
//

inline Any::Any(const Object& value) :
    type{Shared}
{
    new(&pimpl) std::shared_ptr<Imp>(value.self());
}

inline Any::Any(Object&& value) :
    type{Shared}
{
    new(&pimpl) std::shared_ptr<Imp>(std::move(value.self()));
}

inline Any& Any::operator=(const Object& value)
{
    if (type == Shared && value)
        pimpl = value.self();
    else {
        destruct();
        if (!value)
            type = Null;
        else {
            type = Shared;
            new(&pimpl) std::shared_ptr<Imp>(value.self());
        }
    }
    return *this;
}

inline Any& Any::operator=(Object&& value)
{
    if (type == Shared && value)
        pimpl = std::move(value.self());
    else {
        destruct();
        if (!value)
            type = Null;
        else {
            type = Shared;
            new(&pimpl) std::shared_ptr<Imp>(std::move(value.self()));
        }
    }
    return *this;
}

#endif  // ES_OBJECT_H
