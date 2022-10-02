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

#ifndef _FRAMEWORK_STL_SYS_NET_INCLUDE_TCPSTREAM_H_
#define _FRAMEWORK_STL_SYS_NET_INCLUDE_TCPSTREAM_H_


#include <netdb.h>
#include <sys/socket.h>
#include <linux/if_ether.h>

#include "Framework/Basics/Inc"
#include "Framework/STL/IO/Inc"

using namespace framework::basics;
using namespace stl::io;

Const DOMAIN = framework::Domain::Sys;

namespace stl { namespace net {

    class TcpStream: public Stream {
      private:
        using SockAddrIn = struct sockaddr_in;
        using Hostent    = struct hostent;

        SockAddrIn _addr;

        inline TcpStream(TcpStream& obj) = delete;
    
        inline TcpStream(Int32 sockfd): Stream(sockfd), _addr({ 0 }) { };
    
        inline TcpStream(Int32 sockfd, SockAddrIn addr): Stream(sockfd), _addr(addr) { };
    
      public:
        inline TcpStream(): Stream(), _addr({ 0 }) { };

        inline TcpStream(TcpStream&& obj): Stream(obj._fd), _addr(obj._addr) { };

        static inline Fn create() -> TcpStream;

        inline Fn connect(UInt32 portno, String hostname, SSize attempt = 0, Size timeout_msec = 0) -> EResult<MInt32>;

        inline Fn bind(UInt32 portno, SSize attempt = 0, Size timeout_msec = 0) -> EResult<TcpStream*>;

        template<typename Func> inline Fn listen(Func callback, UInt32 max_conn = 20) -> EResult<__>;

        inline Fn send(Bytes data, Size size) -> EResult<MUInt32>;

        inline Fn recv(MBytes data, Size size, Size timeout = 0) -> EResult<MUInt32>;

        inline Fn operator =(TcpStream&& obj) -> Void;
    };


    /* Self Implementation for TcpStream */

    inline Fn TcpStream::create() -> TcpStream {
        Let sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        assertRetOs(
            sockfd != -1,
            { 0 }, 
            "Failed to create AF_INET socket for creating TcpStream object!"
        );

        return TcpStream(sockfd);
    }


    inline Fn TcpStream::connect(UInt32 portno, String hostname, SSize attempt, Size timeout_msec) -> EResult<MInt32> {
        Hostent* server_addr = ::gethostbyname(hostname.as_str());
        memcpy(server_addr->h_addr, (Str) &this->_addr.sin_addr.s_addr, server_addr->h_length);

        this->_addr.sin_family = AF_INET;
        this->_addr.sin_port   = ::htons(portno);

        Let res = ::connect(this->_fd, (const struct sockaddr*) server_addr, sizeof(*server_addr));
        assertRetOs(
            res != -1,
            EResult<MInt32>::err(Error(EK::ConnectionFailure)), 
            "Failed to connect to host using this socket: `%d`!", this->_fd
        );

        return EResult<MInt32>::Ok(res);
    }


    inline Fn TcpStream::bind(UInt32 portno, SSize attempt, Size timeout_msec) -> EResult<TcpStream*> {
        this->_addr.sin_addr.s_addr = INADDR_ANY;
        this->_addr.sin_family      = AF_INET;
        this->_addr.sin_port        = ::htons(portno);

        Let failed_attempt = attempt != -1 ? attempt : INF_U64;
        Loop {
            Let res = ::bind(this->_fd, (const struct sockaddr*) &this->_addr, sizeof(this->_addr));
            if (res == -1) {
                debugErrOs("Failed to bind to port using this socket: `%d`!", this->_fd);
                if (failed_attempt == 0) {
                    return EResult<TcpStream*>::err(Error(DOMAIN, EK::BindingFailure));
                }

                failed_attempt--;
                usleep(timeout_msec * 1000);
                continue;
            }
            break;
        }

        return EResult<TcpStream*>::Ok(this);
    }


    template<typename Func>
    inline Fn TcpStream::listen(Func callback, UInt32 max_conn) -> EResult<__> {
        Let res = ::listen(this->_fd, max_conn);
        assertRetOs(
            res != -1,
            EResult<__>::err(Error(EK::ConnectionFailure)), 
            "Failed to listen to connections using this socket: `%d`!", this->_fd
        );

        SockAddrIn client_addr = { 0 };
        Let        caddr_len   = sizeof(client_addr);

        Let client_fd = ::accept(this->_fd,  (struct sockaddr*) &client_addr,  (socklen_t*)&caddr_len);
        assertRetOs(
            res != -1,
            EResult<__>::err(Error(EK::ConnectionFailure)), 
            "Failed to accept connections on this socket: `%d`!", this->_fd
        );

        callback(TcpStream(client_fd, client_addr));
        return EResult<__>::Ok($);
    }


    inline Fn TcpStream::send(Bytes data, Size size) -> EResult<MUInt32> {
        Let res = self.write(data, size);
        assertRetOs(
            res != -1, 
            EResult<MUInt32>::err(Error(EK::WritingFailure)), 
            "Failed to write data on socket: `%d`!", this->_fd
        );

        return EResult<MUInt32>::Ok(res);
    }


    inline Fn TcpStream::recv(MBytes data, Size size, Size timeout) -> EResult<MUInt32> {
        Let res = self.read(data, size, timeout);
        assertRetOs(
            res != -1, 
            EResult<MUInt32>::err(Error(EK::ReadingFailure)), 
            "Failed to read data from socket: `%d`!", this->_fd
        );

        return EResult<MUInt32>::Ok(res);
    }


    /* Operator overloading */

    inline Fn TcpStream::operator =(TcpStream&& obj) -> Void {
        this->_addr = obj._addr;
    }
    
} // namespace net    
} // namespace stl 


#endif // _FRAMEWORK_STL_SYS_NET_INCLUDE_TCPSTREAM_H_
