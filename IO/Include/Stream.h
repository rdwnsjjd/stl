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

#ifndef _FRAMEWORK_STL_IO_INCLUDE_STREAM_H_
#define _FRAMEWORK_STL_IO_INCLUDE_STREAM_H_

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/OwnerShip.h"
// #include "Framework/Primitives/Error.h"
// #include "Framework/Primitives/Result.h"

#include "./Write.h"
#include "./Read.h"

namespace stl { namespace io {

    using namespace framework;
    using namespace framework::basics;

    class Stream: public Mover, public Dropper, public Writer, public Reader {
      private:
        typedef struct timeval Timeval;

      protected:
        MInt32   _fd;

        inline Stream();

        inline ~Stream();

        inline Stream(Int32 fd);

        inline Stream(Stream& obj) = delete;

        inline Stream(Stream&& obj);

        inline Fn write(Bytes buf, Size size) -> MSSize override;

        inline Fn read(MBytes buf, Size size, Size timeout) -> MSSize override;

        inline Fn drop() -> Void override;

        inline Fn operator =(Stream&& obj) -> Void;
    };


    /* Self Implementation for Stream */

    inline Stream::Stream() { 
        /* nothing */
    }


    inline Stream::Stream(Int32 fd): _fd(fd) { 
        /* nothing */
    }


    inline Stream::Stream(Stream&& obj): _fd(obj._fd) { 
        /* nothing */
    }


    inline Stream::~Stream() { 
        // if (!this->check_move()) {
            close(this->_fd);
        // }
        
    }



    /* Reader Implementation for Stream */

    inline Fn Stream::write(Bytes buf, Size size) -> MSSize {
        return ::write(this->_fd, buf, size);
    };



    /* Writer Implementation for Stream */

    inline Fn Stream::read(MBytes buf, Size size, Size timeout_micro = 0) -> MSSize {
        fd_set  set;
        Timeval _timeout;

        FD_ZERO(&set);
        FD_SET(this->_fd, &set);

        _timeout.tv_sec  = 0;
        _timeout.tv_usec = timeout_micro;

        Let select_res = ::select(this->_fd + 1, &set, NULL, NULL, timeout_micro == 0 ? NULL : &_timeout);

        // assertRetOs(select_res != -1, -1, "Failed to wait on socket to read from it!");
        // if (select_res == 0) {
        //     debugLog("Reading from socket `%d` timed out!", this->_fd);
        //     return 0;
        // }
        

        return ::read(this->_fd, buf, size);
    };



    /* Dropper Implementation for Stream */

    inline Fn Stream::drop() -> Void {
        // ...
    };


    /* Operation Overloading for Stream */

    inline Fn Stream::operator =(Stream&& obj) -> Void {
        this->_fd = obj._fd;
    }
    
} // namespace io
} // namespace stl 

#endif // _FRAMEWORK_STL_IO_INCLUDE_STREAM_H_
