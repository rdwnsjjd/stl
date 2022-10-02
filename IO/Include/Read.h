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

#ifndef _FRAMEWORK_STL_IO_INCLUDE_READ_H_
#define _FRAMEWORK_STL_IO_INCLUDE_READ_H_

#include "Framework/Basics/Inc"

using namespace framework::basics;

namespace stl { namespace io {

    class Reader {
      
      public:
        virtual inline Fn read(MBytes buf, Size size, Size timeout) -> MSSize = 0;
    };


    template<typename T>
    concept Readable = std::is_base_of_v<Reader, T> && requires(T obj, MBytes buf, Size size) {
        obj.read(buf, size);
    };
    
} // namespace io
} // namespace stl 

#endif // _FRAMEWORK_STL_IO_INCLUDE_READ_H_
