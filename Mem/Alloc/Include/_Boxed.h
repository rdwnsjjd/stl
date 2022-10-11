// // Copyright (C) 2022 Emad
// // 
// // This file is part of sandikCpp.
// // 
// // sandikCpp is free software: you can redistribute it and/or modify
// // it under the terms of the GNU General Public License as published by
// // the Free Software Foundation, either version 3 of the License, or
// // (at your option) any later version.
// // 
// // sandikCpp is distributed in the hope that it will be useful,
// // but WITHOUT ANY WARRANTY; without even the implied warranty of
// // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// // GNU General Public License for more details.
// // 
// // You should have received a copy of the GNU General Public License
// // along with sandikCpp.  If not, see <http://www.gnu.org/licenses/>.

// #ifndef _MEM_ALLOC_INCLUDE_BOXED_
// #define _MEM_ALLOC_INCLUDE_BOXED_

// #include "Framework/Basics/Inc"
// #include "Test/Include/Test.h"
// #include "../../Sized/Inc"
// #include "./Default.h"

// namespace stl { namespace mem { namespace alloc {

//     using namespace test;
//     using namespace framework::basics;

//     template<typename T>
//     concept SizedMovable = Movable<T>; // && Sizable<T>;

//     template<
//         Movable   T,    // Think if Boxed value must be copyable or not
//         Allocator A = DefaultAllocator
//     >
//     class Boxed: public Mover, CanBeCopied<EResult<Boxed<T, A>>>, Dropper {
//       private:
//         T* _inner;
//         A  _allocator;

//         inline Boxed(T* ptr) {
//             this->_inner = ptr;
//         }

//       public:
//         inline Boxed() { }

//         inline Boxed(Boxed&& obj) {
//             this->_inner = move_obj(obj._inner);
//             obj.drop();
//         }

//         // UNSTABLE_F(Boxed)
//         static inline Fn create(T data) -> EResult<Boxed> {
//             Let res = A().allocate(1, sizeof(T));
//             unwrapResOrRet(
//                 res, 
//                 resFail(Boxed, EK::AllocationFailure), 
//                 "Failed to allocate `%ld` bytes of memory to create a new boxed object!", sizeof(T)
//             );
//             Let new_ptr = new(res.unwrap()) T(move_obj(data));
//             // Let new_ptr = new T(move_obj(data));
//             return resOk(Boxed, Boxed((T*) new_ptr));
//         }


//         static inline Fn from_raw_ptr(MPtr ptr, Size size) -> EResult<Boxed> {
//             Let res = A().allocate(1, size);
//             unwrapResOrRet(
//                 res, 
//                 resFail(Boxed, EK::AllocationFailure), 
//                 "Failed to allocate `%ld` bytes of memory to create a new boxed object for raw pointer!", sizeof(T)
//             );
//             Let new_ptr = new(res.unwrap()) T(move_obj(*((T*) res.unwrap())));
//             return resOk(Boxed, Boxed((T*) new_ptr));
//         }


//         /**
//          * abort-based boxed object create API.
//          * This API will create a new boxed object but will aborted if allocation failed.
//          * This isn't based on our approach to all API's result should be controlled by user,
//          * but may used in special cases and any failure is on user itself.
//          */
//         static inline Fn ab_create(T data) -> Boxed {
//             Let ptr = A().allocate(1, sizeof(T)).unwrap();
//             return Boxed(new(ptr) T(move_obj(data)));
//         }


//         /**
//          * abort-based boxed object from_raw_ptr API.
//          * This API will create a new boxed object from raw pointer but will aborted if allocation failed.
//          * This isn't based on our approach to all API's result should be controlled by user,
//          * but may used in special cases and any failure is on user itself.
//          */
//         static inline Fn ab_from_raw_ptr(MPtr ptr, Size size) -> EResult<Boxed> {
//             Let res = A().allocate(1, size);
//             unwrapResOrRet(
//                 res, 
//                 resFail(Boxed, EK::AllocationFailure), 
//                 "Failed to allocate `%ld` bytes of memory to create a new boxed object for raw pointer!", sizeof(T)
//             );
//             return resOk(Boxed, Boxed(ptr));
//         }

//         inline Fn copy() -> EResult<Boxed> {
//             Let res = Boxed::from_raw_ptr(this->_inner, sizeof(T));
//             unwrapResOrRetSame(
//                 Boxed, res, 
//                 "Failed to create boxed object from inner pointer of another while copying it!"
//             );
//             return resOk(Boxed, res.unwrap());
//         }

//         inline Fn as_ptr() -> T* {
//             return this->_inner;
//         }

//         inline Fn replace(T data) -> Boxed {
//             this._inner = this->_allocator.reallocate(this->_inner, sizeof(data));
//             soft_assert_ret(is_valid_handle(this->_inner), Boxed::create(""), "Failed to replace content of box!");

//             memcpy(this->_inner, data, sizeof(data));
//             return self;
//         }

//         inline Fn operator *() -> T& {
//             return static_cast<T&>(*this->_inner);
//         }

//         inline Fn operator ->() -> T {
//             return static_cast<T>(*this->_inner);
//         }

//         inline Fn operator =(T&& data) -> Void {
//             *this->_inner = data;
//         }

//         inline Fn operator =(T* data) -> Void {
//             this->_inner = data;
//         }

//         inline Fn operator =(Boxed&& obj) -> Void {
//             this->_inner = move_obj(obj._inner);
//             obj.drop();
//         }

//         inline Fn destroy() -> Void {
//             this->_allocator.deallocate(this->_inner);
//             // delete this->_inner;
//         }

//         inline ~Boxed() {
//             if (!this->dropped) {
//                 this->_inner->~T();
//                 this->destroy();
//             }
//         };

//     //   protected:
//         static Fn test() -> $;
//     };

//     Let _ = Boxed<$>::test();
// } // namespace alloc
// } // namespace mem
// } // namespace stl



// template<Movable T, stl::mem::alloc::Allocator A>
// Fn stl::mem::alloc::Boxed<T, A>::test() -> $ {
//     class Object: public Mover, public Dropper  {
//       public:
//         MInt32 a;
//         Str    str;
//         Object(Int32 a, Str str): a(a), str(str) {}
//         Object(Object&& obj): a(obj.a), str(obj.str) {}

//         Void operator =(Object&& obj) {
//             this->a = obj.a;
//             this->str = obj.str;
//         }
//     };


//     // global_tester.add_test(TestBase::create("box_with_integral_value_test_create", []() -> MBool {
//     //     Let val = Boxed<Int32>::create(-769);
//     //     evalEqIntLog(*val, -769);
//     // }));


//     // global_tester.add_test(TestBase::create("box_with_ojbect_value_test_create", []() -> MBool {
//     //     Boxed<Object> val = Boxed<Object>::create(Object(-83, "This is an test object"));
//     //     evalEqIntLog((*val).a, -83);
//     // }));

    
//     // global_tester.add_test(TestBase::create("box_with_integral_value_test_copy", []() -> MBool {
//     //     Let val = Boxed<Int32>::create(-769);
//     //     evalEqIntLog(*(val.copy()), *(Boxed<Int32>::create(-769)));
//     //     return true;
//     // }));


//     // global_tester.add_test(TestBase::create("box_with_ojbect_value_test_copy", []() -> MBool {
//     //     Boxed<Object> val = Boxed<Object>::create(Object(-83, "This is an test object"));
//     //     Let v = move_obj(*(val.copy()));
//     //     Let n = move_obj(*(Boxed<Object>::create(Object(-83, "This is an test object"))));
//     //     evalEqIntLog(v.a, n.a);
//     // }));

//     return _;
// }

// #endif /* _MEM_ALLOC_INCLUDE_BOXED_ */
