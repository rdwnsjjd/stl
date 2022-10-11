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

#ifndef _MEM_ALLOC_INCLUDE_BOXED_
#define _MEM_ALLOC_INCLUDE_BOXED_

#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/BasicDefs.h"
#include "Framework/Basics/OwnerShip.h"
#include "Framework/Primitives/Result.h"
#include "Framework/STL/Mem/Ptr/Include/RawPtr.h"
#include "Framework/STL/Mem/Helpers.h"
#include "Test/Include/Test.h"
#include "./Default.h"

namespace stl::mem::alloc {

    using namespace test;
    using namespace stl::mem;
    using namespace framework;
    using namespace framework::basics;


    template<Movable T, Allocator A = DefaultAllocator>
    class Boxed: private Mover, private CanBeCopied<EResult<Boxed<T, A>>>, Dropper {
        RawPtr<T> _inner;
        A         _allocator;

        Boxed(RawPtr<T> ptr): _inner(move_obj(ptr)), _allocator(A()) { };

      public:
        ~Boxed();
        Boxed(): _inner() { }
        Boxed(Boxed&& obj): _inner(move_obj(obj._inner)), _allocator(obj._allocator) {
            obj.drop();
        }

        static inline Fn create(T data) -> EResult<Boxed>;

        static inline Fn create_uninit() -> EResult<Boxed>;

        inline Fn copy() -> EResult<Boxed>;

        inline Fn modify(T data) -> EResult<$>;

        inline Fn modify(RawPtr<T> ptr) -> EResult<$>;

        inline Fn operator =(Boxed&& obj);

        static Fn test() -> $;
    };



    /* Self Implementation for Boxed<T, A> */
    template<Movable T, Allocator A>
    inline Fn Boxed<T, A>::create(T data) -> EResult<Boxed> {
        Let ptr = mem_new(data);
        return resOk(Boxed, move_obj(Boxed(ptr)));
    }


    template<Movable T, Allocator A>
    inline Fn Boxed<T, A>::create_uninit() -> EResult<Boxed> {
        Let ptr = mem_new<T>();
        return resOk(Boxed, move_obj(Boxed(ptr)));
    }


    // FIXME: multiple destructor call here for Boxed
    template<Movable T, Allocator A>
    inline Fn Boxed<T, A>::copy() -> EResult<Boxed> {
        Mut new_boxed = Boxed::create_uninit();
        unwrapResOrRetSame(Boxed, new_boxed, "Failed to create uninitialized new boxed to make a copy of original boxed!");

        Let res = new_boxed.unwrap().modify(this->_inner.copy().unwrap());
        unwrapResOrRetSame(Boxed, res, "Failed to modify new boxed after create, in order to make a copy of original one!");
        return resOk(Boxed, move_obj(new_boxed.unwrap()));
    }


    template<Movable T, Allocator A>
    inline Fn Boxed<T, A>::modify(T data) -> EResult<$> {
        evalOrRetFail(
            $, mem_copy(this->_inner, &data, mem_size<T>()) == 0,
            "Failed to copy memory while modifying boxed!"
        );
        return resOkUnit();
    }


    template<Movable T, Allocator A>
    inline Fn Boxed<T, A>::modify(RawPtr<T> ptr) -> EResult<$> {
        // evalOrRetFail(
        //     $, mem_copy(this->_inner.as_mut_ptr(), ptr.as_ptr(), mem_size<T>()) == 0,
        //     "Failed to copy memory while modifying boxed!"
        // );
        // return resOkUnit();
    }


    template<Movable T, Allocator A>
    Boxed<T, A>::~Boxed() {
        if (!this->dropped) {
            mem_delete(this->_inner.as_ptr());
        }
        
    }
    

    template<Movable T, Allocator A>
    inline Fn Boxed<T, A>::operator =(Boxed&& obj) {
        this->_inner     = move_obj(obj._inner);
        this->_allocator = obj._allocator;
        obj.drop();
    }

    Let boxed_test = Boxed<$>::test();
} // stl::mem::alloc


namespace stl::mem::alloc {
    
    template<Movable T, Allocator A>
    Fn Boxed<T, A>::test() -> $ {
        class Object: public Mover, public Dropper  {
        public:
            MInt32 a;
            Str    str;
            Object(Int32 a, Str str): a(a), str(str) {}
            Object(Object&& obj): a(obj.a), str(obj.str) {}

            Void operator =(Object&& obj) {
                this->a = obj.a;
                this->str = obj.str;
            }
        };


        global_tester.add_test(TestBase::create("box_with_integral_value_test_create", []() -> MBool {
            Let val = Boxed<Str>::create("Salam");
            Let n   = val.unwrap().copy().unwrap();
            evalEqIntLog(-769, -769);
        }));


        // global_tester.add_test(TestBase::create("box_with_ojbect_value_test_create", []() -> MBool {
        //     Boxed<Object> val = Boxed<Object>::create(Object(-83, "This is an test object"));
        //     evalEqIntLog((*val).a, -83);
        // }));

        
        // global_tester.add_test(TestBase::create("box_with_integral_value_test_copy", []() -> MBool {
        //     Let val = Boxed<Int32>::create(-769);
        //     evalEqIntLog(*(val.copy()), *(Boxed<Int32>::create(-769)));
        //     return true;
        // }));


        // global_tester.add_test(TestBase::create("box_with_ojbect_value_test_copy", []() -> MBool {
        //     Boxed<Object> val = Boxed<Object>::create(Object(-83, "This is an test object"));
        //     Let v = move_obj(*(val.copy()));
        //     Let n = move_obj(*(Boxed<Object>::create(Object(-83, "This is an test object"))));
        //     evalEqIntLog(v.a, n.a);
        // }));

        return _;
    }


} // namespace stl::mem::alloc 

#endif /* _MEM_ALLOC_INCLUDE_BOXED_ */
