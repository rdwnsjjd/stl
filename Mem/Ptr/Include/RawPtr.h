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

#ifndef _MEM_PTR_INCLUDE_RAWPTR_
#define _MEM_PTR_INCLUDE_RAWPTR_

#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/BasicDefs.h"
#include "Framework/Primitives/Option.h"
#include "Framework/STL/Mem/Alloc/Include/Default.h"

namespace stl::mem {

    using namespace framework;
    using namespace framework::basics;
    using namespace stl::mem::alloc;

    template<typename T, Allocator A = DefaultAllocator>
    class RawPtr: private Mover, private CanBeCopied<EResult<RawPtr<T, A>>>, Dropper {
        T* _inner;

      public:
        RawPtr() = default;

        RawPtr(T* ptr): _inner(ptr) { }

        RawPtr(RawPtr&& obj);

        static inline Fn create() -> RawPtr;

        static inline Fn from_ptr(T* ptr) -> RawPtr;

        inline Fn copy() -> EResult<RawPtr> override;

        inline Fn is_null() -> MBool const;

        UNSAFE inline Fn add(Size idx) -> RawPtr;

        UNSAFE inline Fn sub(Size idx) -> RawPtr;

        UNSAFE inline Fn offset(SSize idx) -> RawPtr;

        template<typename U>
        inline Fn cast() -> RawPtr const;

        inline Fn as_mut_ptr() -> T*;

        inline Fn as_ptr() -> const T*;

        inline Fn addr() -> Size;

        // UNSAFE inline Fn as_ref() -> Option<const T&>;

        UNSAFE inline Fn as_mut() -> T&;

        inline Fn copy_to(T* dst, Size size) -> Void const;

        inline Fn copy_from(const T* src, Size size) -> Void;

        static Fn test() -> $;

        // inline Fn free() -> Void;

        UNSAFE inline Fn operator [](Size idx) -> T;

        UNSAFE inline Fn operator *() -> T;

        UNSAFE inline Fn operator +(Size idx) -> RawPtr;

        UNSAFE inline Fn operator -(Size idx) -> RawPtr;

        inline Fn operator ->() -> T*;

        inline Fn operator =(RawPtr&& obj) -> Void;
    };



    /* RawPtr Self Implementation */

    template<typename T, Allocator A>
    RawPtr<T, A>::RawPtr(RawPtr&& obj): _inner(obj._inner) {
        obj.drop();
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::create() -> RawPtr {
        return RawPtr(NULL);
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::from_ptr(T* ptr) -> RawPtr {
        return RawPtr(ptr);
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::copy() -> EResult<RawPtr> {
        return resOk(RawPtr, RawPtr(this->_inner));
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::is_null() -> MBool const {
        return this->_inner == NULL;
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::add(Size idx) -> RawPtr {
        RawPtr ret;
        unsafe (ret = this->offset(idx);)
        return ret;
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::sub(Size idx) -> RawPtr {
        RawPtr ret;
        unsafe (ret = this->offset(-idx);)
        return ret;
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::offset(SSize idx) -> RawPtr {
        this->_inner += idx;
        return move_obj(*this);
    }


    template<typename T, Allocator A>
    template<typename U>
    inline Fn RawPtr<T, A>::cast() -> RawPtr const {
        return RawPtr((U*) this->_inner);
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::as_mut_ptr() -> T* {
        return this->_inner;
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::as_ptr() -> const T* {
        return this->_inner;
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::addr() -> Size {
        return (Size) this->_inner;
    }


    // template<typename T, Allocator A>
    // inline Fn RawPtr<T, A>::as_ref() -> Option<const T&> {
    //     return (this->is_null() ? optNone(const T&) : optSome(const T&, *(this->_inner)));
    // }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::as_mut() -> T& {
        evalOrAbort(!this->is_null(), "The pointer is null!");
        return *(this->_inner);
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::copy_to(T* dst, Size size) -> Void const {
        memcpy(dst, this->_inner, size);
    }


    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::copy_from(const T* src, Size size) -> Void {
        memcpy(this->_inner, src, size);
    }



    /* RawPtr Operator Overloading */

    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::operator [](Size idx) -> T {
        return this->_inner[idx];
    }

    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::operator *() -> T {
        return *(this->_inner);
    }

    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::operator +(Size idx) -> RawPtr {
        RawPtr ret;
        unsafe (ret = this->add(idx);)
        return ret;
    }

    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::operator -(Size idx) -> RawPtr {
        RawPtr ret;
        unsafe (ret = this->sub(idx);)
        return ret;
    }

    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::operator =(RawPtr&& obj) -> Void {
        this->_inner = obj._inner;
        obj.drop();
    }

    template<typename T, Allocator A>
    inline Fn RawPtr<T, A>::operator ->() -> T* {
        return this->_inner;
    }


    Let raw_ptr_test = RawPtr<$>::test();
    
} // namespace stl::mem 


namespace stl::mem {
    
    template<typename T, Allocator A>
    Fn RawPtr<T, A>::test() -> $ {
        test::global_tester.add_test(test::TestBase::create("raw_ptr_test_as_ref", []() -> MBool {
            RawPtr<char> ptr = (char*) malloc(56);
            ptr.copy_from("Salam!", 7);

            unsafe (
                Ref ref = ptr.as_mut();
                ref = 'c';
                RawPtr<char> p = ptr + 0;
                evalEqCharLog('c', *p);
            )

        }));

        return _;
    }

} // stl::mem


#endif /* _MEM_PTR_INCLUDE_RAWPTR_ */
