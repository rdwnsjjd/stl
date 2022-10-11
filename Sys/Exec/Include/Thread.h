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

#ifndef _SYS_EXEC_INCLUDE_THREAD_
#define _SYS_EXEC_INCLUDE_THREAD_

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "Framework/Basics/BasicDefs.h"
#include "Framework/Basics/BasicTypes.h"
#include "Framework/STL/Mem/Alloc/Inc"
#include "Test/Include/Test.h"

#include "Framework/STL/Sys/Sync/Include/Mutex.h"
#include "Framework/STL/Sys/Sync/Include/Arc.h"

using namespace stl::sys::sync;

namespace stl { namespace sys { namespace exec {

    using namespace framework::basics;
    using namespace stl::mem::alloc;

    template<Movable T, typename F>
    struct ThreadArg {
        T data;
        F (*callback)(T);
    };

    struct ThreadArgVoid {
        Void (*callback)();
    };

    template<Movable T, typename F>
    Fn thread_main(MPtr handle) -> MHandle {
        CONST_IF (isSame(T, $)) {
            ThreadArgVoid* arg = (ThreadArgVoid*) handle;
            arg->callback();
            DefaultAllocator().deallocate(arg);
            return NULL;
        }
        CONST_ELSE {
            ThreadArg<T, F>* arg = (ThreadArg<T, F>*) handle;
            F res = arg->callback(move_obj(arg->data));

            Let mem = (char*) malloc(sizeof(F));
            memcpy(mem, &res, sizeof(F));
            DefaultAllocator().deallocate(arg);
            return (MHandle) mem;
        }
    }


    template<Movable T>
    class JoinHandle {
      public:
        typedef pthread_t PthreadHandle;
        PthreadHandle _handle;
        friend class Thread;

      protected:
        JoinHandle(PthreadHandle pth): _handle(pth) { }

      public:
        inline Fn join() -> T {
            T* th_return = NULL;
            pthread_timedjoin_np(this->_handle, (MHandle*) &th_return, NULL);

            CONST_IF (!isSame(T, $)) {
                Let t = T();
                memcpy((MPtr) &t, th_return, sizeof(T));
                DefaultAllocator().deallocate(th_return);
                return t;
            }
            CONST_ELSE {
                return _;
            }
        }

        // inline Fn try_join(Size timeout_msec) -> Boxed<T> {
        //     T* th_return = NULL;
        //     Let timeout  = (struct timespec) { .tv_sec = 2, .tv_nsec = 0 };
        //     pthread_timedjoin_np(this->_handle, (MHandle*) &th_return, &timeout);

        //     return Boxed<T>::create(th_return);
        // }
    };


    class Thread {
        typedef pthread_t PthreadHandle;

      public:
        template<typename F> 
        static inline Fn spawn(F (*callback)()) -> JoinHandle<std::conditional_t<isSame(F, Void), $, F>> {
            PthreadHandle pth = 0;
            Let arg = (ThreadArgVoid*) DefaultAllocator().allocate(1, sizeof(ThreadArgVoid)).unwrap();
            arg->callback = callback;

            pthread_create(&pth, NULL, thread_main<$, F>, arg);
            CONST_IF (isSame(F, Void)) {
                return JoinHandle<$>(pth);
            } 
            CONST_ELSE {
                return JoinHandle<F>(pth);
            }
        }

        template<Movable T, typename F> 
        static inline Fn spawn(T data, F (*callback)(T)) -> JoinHandle<std::conditional_t<isSame(F, Void), $, F>> {
            PthreadHandle pth = 0;
            Let arg = (ThreadArg<T, F>*) DefaultAllocator().allocate(1, sizeof(ThreadArg<T, F>)).unwrap();
            arg->callback = callback;
            arg->data     = move_obj(data);

            pthread_create(&pth, NULL, thread_main<T, F>, arg);
            CONST_IF (isSame(F, Void)) {
                return JoinHandle<$>(pth);
            } 
            CONST_ELSE {
                return JoinHandle<F>(pth);
            }
        }
        
        static Fn test() -> $;
    };

    Let test = Thread::test();
} // namespace exec 
} // namespace sys 
} // namespace stl 


Fn stl::sys::exec::Thread::test() -> $ {
    
    // global_tester.add_test(TestBase::create("thread_test_create_with_unit_arg_and_unit_ret", []() -> MBool {
    //     Let thread = Thread::spawn<Void>([]() {
    //         printf("This is a simplest thread ever!\n");
    //     });

    //     pthread_join(thread._handle, nullptr);
    //     Let res = thread.join();
    //     evalEqIntLog(res, _);
    // }));

    
    // global_tester.add_test(TestBase::create("thread_test_create_with_int_arg_and_int_ret", []() -> MBool {
    //     Let thread = Thread::spawn<MInt32, MInt32>(34, [](MInt32) -> MInt32{
    //         printf("This is a int arg and return thread!\n");
    //         return -74;
    //     });

    //     Let res = thread.join();
    //     evalEqIntLog(res, -74);
    // }));

    
    // global_tester.add_test(TestBase::create("thread_test_create_with_int_arg_and_mutex_ret", []() -> MBool {
    //     Let thread = Thread::spawn<MInt32, Mutex<MInt32>>(5, [](MInt32) -> Mutex<MInt32> {
    //         Let mutex = Mutex<MInt32>::create(54);
    //         mutex.lock().unwrap();
    //         return move_obj(mutex);
    //     });
    //     Let ret = thread.join().lock().unwrap();
    //     evalEqIntLog(*ret, 54);
    // }));

    
    // global_tester.add_test(TestBase::create("thread_test_create_with_mutex_arg_and_int_ret", []() -> MBool {
    //     Let mutex = Mutex<MInt32>::create(54);
    //     Let thread = Thread::spawn<Mutex<MInt32>, MInt32>(move_obj(mutex), [](Mutex<MInt32>) -> MInt32 {
    //         Let mutex = Mutex<MInt32>::create(54);
    //         Let num = mutex.lock().unwrap();
    //         return *num;
    //     });

    //     Let num = mutex.lock().unwrap();
    //     Let res = thread.join();
    //     evalEqIntLog(res, *num);
    // }));

    
    global_tester.add_test(TestBase::create("thread_test_create_with_arc_mutex_arg_and_int_ret", []() -> MBool {
        Let mutex = Mutex<MInt32>::create(54);
        Let arc   = Arc<Mutex<MInt32>>::create(move_obj(mutex)).unwrap();

        Let arc_copied = arc.copy();
        Let thread1 = Thread::spawn<Arc<Mutex<MInt32>>, MInt32>(move_obj(arc), [](Arc<Mutex<MInt32>> arc) -> MInt32 {
            Let locked = (*arc).lock().unwrap();
            sleep(10);
            return *locked;
        });

        // Let thread2 = Thread::spawn<Arc<Mutex<MInt32>>, MInt32>(move_obj(arc_copied), [](Arc<Mutex<MInt32>> arc) -> MInt32 {
        //     Let locked = (*arc).lock().unwrap();
        //     sleep(10);
        //     return *locked;
        // });

        Let num = mutex.lock().unwrap();
        Let res = thread1.join();
        // Let res2 = thread2.join();
        evalEqIntLog(res, *num);
    }));

    return _;
}


#endif /* _SYS_EXEC_INCLUDE_THREAD_ */
