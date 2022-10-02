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

#include "../Include/Thread.h"

namespace stl { namespace sys { namespace exec {

    // template<Movable M, typename F>
    // struct ThreadArg {
    // //   protected:
    //     M data;
    //     F callback;

    //     // friend class Thread;
    // };


    // template<Movable M, typename F>
    // Fn thread_main(MPtr handle) -> MHandle {
    //     ThreadArg<M, F>* arg = (ThreadArg<M, F>*) handle;
    //     arg->callback(arg->data);
    //     return NULL;
    // }



    // /* Thread Self Implementation */

    // template<Movable M, typename F> 
    // Fn Thread::spawn(M data, F callback) -> Thread {
    //     PthreadHandle pth = 0;
    //     Let arg = (ThreadArg<M, F>) {
    //         .data     = data,
    //         .callback = callback
    //     };

    //     pthread_create(&pth, NULL, thread_main<M, F>, &arg);
    //     return Thread(pth);
    // }


    // inline Fn Thread::join() -> Void {
    //     pthread_join(this->handle, NULL);
    // }
    
} // namespace exec 
} // namespace sys 
} // namespace stl 