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

#ifndef _FRAMEWORK_STL_MEM_SIZED_INCLUDE_SIZED_H_
#define _FRAMEWORK_STL_MEM_SIZED_INCLUDE_SIZED_H_

#include "Framework/Basics/BasicDefs.h"

namespace stl { namespace mem {
    
    class Sized {
      public:
        UNSTABLE_F("Sized")
        virtual inline Fn size() -> unsigned long int = 0;
    };


    template<typename T>
    concept Sizable = requires(T obj) {
        obj.size();
    }; // std::is_base_of_v<Sized, T>;

} // namespace mem 
} // namespace stl 


#endif // _FRAMEWORK_STL_MEM_SIZED_INCLUDE_SIZED_H_
