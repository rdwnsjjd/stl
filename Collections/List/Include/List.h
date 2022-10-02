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

// #ifndef _FRAMEWORK_STL_COLLECTIONS_LIST_INCLUDE_LIST_H_
// #define _FRAMEWORK_STL_COLLECTIONS_LIST_INCLUDE_LIST_H_

// #include <iostream>
// #include <type_traits>
// using namespace std;

// #include "Framework/STL/Mem/Alloc/Inc"
// #include "Framework/STL/Iter/Inc"
// #include "Framework/Primitives/Inc"
// #include "Framework/Basics/Inc"

// using namespace stl::mem::alloc;
// using namespace stl::iter;
// using namespace framework::basics;

// namespace stl { namespace collections {
        
//     // const Size DEFAULT_ALLOCATION_SIZE = 32;
//     // #define create_proper_allocation_size_(size_)  DEFAULT_ALLOCATION_SIZE * (size_ / DEFAULT_ALLOCATION_SIZE + (size_ % DEFAULT_ALLOCATION_SIZE == 0 ? 0 : 1))

//     template<Movable M>
//     class ListNode {
//       protected:
//         M         _data;
//         ListNode* _next;
//         ListNode* _prev;

//         friend class List;

//         ListNode(){ }

//         ListNode(M data, ListNode* next, ListNode* prev):
//             _data(moveObj(data)),
//             _next(next),
//             _prev(prev)
//         { }

//         static inline ListNode create(M data, ListNode* next = nullptr, ListNode* prev = nullptr) {
//             return ListNode(moveObj(data), next, prev);
//         }
//     }; 


//     // template <Allocator A = DefaultAllocator>
//     class List: public Sized, public Mover {
//       protected:
//         Void*     _head;
//         Void*     _tail;
//         MSize     _len;
//         // A         _allocator;

//         template<Movable M> inline List(M* array, Size size);

//       public:
//         inline List():
//             _len(0) 
//         { /* nothing */ }

//         inline ~List();

//         List(List&& obj);

//         static inline Fn create() -> List {
//             return List();
//         }

//         static inline Fn copy() -> List;

//         template<Movable M> 
//         static inline Fn from(M* array, Size size) -> List;

//         static inline Fn from(List&& LISTtor) -> List;

//         template <Iterable I> static inline Fn from(I&& obj) -> List;

//         inline Fn len() -> MSize;

//         template<Movable M>
//         inline Fn push(M&& data) -> Void;

//         template<Movable M>
//         inline Fn push(M* byt, Size size) -> Void;
        
//         inline Fn push(List&& list) -> Void;
    
//         template<Movable M>
//         inline Fn pop() -> M;

//         template<Movable M>
//         inline Fn get(Idx idx) -> M&;

//         inline Fn destroy() -> Void;

//         Fn operator =(List&& obj) -> Void;

//         template<Movable M>
//         Fn operator [](Idx idx) -> M&;

//         // template<Movable M>
//         // inline Fn next() -> Option<M> override;

//         // template<Movable M>
//         // inline Fn begin() -> M* override;

//         // template<Movable M>
//         // inline Fn end() -> M* override;

//         inline Fn move() -> Void override;

//         inline Fn size() -> MSize override;

//         template<Movable M>
//         inline Fn print() -> Void;

//     };


//     /* Self Implementations For List */

//     inline List::List(List&& obj) {
//         this->_head = obj._head;
//         this->_tail = obj._tail;
//         this->_len  = obj._len;
//     }


//     inline Fn List::copy() -> List {
//         // return self;
//     }


//     template<Movable M>
//     inline List::List(M* array, Size size) {
//         Let prev = (ListNode<M>**) &this->_head;

//         for (MIdx i = 0; i < size; i++) {
//             Let node = new ListNode<M>(array[i], nullptr, nullptr);
            
//             if (this->_len != 0) {
//                 (*prev)->_next = node;
//                 node->_prev = (*prev);
//                 this->_tail  = node;
//                 prev = &(*prev)->_next;
//             }
//             else {
//                 this->_head = node;
//                 this->_tail = node;
//             }

//             this->_len++;
//         }
//     }


//     template<Movable M>
//     inline Fn List::from(M* array, Size size) -> List {
//         return List(array, size);
//     }


//     inline Fn List::from(List&& LISTtor) -> List {
//         return List(moveObj(LISTtor));
//     }


//     template<Iterable I> 
//     inline Fn List::from(I&& obj) -> List {
//         Let list = List();
//         Loop {
//             Let val = obj.next();
//             if (val.is_none()) {
//                 break;
//             }

//             list.push(val.unwrap());
//         }

//         return list;
//     }


//     inline Fn List::len() -> MSize {
//         return this->_len;
//     }


//     template<Movable M>
//     inline Fn List::print() -> Void {
//         Let elem = (ListNode<M>*) this->_head;

//         cout << "{ ";
//         for (MIdx i = 0; i < this->_len; i++) {
//             // TODO: do not use cout
//             cout << elem->_data << ", ";
//             elem = elem->_next;
//         }
//         cout << "}" << '\n';
//     }


//     template<Movable M>
//     inline Fn List::push(M&& data) -> Void {
//         Let node = new ListNode<M>(moveObj(data));
//         ((ListNode<M>*) this->_tail)->_next = node;
//         this->_len++;
//     }


//     inline Fn List::push(List&& list) -> Void {
//         // Let len = list.len();
//         // Let elem = list._head;
        
//         // for (MIdx i = 0; i < len; i++) {
//         //     this->push((ListNode<M>*) elem);
//         // }
//     }

 
//     template<Movable M>
//     inline Fn List::push(M* byt, Size size) -> Void {
//         Let len = this->_len;
//         Let elem = this->_head;

//         for (MIdx i = 0; i < len; i++) {
//             this->push((ListNode<M>*) elem);
//         }
//     }


//     template<Movable M>
//     inline Fn List::pop() -> M {
//         Let to_be_popped = this->_tail;
//         ((ListNode<M>*) this->_tail)->_prev = nullptr;

//         this->_len--;
//         return to_be_popped;
//     }


//     template<Movable M>
//     inline Fn List::get(Idx idx) -> M& {
//         // TODO: check bounds 
        
//         Let len  = this->_len;
//         Let elem = this->_head;

//         for (MIdx i = 0; i < len; i++) {
//             if (i == idx) {
//                 return elem;
//             }
//             elem = ((ListNode<M>*) elem)->_next;
//         }
//     }


//     inline Fn List::destroy () -> Void {
//         // TODO
//         Let elem = this->_head;

//         for (MIdx i = 0; i < this->_len; i++) {
//             // Let next = elem->
//             delete elem;
//         }
//     }


//     // template<Allocator A> 
//     inline List::~List () {
//         if (!this->check_move()) {
//             this->destroy();
//         }
//     }


//     // /* Iterator Implementations */

//     // template<Allocator A> 
//     // inline Fn List::next() -> Option<M> {
//     //     return Option<M>::None;
//     // }


//     // template<Allocator A> 
//     // inline Fn List::begin() -> M* {
//     //     return this->_inner.ptr();
//     // }


//     // template<Allocator A> 
//     // inline Fn List::end() -> M* {
//     //     return this->_inner.ptr() + this->_len;
//     // }



//     // /* Move Implementations */

//     inline Fn List::move() -> Void {
//         this->is_moved = true;
//     }



//     // /* Sized Implementations */

//     inline Fn List::size() -> MSize {
//         // return this->_inner.size() + sizeof(this->_len);
//     }



//     // /* Operator Overloading */

//     inline Fn List::operator =(List&& obj) -> Void {
//         this->_head = obj._head;
//         this->_tail = obj._tail;
//         this->_len  = obj._len;
//     }

//     template<Movable M> 
//     inline Fn List::operator [](Idx idx) -> M& {
//         // return this->get(idx);
//     }

    
// } // namespace basics 
// } // namespace framework




// #endif // _FRAMEWORK_STL_COLLECTIONS_LIST_INCLUDE_LIST_H_
