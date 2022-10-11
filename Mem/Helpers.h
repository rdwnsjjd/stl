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

#ifndef _MEM_ALLOC_INCLUDE_HELPERS_
#define _MEM_ALLOC_INCLUDE_HELPERS_

#include "string.h"

#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/BasicDefs.h"

namespace stl::mem {

    using namespace framework;
    using namespace framework::basics;


    template<typename T>
    static inline Fn mem_new() -> T* {
        return new T();
    }


    template<typename T>
    static inline Fn mem_new(T data) -> T* {
        return new T(data);
    }


    template<typename T>
    static inline Fn mem_delete(T* ptr) {
        delete ptr;
    }


    template<typename T>
    static inline Fn mem_size() -> MSize {
        return sizeof(T);
    }


    static inline Fn mem_copy(MPtr src, Ptr dst, Size size) -> Int32 {
        memcpy(src, dst, size);
        return 0;
    }
    
} // namespace stl::mem 


#endif /* _MEM_ALLOC_INCLUDE_HELPERS_ */
