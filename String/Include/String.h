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

#ifndef _STL_STRING_INCLUDE_STRING_
#define _STL_STRING_INCLUDE_STRING_

#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/OwnerShip.h"
#include "Framework/STL/Iter/Include/Iterator.h"
#include "Framework/STL/Collections/Vec/Include/Vec.h"
#include "Framework/STL/Mem/Sized/Include/Sized.h"

namespace stl {

    using namespace framework::basics;
    using namespace stl::iter;
    using namespace stl::mem;
    using namespace stl::collections;

    class String: 
        public Mover, 
        public CanBeCopied<String>, 
        public Dropper, 
        public Sized, 
        public Iterator<MChar>
    {
      private:
        Vec<MChar> _inner;

        inline String(Vec<MChar>&& vec);

      public:
        inline String() { };

        inline String(String&& obj);

        inline String(MChar* ptr, Size len, Size cap);

        inline ~String();

        static inline Fn create() -> String;

        static inline Fn from(Str str) -> String;

        static inline Fn from(String&& string) -> String;

        template <Iterable I> static inline Fn from(I&& obj) -> String;

        static inline Fn from_raw_ptr(MChar* ptr, Size len, Size cap) -> String;

        inline Fn as_str() -> Str;

        inline Fn to_vec() -> Vec<MChar>;

        inline Fn copy() -> String;

        inline Fn len() -> MSize const;

        inline Fn push(MChar ch) -> Void;

        inline Fn pop() -> MChar;

        inline Fn operator =(String&& obj) -> Void;

        inline Fn operator &() -> Str;

        inline Fn operator +(String&& obj) -> String;

        // inline Fn next() -> Option<MChar> override;
        inline Fn next() -> MChar override;

        inline Fn begin() -> MChar* override;

        inline Fn end() -> MChar* override;

        inline Fn size() -> MSize override;

        inline Fn drop() -> Void override;

        Fn serialize(BasicSerializer& ser) -> Serialized<MUInt8*> {
            ser.serialize_bytes(this->_inner.as_ptr(), this->len());
            ser.serialize_size(this->len());
            return ser.finished();
        }

        static Fn deserialize(BasicDeserializer& de) -> String {
            Let len = de.deserialize_size();

            Let ptr = DefaultAllocator().allocate(len, 1).unwrap();
            de.deserialize_bytes((MBytes) ptr, len);

            Let new_string = String::from_raw_ptr((MChar*) ptr, len, len);

            DefaultAllocator().deallocate(ptr);
            return new_string;
        }
    };



    /* Self Implementations for String */

    // inline String::String() {
    //     // this->_inner = Vec<MChar>::create();
    // }

    
    inline String::String(String&& obj) {
        this->_inner = moveObj(obj._inner);
        obj.drop();
    }

    
    inline String::String(Vec<MChar>&& vec) {
        this->_inner = moveObj(vec);
    }

    
    inline String::String(MChar* ptr, Size len, Size cap) {
        this->_inner = moveObj(Vec<MChar>::from_raw_ptr((Ptr) ptr, len, cap)).unwrap();
    }

    
    inline Fn String::create() -> String {
        return String();
    }


    inline Fn String::copy() -> String {
        return String(this->_inner.copy().unwrap());
    }

    
    inline Fn String::from(Str str) -> String {
        Let len = strlen(str);
        // Let vec = Vec<MChar>::from((Ptr) str, len, len);
        // return String(moveObj(vec));
    }

    
    inline Fn String::from(String&& vector) -> String {
        return String(moveObj(vector));
    }

    
    // TODO: need refactor
    template<Iterable I> 
    inline Fn String::from(I&& obj) -> String {
        return String::from(moveObj(Vec<MChar>::from(moveObj(obj))));
    }

    
    inline Fn String::from_raw_ptr(MChar* ptr, Size len, Size cap) -> String {
        return String(ptr, len, cap);
    }

    
    inline Fn String::as_str() -> Str {
        return &self;
    }

    
    inline Fn String::to_vec() -> Vec<MChar> {
        return moveObj(this->_inner);
    }

    
    inline Fn String::len() -> MSize const {
        return this->_inner.len();
    }

    
    inline Fn String::push(MChar data) -> Void {
        this->_inner.push(moveObj(data));
    }

    
    inline Fn String::pop() -> MChar {
        return this->_inner.pop();
    }


    inline Fn String::drop() -> Void {
        this->_inner.drop();
    }


    inline String::~String() {
        if (!this->dropped) {
            // ...
        }
    }



    /* Iterator Implementations for String */
    
    inline Fn String::next() -> MChar {
        // return Option<MChar>::None;
    }


    inline Fn String::begin() -> MChar* {
        return this->_inner.begin();
    }

    
    inline Fn String::end() -> MChar* {
        return this->_inner.end();
    }



    /* Sized Implementation for String */

    inline Fn String::size() -> MSize {
        return this->_inner.size();
    }



    /* Operator Overloading */

    inline Fn String::operator =(String&& obj) -> Void {
        this->_inner = moveObj(obj._inner);
        obj.drop();
    }


    inline Fn String::operator &() -> Str {
        return this->_inner.as_ptr();    
    }


    inline Fn String::operator +(String&& obj) -> String {
        this->_inner.push(moveObj(obj._inner));
        return String(this->_inner.copy().unwrap()); 
    }


    // /* Serializable Implementation for String */

    // Fn String::serialize(BasicSerializer& ser) -> Vec<MUInt8> {
        
    // }


    // /* Deserializable Implementation for String */

    // Fn String::deserialize(BasicDeserializer& de) -> String {

    // }

} // namespace stl 


#endif /* _STL_STRING_INCLUDE_STRING_ */
