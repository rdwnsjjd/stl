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

#ifndef _COLLECTIONS_BASICVEC_INCLUDE_BASICVEC_
#define _COLLECTIONS_BASICVEC_INCLUDE_BASICVEC_

#include "Framework/Basics/OwnerShip.h"
#include "Framework/Primitives/Result.h"
#include "Framework/STL/Mem/Sized/Include/Sized.h"
#include "Framework/STL/Mem/Alloc/Include/Default.h"
#include "Framework/STL/IO/Serialization/Include/Serialize.h"
#include "Framework/STL/IO/Serialization/Include/Deserialize.h"

namespace stl { namespace collections {

    using namespace stl::io;
    using namespace stl::mem;
    using namespace stl::mem::alloc;
    using namespace framework::basics;

    // static Domain DOMAIN = Domain::Collections;

    #if __cplusplus > 201703L && __cpp_concepts >= 201907L
        template<Movable T, Allocator A = DefaultAllocator>
    #else
        template<
            typename T, 
            typename A = DefaultAllocator, std::enable_if_t<std::is_base_of_v<BasicAllocator, A>, bool> = true
        >
    #endif
    class BasicVec: public Mover, public CanBeCopied<EResult<BasicVec<T, A>>>, public Dropper, public Sized {

      private:
        T*    _raw_ptr;
        MSize _capacity = 0;
        A     _allocator;

        Fn _reserve_capacity(Size size) -> EResult<MSize>;


      public:
        BasicVec(T* ptr, Size cap);

        // DEPRECATED_I("create")
        inline BasicVec() : _raw_ptr(), _capacity(0) { /* nothing todo! */ };

        inline BasicVec(BasicVec&& obj);
    
        static inline Fn create() -> BasicVec;
        
        static inline Fn with_capacity(Size cap) -> EResult<BasicVec>;
        
        static inline Fn from_raw_ptr(Ptr ptr, Size cap) -> EResult<BasicVec>;

        inline Fn reserve(Size size) -> EResult<MSize>;

        inline Fn need_reserve(Size val) -> MBool const;

        inline Fn capacity() -> Size const;

        inline Fn copy() -> EResult<BasicVec>;

        UNSTABLE_F(Sized)
        inline Fn size() -> MSize override;

        inline Fn ptr() -> T* const;

        Fn operator =(BasicVec&& obj) -> Void;

        Fn drop() -> Void override;

        inline ~BasicVec() {
            if (this->_raw_ptr) {
                this->_allocator.deallocate(this->_raw_ptr);
            }
        }
    };



    /* Self Implementation for BasicVec<T, A> */

    template<Movable T, Allocator A>
    inline BasicVec<T, A>::BasicVec(T* ptr, Size cap) {
        this->_raw_ptr  = ptr;
        this->_capacity = cap;
    }


    template<Movable T, Allocator A>
    inline BasicVec<T, A>::BasicVec(BasicVec&& obj) {
        this->_raw_ptr   = obj._raw_ptr;
        this->_capacity  = obj._capacity;
        this->_allocator = obj._allocator;

        obj.drop();
    }
    
    
    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::create() -> BasicVec {
        return BasicVec();
    }


    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::with_capacity(Size cap) -> EResult<BasicVec> {
        Let res = A().allocate(cap, sizeof(T));
        unwrapResOrRetSame(BasicVec, res, "Failed to allocate memory for inner pointer of BasicVec!");
        return resOk(BasicVec, BasicVec((T*) res.unwrap(), cap));
    }

    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::from_raw_ptr(Ptr ptr, Size cap) -> EResult<BasicVec> {
        Let res = A().allocate(cap, 1);
        unwrapResOrRetSame(BasicVec, res, "Failed to allocate memory for creating BasicVec from raw pointer!");

        memcpy(res.unwrap(), ptr, cap);
        return resOk(BasicVec, BasicVec((T*) res.unwrap(), cap));
    }


    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::reserve(Size size) -> EResult<MSize> {
        return this->_reserve_capacity(size);
    }


    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::need_reserve(Size val) -> MBool const {
        return val > this->_capacity;
    }


    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::capacity() -> Size const {
        return this->_capacity;
    }


    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::ptr() -> T* const {
        return this->_raw_ptr;
    }


    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::drop() -> Void {
        this->_raw_ptr  = nullptr;
        this->_capacity = 0;
    }


    template<Movable T, Allocator A>
    Fn BasicVec<T, A>::_reserve_capacity(Size size) -> EResult<MSize> {
        if (this->_capacity == 0 || this->_capacity == INVALID_ID) {
            Let res = this->_allocator.allocate(size, sizeof(T));
            unwrapResOrRetSame(MSize, res, "Failed to allocate `%d` bytes for vector inner pointer!", size * sizeof(T));
            this->_raw_ptr = (T*) res.unwrap();
        }
        else {
            Let res = this->_allocator.reallocate(this->_raw_ptr, size * sizeof(T));
            unwrapResOrRetSame(MSize, res, "Failed to reallocate `%d` bytes for vector inner pointer!", size * sizeof(T));
            this->_raw_ptr = (T*) res.unwrap();
        }

        this->_capacity = size;
        return EResult<MSize>::ok(size);
    }



    /* Copyable Implementation for BasicVec<T, A> */

    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::copy() -> EResult<BasicVec> {
        Let res = BasicVec::from_raw_ptr((Ptr) this->_raw_ptr, this->_capacity);
        unwrapResOrRetSame(BasicVec, res, "Failed to create new BasicVec from raw ptr to make a copy of it!");
        return resOk(BasicVec, res.unwrap());
    }



    /* Operator Overloading for BasicVec<T, A> */

    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::operator =(BasicVec&& obj) -> Void {
        this->_raw_ptr   = obj._raw_ptr;
        this->_capacity  = obj._capacity;
        this->_allocator = obj._allocator;

        obj.drop();
    }



    /* Sized Implementation for BasicVec<T, A> */

    template<Movable T, Allocator A>
    inline Fn BasicVec<T, A>::size() -> MSize {
        return sizeof(this->_capacity) + this->_capacity * sizeof(T);
    }
    
} // namespace collections
} // namespace stl 


#endif /* _COLLECTIONS_BASICVEC_INCLUDE_BASICVEC_ */
