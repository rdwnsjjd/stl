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

#ifndef _MEM_ALLOC_INCLUDE_SLICE_
#define _MEM_ALLOC_INCLUDE_SLICE_

#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/OwnerShip.h"
#include "Framework/Basics/BasicOperations.h"
#include "Framework/Primitives/Option.h"
#include "Framework/Primitives/Result.h"
#include "Framework/STL/Mem/Alloc/Include/Default.h"

#include "Test/Include/Test.h"
using namespace test;

namespace stl::mem {

    using namespace framework;
    using namespace framework::basics;
    using namespace stl::mem::alloc;

    template<Movable T, Allocator A = DefaultAllocator>
    class Slice: private Mover, private CanBeCopied<EResult<Slice<T, A>>>, private Dropper {
      private:
        T*    _ptr;
        MSize _len;
        A     _allocator;

        inline Slice(T* ptr, Size len) {
            this->_ptr = ptr;
            this->_len = len;
        }

      public:
        Slice() {};
        Slice(Slice&& obj);

        static inline Fn from_raw_ptr(Ptr ptr, Size len) -> EResult<Slice>;

        static inline Fn from_raw_ptr_unchecked(Ptr ptr, Size len) -> Slice;

        inline Fn len() -> MSize const;

        inline Fn is_empty() -> Bool const;

        inline Fn first() -> Option<const T&> const;

        inline Fn first_mut() -> Option<T&>;

        inline Fn last() -> Option<const T&> const;

        inline Fn last_mut() -> Option<T&>;

        inline Fn get(Idx idx) -> Option<const T&> const;

        inline Fn get_mut(Idx idx) -> Option<T&>;

        inline Fn get_unchecked(Idx idx) -> const T&;

        inline Fn get_unchecked_mut(Idx idx) -> T&;

        inline Fn as_ptr() -> const T*;

        inline Fn as_mut_ptr() -> T*;

        inline Fn as_ptr_range() -> Range<const T*> const;

        inline Fn as_mut_ptr_range() -> Range<T*>;

        inline Fn swap(Idx a, Idx b) -> EResult<$>;

        inline Fn swap_unchecked(Idx a, Idx b);

        inline Fn reverse();

        inline Fn copy() -> EResult<Slice> override;

        inline Fn copy_unchecked() -> Slice;

        inline Fn operator =(Slice&& obj) -> Void;

        inline ~Slice() {
            if (!this->dropped) {
                this->_allocator.deallocate(this->_ptr);
                this->_len = 0;
            }
        }

        static Fn test() -> $;
    };


    template<Movable T, Allocator A>
    inline Slice<T, A>::Slice(Slice&& obj) {
        this->_ptr = obj._ptr;
        this->_len = obj._len;
        obj.drop();
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::from_raw_ptr(Ptr ptr, Size len) -> EResult<Slice> {
        Let res = A().allocate(1, len);
        unwrapResOrRet(res, resFail(Slice, EK::AllocationFailure), "Failed to allocate memory while creating new slice!");
        
        Let new_ptr = res.unwrap();
        memcpy(new_ptr, ptr, len);
        return resOk(Slice, move_obj(Slice((T*) new_ptr, len)));
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::from_raw_ptr_unchecked(Ptr ptr, Size len) -> Slice {
        Let res = A().allocate(1, len);
        Let new_ptr = res.unwrap();
        memccpy(new_ptr, ptr, len);
        return Slice(ptr, len);
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::len() -> MSize const {
        return this->_len;
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::is_empty() -> Bool const {
        return this->_len == 0;
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::first() -> Option<const T&> const {
        return this->_len == 0 ? optNone(const T&) : optSome(const T&, this->_ptr[0]);
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::first_mut() -> Option<T&> {
        return this->_len == 0 ? optNone(T&) : optSome(T&, this->_ptr[0]);
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::last() -> Option<const T&> const {
        return this->_len == 0 ? optNone(const T&) : optSome(const T&, this->_ptr[this->_len - 1] );
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::last_mut() -> Option<T&> {
        return this->_len == 0 ? optNone(T&) : optSome(T&, this->_ptr[this->_len - 1] );
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::get(Idx idx) -> Option<const T&> const {
        evalOrRetNone(T&, idx < this->_len, "The index exceeded slice bounds!");
        return optSome(T&, (const T) this->_ptr[idx]);
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::get_mut(Idx idx) -> Option<T&> {
        evalOrRetNone(T&, idx < this->_len, "The index exceeded slice bounds!");
        return optSome(T&, this->_ptr[idx] );
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::get_unchecked(Idx idx) -> const T& {
        evalOrAbort(idx < this->_len, "The index exceeded slice bounds!");
        return this->_ptr[idx];
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::get_unchecked_mut(Idx idx) -> T& {
        return this->_ptr[idx];
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::as_ptr() -> const T* {
        return this->_ptr;
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::as_mut_ptr() -> T* {
        return this->_ptr;
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::as_ptr_range() -> Range<const T*> const {
        return (Range<const T*>) { .start = this->_ptr, .end = this->_ptr[this->_len - 1] };
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::as_mut_ptr_range() -> Range<T*> {
        return (Range<T*>) { .start = this->_ptr, .end = this->_ptr[this->_len - 1] };
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::swap(Idx a, Idx b) -> EResult<$> {
        evalOrRetFail($, a < this->_len, "The `a` index exceeded slice bounds!");
        evalOrRetFail($, b < this->_len, "The `b` index exceeded slice bounds!");
        evalOrRetFail($, b > a, "The `a` index must be greater than `b` index!");

        Let tmp = this->_ptr[a];
        this->_ptr[a] = this->_ptr[b];
        this->_ptr[b] = tmp;

        return resOk($, _);
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::swap_unchecked(Idx a, Idx b) {
        Let tmp = this->_ptr[a];
        this->_ptr[a] = this->_ptr[b];
        this->_ptr[b] = tmp;
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::reverse() {
        // TODO ...
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::copy() -> EResult<Slice> {
        Let res = Slice::from_raw_ptr(this->_ptr, this->_len);
        unwrapResOrRet(res, resFail(Slice, EK::CreationFailure), "Failed to create new slice while copying old one!");
        return resOk(Slice, move_obj(res.unwrap()));
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::copy_unchecked() -> Slice {
        Let res = Slice::from_raw_ptr(this->_ptr, this->_len);
        return res.unwrap();
    }


    template<Movable T, Allocator A>
    inline Fn Slice<T, A>::operator =(Slice&& obj) -> Void {
        this->_ptr = obj._ptr;
        this->_len = obj._len;
        this->drop();
    }


    Let slice_test = Slice<int>::test();
}


namespace stl::mem {
    
    template<Movable T, Allocator A>
    Fn Slice<T, A>::test() -> $ {
        
        global_tester.add_test(TestBase::create("slice_test_create_simple", []() -> MBool {
            Let ptr = "This is just a simple test for testing slice creating and usage!";
            Let slice = Slice<char>::from_raw_ptr(ptr, 65).unwrap();

            Let g = 7;
            Ref r = g;

            r = 9;

            // Ref a = slice.get(0).unwrap();
            // a = '6';

            return true;
        }));
        

        return _;
    }

} // namespace stl::mem 


#endif /* _MEM_ALLOC_INCLUDE_SLICE_ */
