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

#ifndef _COLLECTIONS_HASH_INCLUDE_MAP_
#define _COLLECTIONS_HASH_INCLUDE_MAP_

#include "Framework/Basics/BasicTypes.h"
#include "../../BasicVec/Include/BasicVec.h"
#include "Framework/STL/Iter/Include/Iterator.h"
#include "Framework/STL/Mem/Alloc/Inc"
#include "Framework/STL/IO/Serialization/Inc"

namespace stl::collections {

    using namespace framework::basics;
    using namespace stl::mem::alloc;
    using namespace stl::iter;
    using namespace stl::io;

    template<Movable T, Movable U, Allocator A = DefaultAllocator>
    class HashMap: private Mover, private CanBeCopied<HashMap<T, U, A>>, private Dropper {

    };
    
} // namespace stl::collections

#endif /* _COLLECTIONS_HASH_INCLUDE_MAP_ */
