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
#include "../../Collections/Pair/Inc"

namespace stl { namespace iter {

    using namespace framework;
    using namespace framework::basics;
    using namespace framework::concepts;
    using namespace stl::collections;
    
    template <Movable M>
    class Iterator {
      protected:
        M* current;
        
      public:
        // virtual inline Fn next() -> Option<M> = 0;
        virtual inline Fn next() -> M = 0;

        virtual inline Fn begin() -> M* = 0;

        virtual inline Fn end() -> M* = 0;

        // virtual inline Fn advanced_by(Idx idx) -> Option<M>;
        virtual inline Fn advanced_by(Idx idx) -> M;
        
        // virtual inline Fn nth(Idx idx) -> Option<M>;
        virtual inline Fn nth(Idx idx) -> M;


        virtual inline Fn count() -> MSize;

        virtual inline Fn last() -> M;

        // TODO
        // virtual inline Map<M> map() = 0;  
        // virtual inline Map<M> foreach() = 0;  
        // virtual inline Map<M> filter() = 0;  
        // virtual inline Map<M> enumerate() = 0;  
        // virtual inline Pair<MSize, Option<MSize>> size_hint();
        
    };


    /* Self Implementations for Iterator */

    template<Movable M> 
    inline Fn Iterator<M>::advanced_by(Idx idx) -> M {
        for (MIdx i = 0; i < idx; i++) {
            this->next();
        }
        // return Option<M>::Some(moveObj(*(this->current)));
        // return moveObj(*(this->current));
    }


    template<Movable M> 
    inline Fn Iterator<M>::nth(Idx idx) -> M {
        return this->advanced_by(idx);
    }


    // template<Movable M> 
    // inline Pair<MSize, Option<MSize>> Iterator<M>::size_hint() {
    //     return Pair<MSize, Option<MSize>>::create(0, Option<MSize>::None);
    // }


    template<Movable M> 
    inline Fn Iterator<M>::count() -> MSize {
        Mut count = 0;
        // Loop {
        //     Mut res = this->next();
        //     if (res.is_none()) {
        //         break;
        //     }
        //     count++;
        // }
        return count;
    }


    template<Movable M> 
    inline Fn Iterator<M>::last() -> M {
        // TODO
        // return moveObj(*(this->current));
    }


    /* Concepts */

    template <typename T>
    concept Iterable = IsDerived<Iterator, T>;


} // namespace iter
} // namespace stl 


#endif /* _STL_ITER_INCLUDE_ITERATOR_ */
