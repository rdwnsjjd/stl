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

#ifndef _SYS_SYNC_INCLUDE_ARC_
#define _SYS_SYNC_INCLUDE_ARC_



#include "./Concepts.h"
#include "Framework/Basics/BasicTypes.h"
#include "Framework/STL/Mem/Alloc/Inc"

namespace stl { namespace sys { namespace sync {

    using namespace framework;
    using namespace stl::mem::alloc;

    template<Movable T>
    class RawArc: public Mover {
        T     data;
        MSize counter = 1;

        RawArc(T data): data(move_obj(data)), counter(1) {  };

      protected:
        template<Movable U> friend class Arc;
        // template<Movable U> friend class Boxed;

        static inline Fn create(T data) -> RawArc {
            return RawArc(move_obj(data));
        } 

        inline Fn increase() -> Void {
            this->counter++;
        }

        inline Fn decrease() -> MBool {
            return (--this->counter == 0);
        }

        inline Fn operator *() -> T& {
            return static_cast<T&>(this->data);
        }

        inline Fn operator =(T&& data) -> Void {
            this->data = data;
        }

      public:
        inline RawArc(RawArc&& obj) { 
            this->data    = move_obj(obj.data);
            this->counter = obj.counter;
        };

        // inline Fn copy() -> RawArc {
        //     // return RawArc(*this->inner);
        // }

        inline Fn operator =(RawArc&& obj) -> Void {
            this->data    = move_obj(obj.data);
            this->counter = obj.counter;
        }

        inline ~RawArc() = default;
    };


    template<Movable T>
    class Arc final: public Mover, public CanBeCopied<Arc<T>>, Dropper {
        Boxed<RawArc<T>, DefaultAllocator> inner;

        Arc(Boxed<RawArc<T>, DefaultAllocator>&& inner):
            inner(move_obj(inner))
        { }

        Arc(RawArc<T>* raw_arc) {
            this->inner = raw_arc;
            (*this->inner).increase();
        }


      public:
        Arc() { }
        Arc(Arc&& obj) {
            this->inner = move_obj(obj.inner);
            obj.drop();
        };

        static inline Fn create(T data) -> EResult<Arc> {
            Let res = Boxed<RawArc<T>>::create(move_obj(RawArc<T>::create(move_obj(data))));
            unwrapResOrRetSame(Arc, res, "Failed to create new boxed object while creating new Arc object!");
            return resOk(Arc, Arc(move_obj(res.unwrap())));
        }

        inline Fn copy() -> Arc {
            return Arc(this->inner.as_ptr());
        }

        static inline Fn ab_create(T data) -> Arc {
            Let res = Boxed<RawArc<T>>::ab_create(move_obj(RawArc<T>::create(move_obj(data))));
            return Arc(move_obj(res));
        }

        inline Fn ab_copy() -> Arc {
            Let res = this->inner.copy().unwrap();
            return Arc(res.as_ptr());
        }

        inline Fn operator *() -> T& {
            return *(*this->inner);
        }

        inline Fn operator =(T&& data) -> Void {
            *this->inner = data;
        }

        inline Fn operator =(Arc&& obj) -> Void {
            this->inner = move_obj(obj.inner);
            obj.drop();
        }

        inline ~Arc() {
            if (!this->dropped && (*(this->inner)).decrease()) {
                this->inner.~Boxed();
            }
        }
    };
    
    
} // namespace sync 
} // namespace sys 
} // namespace stl 

#endif /* _SYS_SYNC_INCLUDE_ARC_ */
