// Copyright (C) 2022 Emad
// 
// This file is part of sandikCpp.
// 
// sandikCpp is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// sandikCpp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with sandikCpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _COLLECTIONS_VEC_INCLUDE_VEC_
#define _COLLECTIONS_VEC_INCLUDE_VEC_

#include "Framework/Basics/BasicTypes.h"
#include "../../BasicVec/Include/BasicVec.h"
#include "Framework/STL/Iter/Inc"
#include "Framework/STL/Mem/Alloc/Inc"
#include "Framework/STL/IO/Serialization/Inc"

namespace stl { namespace collections {

    using namespace framework::basics;
    using namespace stl::mem::alloc;
    using namespace stl::iter;
    using namespace stl::io;
        
    const Size DEFAULT_ALLOCATION_SIZE = 32;
    #define create_proper_allocation_size_(size_)  DEFAULT_ALLOCATION_SIZE * (size_ / DEFAULT_ALLOCATION_SIZE + (size_ % DEFAULT_ALLOCATION_SIZE == 0 ? 0 : 1))

    #if __cplusplus > 201703L && __cpp_concepts >= 201907L
        template<Movable T, Allocator A = DefaultAllocator>
    #else
        template<
            typename T, 
            typename A = DefaultAllocator, std::enable_if_t<std::is_base_of_v<BasicAllocator, A>, bool> = true
        >
    #endif
    class Vec: public Mover, public CanBeCopied<EResult<Vec<T, A>>>, public Dropper, public Sized, public Iterator<T> {

      protected:
        typedef BasicVec<T, A> InnerVec;
        friend class String;

        InnerVec _inner;
        MSize    _len = 0;

        inline Vec(T* array, Size size, Size caps);

        inline Vec(InnerVec inner, Size len);

      public:
        inline Vec();

        inline ~Vec();

        Vec(Vec&& obj);

        static inline Fn create() -> Vec;

        static inline Fn with_capacity(Size cap) -> EResult<Vec>;

        static inline Fn from(Vec&& vector) -> Vec;

        inline Fn copy() -> EResult<Vec>;

        template <Iterable I> static inline Fn from(I&& obj) -> Vec;

        static inline Fn from_raw_ptr(Ptr ptr, Size size, Size cap) -> EResult<Vec>;

        inline Fn len() -> MSize;

        inline Fn capacity() -> MSize;

        inline Fn as_ptr() -> const T*;

        inline Fn push(T&& data) -> Void;

        inline Fn push(T* byt, Size size) -> Void;
        
        inline Fn push(Vec&& vec) -> Void;

        inline Fn pop() -> T;

        inline Fn pop_vec(Size len) -> Vec<T>;

        inline Fn get(Idx idx) -> T&;

        inline Fn get_vec(Idx start, Idx end) -> Vec<T>;

        inline Fn drop() -> Void override;

        Fn operator =(Vec&& obj) -> Void;

        Fn operator [](Idx idx) -> T&;

        // inline Fn next() -> Option<T> override;
        inline Fn next() -> T override;

        inline Fn begin() -> T* override;

        inline Fn end() -> T* override;

        inline Fn size() -> MSize override;

        Fn serialize(BasicSerializer& ser) -> Serialized<MUInt8*>;

        static Fn deserialize(BasicDeserializer& de) -> Vec;
    };


    /* Self Implementations For Vec<T, A> */

    template<Movable T, Allocator A> 
    inline Vec<T, A>::Vec(): 
        _inner(), 
        _len(0) 
    { /* nothing */ }


    template<Movable T, Allocator A> 
    inline Vec<T, A>::Vec(T* array, Size size, Size caps): _inner(array, caps) {
        this->_len = size;
    }


    template<Movable T, Allocator A> 
    inline Vec<T, A>::Vec(Vec&& obj) {
        this->_inner = moveObj(obj._inner);
        this->_len   = obj._len;

        obj.drop();
    }


    template<Movable T, Allocator A> 
    inline Vec<T, A>::Vec(Vec::InnerVec inner, Size len) {
        this->_inner = moveObj(inner);
        this->_len   = len;
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::create() -> Vec<T, A> {
        return Vec();
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::with_capacity(Size cap) -> EResult<Vec<T, A>> {
        Let res = BasicVec<T, A>::with_capacity(cap);
        unwrapResOrRet(
            res,
            resErr(Vec, EK::AllocationFailure),
            "Failed to create BasicVec with capacity of `%ld`", cap
        );
        return resOk(Vec, Vec(res.unwrap(), 0));
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::copy() -> EResult<Vec<T, A>> {
        Let res = this->_inner.copy();
        unwrapResOrRetSame(Vec, res, "Failed to copy from BasicVec to create new copy of Vec!");
        return EResult<Vec>::ok(Vec<T, A>(moveObj(res.unwrap()), this->len()));
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::from(Vec&& vector) -> Vec<T, A> {
        return Vec(moveObj(vector));
    }


    template<Movable T, Allocator A> 
    template<Iterable I> 
    inline Fn Vec<T, A>::from(I&& obj) -> Vec<T, A> {
        Let vec = Vec();
        Loop {
            Let val = obj.next();
            if (val.is_none()) {
                break;
            }

            vec.push(val.unwrap());
        }

        return vec;
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::from_raw_ptr(Ptr ptr, Size size, Size cap) -> EResult<Vec<T, A>> {
        Let bv = BasicVec<T, A>::from_raw_ptr(ptr, cap);
        return resOk(Vec, Vec(bv, size));
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::len() -> MSize {
        return this->_len;
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::capacity() -> MSize {
        return this->_inner.capacity();
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::as_ptr() -> const T* {
        return this->_inner.ptr();
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::push(T&& data) -> Void {
        if (this->_len >= this->_inner.capacity()) {
            this->_inner.reserve(create_proper_allocation_size_(1) + this->_inner.capacity());
        }

        this->_inner.ptr()[this->_len++] = moveObj(data);
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::push(Vec&& vec) -> Void {
        if (this->_len + vec.len() >= this->_inner.capacity()) {
            this->_inner.reserve(create_proper_allocation_size_(vec.len()) + this->_inner.capacity());
        }

        Let len = vec.len();
        for (MIdx i = 0; i < len; i++) {
            this->_inner.ptr()[this->_len++] = moveObj(vec[i]);
        }
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::push(T* byt, Size size) -> Void {
        if (this->_len + size >= this->_inner.capacity()) {
            this->_inner.reserve(create_proper_allocation_size_(size) + this->_inner.capacity());
        }

        // for (MSSize i = size - 1; i >= 0; i--) {
        for (MIdx i = 0; i < size; i++) {
            // TODO: change this to copy data
            this->_inner.ptr()[this->_len++] = moveObj(*(byt + i));
        }
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::pop() -> T {
        // Let to_be_popped = ;
        return this->_inner.ptr()[--this->_len];
    }


    // TODO: change this to copy data
    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::pop_vec(Size len) -> Vec<T> {
        Vec<T, A> vec = Vec<T, A>::create();
        for (MSize idx = 0; idx < len; idx++) {
            Let tmp = *(this->as_ptr() + (this->_len - len + idx));
            vec.push(moveObj(tmp));
            // if (!IsPrimitive<int>) {
            //     vec.push((*(this->as_ptr() + (this->_len - len + 1))).copy());
            // }
            // else {
            //     vec.push(moveObj(*(this->as_ptr() + (this->_len - len + 1))));
            // }
        }
        
        this->_len -= len;
        return vec;
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::get(Idx idx) -> T& {
        return this->_inner.ptr()[idx];
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::get_vec(Idx start, Idx end) -> Vec<T> {
        // Let vec = Vec<T, A>::with_capacity(end - start);
        // memcpy(this->_inner.ptr()[start], vec.as_ptr(), end - start);
        // return vec;
    }


    // template<Movable T, Allocator A> 
    // inline Fn Vec<T, A>::reserve(Size size) -> Void {
    //     Let vec = Vec<T, A>::with_capacity(end - start);
    //     memcpy(this->_inner.ptr()[start], vec.as_ptr(), end - start);
    //     return vec;
    // }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::drop() -> Void {
        this->_inner.drop();
        this->_len = 0;
    }


    template<Movable T, Allocator A> 
    inline Vec<T, A>::~Vec () {
        if (this->_len != 0) {
            // ...
        }
    }


    /* Iterator Implementations */

    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::next() -> T {
        // return Option<T>::None;
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::begin() -> T* {
        return this->_inner.ptr();
    }


    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::end() -> T* {
        return this->_inner.ptr() + this->_len;
    }



    /* Sized Implementations */

    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::size() -> MSize {
        return this->_inner.size() + sizeof(this->_len);
    }



    /* Operator Overloading */

    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::operator =(Vec&& obj) -> Void {
        this->_inner = moveObj(obj._inner);
        this->_len   = obj._len;

        obj.drop();
    }

    template<Movable T, Allocator A> 
    inline Fn Vec<T, A>::operator [](Idx idx) -> T& {
        return this->get(idx);
    }


    /* Serializable Implementation for String */

    template<Movable T, Allocator A> 
    Fn Vec<T, A>::serialize(BasicSerializer& ser) -> Serialized<MUInt8*> {
        for (MSSize idx = this->_len - 1; idx >= 0; idx--) {
            ser.serialize_elem<BasicSerializer>(this->_inner.ptr()[idx]);
        }
        
        ser.serialize_size(this->_len);
        return ser.finished();
    }


    /* Deserializable Implementation for String */

    template<Movable T, Allocator A> 
    Fn Vec<T, A>::deserialize(BasicDeserializer& de) -> Vec {
        Let len = de.deserialize_size();
        Let vec = Vec<T, A>::create();

        for (MIdx idx = 0; idx < len; idx++) {
            Let elem = de.deserialize<BasicDeserializer, T>();
            vec.push(moveObj(elem));
        }
        

        return moveObj(vec);
    }

    
} // namespace basics 
} // namespace framework




#endif /* _COLLECTIONS_VEC_INCLUDE_VEC_ */
