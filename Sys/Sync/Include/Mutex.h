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

#ifndef _SYS_SYNC_INCLUDE_MUTEX_
#define _SYS_SYNC_INCLUDE_MUTEX_

#include <pthread.h>

#include "./Concepts.h"
#include "Framework/Basics/BasicTypes.h"

namespace stl { namespace sys { namespace sync {

    using namespace framework;

    class DefaultLocker final: public LockerBase, Dropper {
      private:
        using PthreadMutex = pthread_mutex_t;

        template <Movable T, Lockable U> friend class MutexValue;
        template <Movable T, Lockable U> friend class Mutex;
        
        PthreadMutex pth_mutex;

      protected:
        inline DefaultLocker() { 
            this->pth_mutex = { 0 };
        }

        inline Fn unlock() -> EResult<$> override {
            Let res = pthread_mutex_unlock(&this->pth_mutex);
            evalOrRetFailWithOsLog($, res != -1, "Failed to unlock pthread mutex!");
            return resOk($, _);
        };

      public:
        inline DefaultLocker(DefaultLocker&& obj) { 
            this->pth_mutex = obj.pth_mutex;
            obj.drop();
        }

        static inline Fn create() -> DefaultLocker {
            return DefaultLocker();
        }

        inline Fn lock() -> EResult<$> override {
            Let res = pthread_mutex_lock(&this->pth_mutex);
            evalOrRetFailWithOsLog($, res != -1, "Failed to lock pthread mutex!");
            return resOk($, _);
        };

        inline Fn operator =(DefaultLocker&& obj) -> Void {
            this->pth_mutex = obj.pth_mutex;
            obj.drop();
        }
        
        inline ~DefaultLocker() {
            if (this->dropped) {
                this->unlock();
            }
            
        };
    };


    template <Movable T, Lockable LockerT = DefaultLocker>
    class MutexValue: public Mover, CanBeCopied<T>, Dropper {
      private:
        T*       data;
        LockerT* locker;

        template <Movable U, Lockable V> friend class Mutex;

      protected:
        inline MutexValue(T* data, LockerT* locker): data(data), locker(locker) { }

      public:
        MutexValue() { };

        MutexValue(MutexValue&& obj) {
            this->data   = obj.data;
            this->locker = obj.locker;

            obj.drop();
        }

        inline Fn copy() -> T {
            primitiveOrNot(return *data;, return (*data).copy();)
        }

        inline Fn operator *() -> T& {
            return static_cast<T&>(*(this->data));
        }

        inline Fn operator =(T data) -> Void {
            *this->data = data;
        }

        inline Fn operator =(MutexValue&& obj) -> Void {
            this->data = obj.data;
            this->locker = obj.locker;

            obj.drop();
        }

        inline ~MutexValue() {
            if (!this->dropped) {
                locker->unlock();
            }
        }
    };


    template <Movable T, Lockable LockerT = DefaultLocker>
    class Mutex final: public Mover, private Dropper {
      private:
        T      data;
        LockerT locker;
    
      public:
        inline Mutex(T data): data(move_obj(data)), locker(DefaultLocker::create()) { /* nothing */ };
        inline Mutex() { /* nothing */ }

        inline Mutex(Mutex&& obj) {
            this->data   = move_obj(obj.data);
            this->locker = move_obj(obj.locker);

            obj.drop();
        }

        static inline Fn create(T data) -> Mutex {
            return Mutex(move_obj(data));
        }

        inline Fn lock() -> EResult<MutexValue<T>> {
            // evalOrRet(
            //     !this->dropped,
            //     resFail(MutexValue<T>, EK::ValueIsDropped),
            //     "Use of dropped value!"
            // );
            
            unwrapResOrRet(
                this->locker.lock(), 
                resFail(MutexValue<T>, EK::MutexFailure),
                "Failed to lock mutex locker!"
            );
            return resOk(MutexValue<T>, MutexValue(&this->data, &this->locker));
        }

        inline Fn operator =(Mutex&& obj) -> Void {
            this->data   = move_obj(obj.data);
            this->locker = move_obj(obj.locker);

            obj.drop();
        }

        inline ~Mutex() = default;
    };
    
    
} // namespace sync 
} // namespace sys 
} // namespace stl 


#endif /* _SYS_SYNC_INCLUDE_MUTEX_ */
