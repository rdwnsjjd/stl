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

#ifndef _FRAMEWORK_STL_SYS_IPC_INCLUDE_SHAREDFILE_H_
#define _FRAMEWORK_STL_SYS_IPC_INCLUDE_SHAREDFILE_H_

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "Framework/Basics/Inc"
#include "Framework/STL/String/Inc"
#include "Framework/STL/IO/Inc"
#include "Framework/STL/Sys/Fs/Inc"

using namespace framework::basics;
using namespace stl;
using namespace stl::io;

namespace stl { namespace sys { namespace ipc {

    using stl::sys::fs::FileAccessMode;

    struct SharedFileHeader: public Mover {
      private:
        MUInt32 _version;
        MSize   _max_conn;
        MSize   _already_connected;

        friend class SharedFileContent;

      protected:
        SharedFileHeader(MSize max = 20, MInt32 version = 0):
            _version(version), 
            _max_conn(max), 
            _already_connected(0) 
        { }

      public:
        SharedFileHeader(SharedFileHeader&& obj):
            _version(obj._version), 
            _max_conn(obj._max_conn), 
            _already_connected(obj._already_connected) 
        { }

        Fn operator =(SharedFileHeader&& obj) -> Void {
            this->_version  = obj._version;
            this->_max_conn = obj._max_conn;

            this->_already_connected = obj._already_connected;
        }


    };


    struct SharedFileContent: public Mover {
      private:
        SharedFileHeader _header;
        MBytes           _body;

        friend class SharedFile;

      protected:
        SharedFileContent(MSize max, MInt32 version): 
            _header(max, version),
            _body() 
        { }

        template<Serializable S> inline Fn add_body(S& body) -> Void;

      public:
        SharedFileContent(SharedFileContent&& obj):
            _header(moveObj(obj._header)), 
            _body(obj._body)
        { }

        Fn operator =(SharedFileContent&& obj) -> Void {
            this->_header = moveObj(obj._header);
            this->_body   = obj._body;
        }
    };



    /* Self Implementation for SharedFileContent */

    template<Serializable S T> 
    inline Fn SharedFileContent::add_body(S& body) -> Void {
        // body
        memcpy(this->_body, body.serialize(), sizeof(body));
    };



    class SharedFile: public Stream {
      private:
        SharedFile(Int32 fd): Stream(fd) { }

      public:
        static inline Fn create(Str path) {
            Int32 fd = ::creat(path, (mode_t) FileAccessMode::ReadWrite);
            debugLog("%s", strerror(errno));
            // TODO(emad): handle errors and return result

            return SharedFile(fd);
        }


        static inline Fn connect(Str path) {
            //
        }


        inline Fn bind(MSize max = 20, MInt32 version = 0) {
            Let cntnt = SharedFileContent(max, version);

            this->write(cntnt.serialize(), )
        }


        static inline Fn listen() {
            //
        }
    };
    
} // namespace ipc 
} // namespace sys 
} // namespace stl 



#endif // _FRAMEWORK_STL_SYS_IPC_INCLUDE_SHAREDFILE_H_
