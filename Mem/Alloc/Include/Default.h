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

#ifndef _MEM_ALLOC_INCLUDE_DEFAULT_
#define _MEM_ALLOC_INCLUDE_DEFAULT_

#include <stdlib.h>
#include <type_traits>
#include "Framework/Basics/BasicTypes.h"
#include "Framework/Primitives/Result.h"

namespace stl { namespace mem { namespace alloc {

    using namespace std;
    using namespace framework;

    class AbstractAllocator {
        protected:
            virtual Fn allocate(Size n, Size size) -> EResult<MRawPtr> = 0;

            virtual Fn deallocate(RawPtr ptr) -> Void = 0;


            virtual Fn callocate(Size n, Size size) -> EResult<MRawPtr> {
                Let res = this->allocate(n, size);
                unwrapResOrRetSame(MRawPtr, res, "Failed to allocate memory while clear allocation!");

                memset((Void*) res.unwrap(), 0, size * n);
                return resOk(MRawPtr, res.unwrap());
            };


            virtual Fn reallocate(RawPtr ptr, Size size) -> EResult<MRawPtr> {
                Let res = this->allocate(1, size);
                unwrapResOrRetSame(MRawPtr, res, "Failed to allocate memory while reallocation!");

                memcpy(res.unwrap(), ptr, size);
                this->deallocate(ptr);
                return res;
            };
    };

    
    class DefaultAllocator: public AbstractAllocator {
      private:
        Domain DOMAIN = Domain::Mem;

        public:
            Fn allocate(Size n, Size size) -> EResult<MRawPtr>;

            Fn callocate(Size n, Size size) -> EResult<MRawPtr>;

            Fn reallocate(RawPtr ptr, Size size) -> EResult<MRawPtr>;

            Fn deallocate(RawPtr ptr) -> Void;
    };


    /* Self Implementation for DefaultAllocator */

    Fn DefaultAllocator::allocate(Size n, Size size) -> EResult<MRawPtr> {
        Let ptr = (MRawPtr) malloc(n * size);
        evalOrRetFail(MRawPtr, isValidHandle(ptr), "Failed to allocate `%ld` bytes of memory!", n * size);
        return resOk(MRawPtr, ptr);
    }


    Fn DefaultAllocator::callocate(Size n, Size size) -> EResult<MRawPtr> {
        Let ptr = (MRawPtr) calloc(n, size);
        evalOrRetFail(MRawPtr, isValidHandle(ptr), "Failed to callocate `%ld` bytes of memory!", n * size);
        return resOk(MRawPtr, ptr);
    }


    Fn DefaultAllocator::reallocate(RawPtr ptr, Size size) -> EResult<MRawPtr> {
        Let new_ptr = (MRawPtr) realloc((Void*) ptr, size);
        evalOrRetFail(MRawPtr, isValidHandle(new_ptr), "Failed to reallocate `%ld` bytes of memory!", size);
        return resOk(MRawPtr, new_ptr);
    }


    Fn DefaultAllocator::deallocate(RawPtr ptr) -> Void {
        return free((Void*) ptr);
    }

    
    class CppAllocator: public AbstractAllocator {
        Domain DOMAIN = Domain::Mem;
      public:
            Fn allocate(Size n, Size size) -> EResult<MRawPtr>;

            Fn callocate(Size n, Size size) -> EResult<MRawPtr>;

            Fn reallocate(RawPtr ptr, Size size) -> EResult<MRawPtr>;

            Fn deallocate(RawPtr ptr) -> Void;
    };


    /* Self Implementation for CppAllocator */

    Fn CppAllocator::allocate(Size n, Size size) -> EResult<MRawPtr> {
        // return (MRawPtr) new char[n * size];
        return resFail(MRawPtr, EK::NotImplemented);
    }


    Fn CppAllocator::callocate(Size n, Size size) -> EResult<MRawPtr> {
        // return (MRawPtr) calloc(n, size);
        return resFail(MRawPtr, EK::NotImplemented);
    }


    Fn CppAllocator::reallocate(RawPtr ptr, Size size) -> EResult<MRawPtr> {
        // return (MRawPtr) realloc((Void*) ptr, size);
        return resFail(MRawPtr, EK::NotImplemented);
    }


    Fn CppAllocator::deallocate(RawPtr ptr) -> Void {
        // return free((Void*) ptr);
    }


    template <typename T>
    concept Allocator = std::is_base_of_v<AbstractAllocator, T>
    && requires(T obj, RawPtr ptr, Size n, Size size) {
        obj.allocate(n, size);
        obj.callocate(n, size);
        obj.reallocate(ptr, size);
        obj.deallocate(ptr);
    };

} // namespace alloc
} // namespace mem
} // namespace stl



#endif /* _MEM_ALLOC_INCLUDE_DEFAULT_ */
