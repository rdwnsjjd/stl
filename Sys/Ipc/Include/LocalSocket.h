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

#ifndef _FRAMEWORK_STL_SYS_NET_INCLUDE_LOCALSTREAM_H_
#define _FRAMEWORK_STL_SYS_NET_INCLUDE_LOCALSTREAM_H_

#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "Framework/Basics/Inc"
#include "Framework/STL/IO/Inc"

using namespace framework::basics;
using namespace stl::io;

namespace stl { namespace sys { namespace ipc {

    #define LOCAL_STREAM_PATH   "tmp/9Lq7BNBnBycd6nxy.socket"

    class LocalSocket: public Stream {
      private:
        using SockAddrUn = struct sockaddr_un;
        using Hostent    = struct hostent;

        SockAddrUn _path;
    
        inline LocalSocket(Int32 sockfd): Stream(sockfd), _path({ 0 }) { };
    
        inline LocalSocket(Int32 sockfd, SockAddrUn addr): Stream(sockfd), _path(addr) { };

      public:
        inline ~LocalSocket() = default;

        inline LocalSocket(LocalSocket&& obj): Stream(obj._fd), _path(obj._path) { };

        static inline Fn create() -> LocalSocket;

        inline Fn connect(Str path = LOCAL_STREAM_PATH) -> EResult<Int32>;

        inline Fn bind(Str path = LOCAL_STREAM_PATH) -> EResult<Int32>;

        inline Fn listen(UInt32 max_conn) -> EResult<LocalSocket>;

        inline Fn send() -> EResult<UInt32>;

        inline Fn recv() -> EResult<UInt32>;

        inline Fn operator =(LocalSocket&& obj) -> Void;
    };


    /* Self Implementation for LocalSocket */

    inline Fn LocalSocket::create() -> LocalSocket {
        Let sockfd = ::socket(AF_LOCAL, SOCK_STREAM, 0);
        debugLog("%s", strerror(errno));
        // TODO(emad): handle error

        return LocalSocket(sockfd);
    }


    inline Fn LocalSocket::connect(Str path) -> EResult<Int32> {
        return this->bind(move_obj(path));
    }


    inline Fn LocalSocket::bind(Str path) -> EResult<Int32> {
        memset(&this->_path, 0, sizeof(this->_path));
        
        this->_path.sun_family = AF_LOCAL;
        strncpy(this->_path.sun_path, path, sizeof(this->_path.sun_path) - 1);

        Let res = ::bind(this->_fd, (const struct sockaddr*) &this->_path, sizeof(this->_path));
        debugLog("%s", strerror(errno));
        // TODO(emad): handle errors and return result

        return EResult<Int32>::Ok(res);
    }


    inline Fn LocalSocket::listen(UInt32 max_conn = 20) -> EResult<LocalSocket> {
        Let res = ::listen(this->_fd, max_conn);
        debugLog("%s", strerror(errno));
        // TODO(emad): handle errors

        SockAddrUn client_addr = { 0 };
        Let caddr_len          = sizeof(client_addr);

        Let client_fd = ::accept(this->_fd,  (struct sockaddr*) &client_addr,  (socklen_t*)&caddr_len);
        debugLog("%s", strerror(errno));
        // TODO(emad): handle errors

        return EResult<LocalSocket>::Ok(move_obj(LocalSocket(client_fd, client_addr)));
    }


    inline Fn LocalSocket::send() -> EResult<UInt32> {

    }


    inline Fn LocalSocket::recv() -> EResult<UInt32> {
        
    }



    /* Operator Overloading for LocalSocket */


    inline Fn LocalSocket::operator =(LocalSocket&& obj) -> Void {
        this->_fd   = obj._fd;
        this->_path = obj._path;
    }
    
} // namespace ipc 
} // namespace sys 
} // namespace stl 

#endif // _FRAMEWORK_STL_SYS_NET_INCLUDE_LOCALSTREAM_H_
