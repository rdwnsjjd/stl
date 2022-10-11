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

#ifndef _COLLECTIONS_LIST_INCLUDE_LIST_
#define _COLLECTIONS_LIST_INCLUDE_LIST_

#include <iostream>

#include "Framework/STL/Mem/Alloc/Include/Default.h"
#include "Framework/STL/Mem/Ptr/Include/RawPtr.h"
#include "Framework/STL/Iter/Include/Iterator.h"
#include "Framework/Primitives/Result.h"
#include "Framework/Primitives/Option.h"
#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/BasicDefs.h"
#include "Framework/Basics/OwnerShip.h"

namespace stl { namespace collections {

    using namespace stl::mem::alloc;
    using namespace stl::mem;
    using namespace stl::iter;
    using namespace framework::basics;


    template<Movable T, Allocator A>
    class List;


    template<Movable T>
    class ListNode: public Mover, public CanBeCopied<ListNode<T>>, public Dropper {
        template<Movable U, Allocator A> friend class List;
      protected:
        T                           _data;
        Option<RawPtr<ListNode<T>>> _next;
        Option<RawPtr<ListNode<T>>> _prev;

        ListNode(
            T data, 
            Option<RawPtr<ListNode<T>>> next, 
            Option<RawPtr<ListNode<T>>> prev
        ): _data(move_obj(data)), _next(move_obj(next)), _prev(move_obj(prev)) { }

      public:
        ListNode(T data): _data(data) { }

        ListNode(ListNode&& obj) {
            this->_data = obj._data;
            this->_next = obj._next;
            this->_prev = obj._prev;
            obj.drop();
        }

        static inline Fn create(
            T data, 
            Option<RawPtr<ListNode<T>>> next = optNone(RawPtr<ListNode<T>>), 
            Option<RawPtr<ListNode<T>>> prev = optNone(RawPtr<ListNode<T>>)
        ) -> ListNode;

        inline Fn copy() -> ListNode override;

        inline Fn get_data() -> const T&;

        inline Fn get_mut_data() -> T&;

        inline Fn set_next(MPtr next) -> Void;

        inline Fn get_next() -> MPtr const;

        inline Fn set_prev(MPtr prev) -> Void;

        inline Fn get_prev() -> MPtr const;

        inline Fn operator =(ListNode&& obj) -> Void;
    };



    /* Self Implementation for ListNode */

    template<Movable T>
    inline Fn ListNode<T>::create(
        T data, 
        Option<RawPtr<ListNode<T>>> next, 
        Option<RawPtr<ListNode<T>>> prev
    ) -> ListNode {
        return ListNode(move_obj(data), move_obj(next), move_obj(prev));
    } 


    template<Movable T>
    inline Fn ListNode<T>::copy() -> ListNode {
        CONST_IF (IsPrimitive<T>) {
            return ListNode(this->_data, move_obj(this->_next), move_obj(this->_prev));
        }
        CONST_ELSE {
            return ListNode(this->_data.copy(), move_obj(this->_next), move_obj(this->_prev));
        }
    }


    template<Movable T>
    inline Fn ListNode<T>::get_data() -> const T& {
        return this->_data;
    }


    template<Movable T>
    inline Fn ListNode<T>::get_mut_data() -> T& {
        return this->_data;
    }


    template<Movable T>
    inline Fn ListNode<T>::set_next(MPtr next) -> Void {
        this->_next = (ListNode<T>*) next;
    }


    template<Movable T>
    inline Fn ListNode<T>::get_next() -> MPtr const {
        return (MPtr) this->_next;
    }


    template<Movable T>
    inline Fn ListNode<T>::set_prev(MPtr prev) -> Void {
        this->_prev = (ListNode<T>*) prev;
    }


    template<Movable T>
    inline Fn ListNode<T>::get_prev() -> MPtr const {
        return (MPtr) this->_prev;
    }


    template<Movable T>
    inline Fn ListNode<T>::operator =(ListNode&& obj) -> Void {
        this->_data = obj._data;
        this->_next = obj._next;
        this->_prev = obj._prev;
        obj.drop();
    }
        
    
    /* ############################################################################################################################### */
    
    template<Movable T, Allocator A = DefaultAllocator>
    class List: private Mover, private CanBeCopied<List<T, A>>, Dropper {
        Option<RawPtr<ListNode<T>>>  _head;
        Option<RawPtr<ListNode<T>>>  _tail;
        MSize                        _len;

        List(Option<RawPtr<ListNode<T>>> head, Option<RawPtr<ListNode<T>>> tail, Size len): 
            _head(move_obj(head)), 
            _tail(move_obj(tail)), 
            _len(len) 
        { }


      public:
        List() { }

        List(List&& obj): _head(move_obj(obj._head)), _tail(move_obj(obj._tail)), _len(obj._len) {
            obj.drop();
        }

        static inline Fn create() -> List;

        inline Fn copy() -> List;

        inline Fn push_back(T data) -> EResult<$>;

        UNSAFE inline Fn unlink_node(RawPtr<ListNode<T>> node);

        inline Fn pop_back() -> Option<T>;
        
        inline Fn is_empty() -> MBool;

        inline Fn len() -> MSize;

        inline Fn append() -> EResult<$>;

        inline Fn operator =(List&& obj) -> Void;

        static Fn test() -> $;
    };

    Let test = List<$>::test();


    /* Self Implementation for List */

    template<Movable T, Allocator A>
    inline Fn List<T, A>::create() -> List {
        return List(optNone(RawPtr<ListNode<T>>), optNone(RawPtr<ListNode<T>>), 0);
    }


    template<Movable T, Allocator A>
    inline Fn List<T, A>::copy() -> List {
        return List(move_obj(this->_head), move_obj(this->_tail), this->_len);
    }


    template<Movable T, Allocator A>
    inline Fn List<T, A>::is_empty() -> MBool {
        return this->_len == 0;
    }


    template<Movable T, Allocator A>
    inline Fn List<T, A>::len() -> MSize {
        return this->_len;
    }


    template<Movable T, Allocator A>
    inline Fn List<T, A>::push_back(T data) -> EResult<$> {
        ListNode<T>* node = new ListNode<T>(move_obj(data));      // TODO: change this to use `malloc` instead of `new`

        if (self._tail.is_some()) {
            this->_tail.unwrap()->_next = optSome(RawPtr<ListNode<T>>, node);
        }
        else {
            this->_head = optSome(RawPtr<ListNode<T>>, node);
        }
        
        node->_prev = move_obj(this->_tail);
        this->_tail = optSome(RawPtr<ListNode<T>>, node);
        this->_len++;

        return resOk($, _);
    }


    template<Movable T, Allocator A>
    inline Fn List<T, A>::unlink_node(RawPtr<ListNode<T>> node) {
        ListNode<T>* _node = node.as_mut_ptr();

        if (_node->_prev.is_some()) {
            _node->_prev.unwrap()->_next = move_obj(_node->_next);
        }
        else {
            this->_head = move_obj(_node->_next);
        }

        if (_node->_next.is_some()) {
            _node->_next.unwrap()->_prev = move_obj(_node->_prev);
        }
        else {
            this->_tail = move_obj(_node->_prev);
        }
    }


    template<Movable T, Allocator A>
    inline Fn List<T, A>::pop_back() -> Option<T> {
        if (this->_tail.is_none()) {
            return optNone(T);
        }
        else {
            Let item = optSome(T, ((ListNode<T>*) this->_tail.unwrap().as_ptr())->get_data());
            this->_tail = move_obj(((ListNode<T>*) this->_tail.unwrap().as_ptr())->_prev);
            this->_len--;
            return item;
        }
    }


    template<Movable T, Allocator A>
    inline Fn List<T, A>::append() -> EResult<$> {

    }


    template<Movable T, Allocator A>
    inline Fn List<T, A>::operator =(List&& obj) -> Void {
        this->_head = move_obj(obj._head);
        this->_tail = move_obj(obj._tail);
        this->_len  = obj._len;
        obj.drop();
    }
} // namespace collections 
} // namespace stl


namespace stl::collections {
    
    template<Movable T, Allocator A>
    Fn List<T, A>::test() -> $ {
        ADD_TEST(list_test_creation, []() -> MBool {
            Let list = List<const char*>::create();
            list.push_back("Salam");
            list.push_back("Mostafa");
            list.push_back("Khoobi?");

            Let a = list.pop_back();
            Let b = list.pop_back();
            Let c = list.pop_back();
            Let d = list.pop_back();
            
            Let t = optSome(int, 5);
            t.take();

            return true;
        });


        ADD_TEST(list_test_unlink_node, []() -> MBool {
            // Let list = List<const char*>::create();
            // list.push_back("Salam");

            // Let node = ListNode<const char*>::create("Mosi");
            // Let ptr  = RawPtr<ListNode<const char*>>::from_ptr(&node);
            // unsafe (
            //     list.unlink_node(move_obj(ptr));
            // )
            return true;
        });


        // static_assert(IsDerived<Mover, ListNode<char>>);

        return _;
    }

} // namespace name



#endif /* _COLLECTIONS_LIST_INCLUDE_LIST_ */
