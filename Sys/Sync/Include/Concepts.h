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

#ifndef _SYS_SYNC_INCLUDE_CONCEPTS_
#define _SYS_SYNC_INCLUDE_CONCEPTS_

#include <pthread.h>
#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/OwnerShip.h"
#include "Framework/Primitives/Result.h"


namespace stl { namespace sys { namespace sync {

    using namespace framework;
    using namespace framework::basics;

    class LockerBase: public Mover {
        protected:
            virtual Fn lock() -> EResult<__> = 0;
            virtual Fn unlock() -> EResult<__> = 0;
            // virtual ~LockerBase() = 0;
    };

    template <typename T>
    concept Lockable = std::is_base_of_v<LockerBase, T>;
    
    
} // namespace sync 
} // namespace sys 
} // namespace stl 


#endif /* _SYS_SYNC_INCLUDE_CONCEPTS_ */
