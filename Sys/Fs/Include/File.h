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

#ifndef _SYS_FS_INCLUDE_FILE_
#define _SYS_FS_INCLUDE_FILE_

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "Framework/Basics/BasicTypes.h"
#include "Framework/STL/IO/Inc"
#include "Framework/STL/String/Inc"
#include "Framework/STL/Collections/Vec/Include/Vec.h"

namespace stl { namespace sys { namespace fs {
    
    using namespace framework::basics;
    using namespace stl;
    using namespace stl::io;
    using namespace stl::collections;

    enum class FileAccessMode {
        ReadOnly  = O_RDONLY,
        WriteOnly = O_WRONLY,
        ReadWrite = O_RDWR,
        Append    = O_APPEND
    };


    class File: public Stream {
      private:
        FileAccessMode _mode;

        inline File(Int32 fd, FileAccessMode mode);

        inline File(File&) = delete;

      public:
        inline File(File&&);

        static inline Fn create(Str path, UInt32 perm = 0777) -> File;

        static inline Fn open(Str path, FileAccessMode mode = FileAccessMode::ReadOnly) -> File;

        inline Fn get_cursor(Idx idx) -> MInt64;

        inline Fn set_cursor(Idx idx) -> MInt64;

        inline Fn fsize() -> MSize;

        // inline Fn write(Bytes buf, Size size) -> MSize;

        inline Fn write_from_vec(Vec<MUInt8>& vec) -> MSize;

        // inline Fn read(MBytes buf, Size size) -> MSize;

        inline Fn read_to_vec(Vec<MUInt8>& vec) -> MSize;

        inline Fn close() -> MSize;

        inline Fn operator =(File&& obj) -> Void;
    };


    /* Self Implementations */

    inline File::File(Int32 fd, FileAccessMode mode): Stream(fd), _mode(mode) {
        /* nothing */
    }


    inline File::File(File&& obj): Stream(obj._fd), _mode(obj._mode) {
        // nothing
    }


    inline Fn File::create(Str path, UInt32 perm) -> File {
        Let fd = creat(path, perm);
        return File(fd, FileAccessMode::ReadWrite);
    }


    inline Fn File::open(Str path, FileAccessMode mode) -> File {
        Let fd = ::open(path, (MUInt32) mode);
        return File(fd, mode);
    }


    inline Fn File::get_cursor(Idx idx) -> MInt64 {
        return lseek(this->_fd, idx, SEEK_CUR);
    }


    inline Fn File::set_cursor(Idx idx) -> MInt64 {
        Let last_cur = lseek(this->_fd, 0L, SEEK_CUR);
        Let size = lseek(this->_fd, 0L, SEEK_END);
        
        if (idx > size) {
            debugErr("Failed to set cursor to position %ld while file size is %ld!", idx, size);
            lseek(this->_fd, last_cur, SEEK_SET);
            return -1;
        }

        lseek(this->_fd, idx, SEEK_SET);
        return idx;
    }


    // inline Fn File::write(Bytes buf, Size size) -> MSize {
    //     return this->Stream::write(buf, size);
    // }


    // inline Fn File::read(MBytes buf, Size size) -> MSize {
    //     return this->Stream::read(buf, size);
    // }


    inline Fn File::write_from_vec(Vec<MUInt8>& vec) -> MSize {
        return this->Stream::write((Bytes) vec.as_ptr(), vec.len());
    }


    inline Fn File::read_to_vec(Vec<MUInt8>& vec) -> MSize {
        Let size = this->fsize();
        Let ptr  = DefaultAllocator().allocate(size, 1);

        vec  = Vec<MUInt8>::from((MUInt8*) ptr.unwrap(), size);
        return this->Stream::read((MBytes) vec.as_ptr(), size);
    }


    inline Fn File::fsize() -> MSize {
        Let cur  = lseek(this->_fd, 0L, SEEK_CUR);
        Let size = lseek(this->_fd, 0L, SEEK_END);
        lseek(this->_fd, cur, SEEK_SET);
        return size;
    }


    /* Operation Overloading */
    
    inline Fn File::operator =(File&& obj) -> Void {
        this->_mode = obj._mode;
    }
    
} // namespace fs 
} // namespace sys 
} // namespace stl 


#endif /* _SYS_FS_INCLUDE_FILE_ */
