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

#ifndef _STL_ITER_INCLUDE_ITERATOR_
#define _STL_ITER_INCLUDE_ITERATOR_


#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/BasicConcepts.h"
#include "Framework/STL/Mem/Alloc/Include/Slice.h"
#include "../../Collections/Pair/Inc"

#include "Test/Include/Test.h"
using namespace test;

namespace stl { namespace iter {

    using namespace framework;
    using namespace framework::basics;
    using namespace framework::concepts;
    using namespace stl::collections;
    using namespace stl::mem;
    
    template <Movable M>
    class Iterator {
      protected:
        M* current;
        
      public:
        virtual inline Fn next() -> Option<M> = 0;

        /**
         * This is a c++ iterator compatible. is used to for an 
         * object to be iterable in c++ for-in loop. this function
         * should return a pointer to the first element of iterable 
         * object. also this is necessary to access API's that
         * needs pointer of elements to it.
         */
        virtual inline Fn begin() -> M* = 0;

        /**
         * This is a c++ iterator compatible. is used to for an 
         * object to be iterable in c++ for-in loop. this function
         * should return a pointer to the last element of iterable 
         * object.
         */
        virtual inline Fn end() -> M* = 0;

        virtual inline Fn advanced_by(Idx idx) -> Option<M>;
        
        virtual inline Fn nth(Idx idx) -> Option<M>;

        virtual inline Fn count() -> MSize;

        virtual inline Fn last() -> Option<M>;

        virtual inline Fn size_hint() -> Pair<MSize, Option<MSize>>;

        static Fn test() -> $;

        // TODO
        // virtual inline Map<M> map() = 0;  
        // virtual inline Map<M> foreach() = 0;  
        // virtual inline Map<M> filter() = 0;  
        // virtual inline Map<M> enumerate() = 0;  
        
    };


    /* Self Implementations for Iterator */

    template<Movable M> 
    inline Fn Iterator<M>::advanced_by(Idx idx) -> Option<M> {
        for (MIdx i = 0; i < idx; i++) {
            this->next();
        }
        return optSome(M, move_obj(*(this->current)));
    }


    template<Movable M> 
    inline Fn Iterator<M>::nth(Idx idx) -> Option<M> {
        this->advanced_by(idx);
        return this->next();
    }


    template<Movable M> 
    inline Pair<MSize, Option<MSize>> Iterator<M>::size_hint() {
        return Pair<MSize, Option<MSize>>::create(0, optNone(MSize));
    }


    template<Movable M> 
    inline Fn Iterator<M>::count() -> MSize {
        Let count = 0;
        Let ptr   = this->begin();

        if (ptr == NULL) {
            return 0;
        }

        Loop {
            Let res = this->next();
            if (res.is_none()) {
                break;
            }
            count++;
        }
        return count;
    }


    template<Movable M> 
    inline Fn Iterator<M>::last() -> Option<M> {
        MPtr last = NULL;
        Let  ptr  = this->begin();

        if (ptr == NULL) {
            return optNone(M);
        }

        Loop {
            Let res = this->next();
            if (res.is_none()) {
                break;
            }
            last = ptr;
            ptr++;
        }
        return optSome(M, *last);
    }

    Let test = Iterator<$>::test();

    /* Concepts */

    template <typename T>
    concept Iterable = IsDerived<Iterator, T>;


} // namespace iter
} // namespace stl 



template<Movable T>
Fn stl::iter::Iterator<T>::test() -> $ {


    return _;
}



#endif /* _STL_ITER_INCLUDE_ITERATOR_ */
