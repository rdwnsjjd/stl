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

#ifndef _IO_SERIALIZATION_INCLUDE_DESERIALIZE_
#define _IO_SERIALIZATION_INCLUDE_DESERIALIZE_

#include <string.h>

#include "Framework/Basics/BasicTypes.h"
#include "Framework/Basics/BasicConcepts.h"
#include "Framework/Basics/BasicOperations.h"
#include "Framework/Basics/OwnerShip.h"
#include "Framework/STL/Mem/Alloc/Include/Default.h"

namespace stl { namespace io {

    using namespace framework;
    using namespace framework::concepts;
    using namespace framework::basics;
    using namespace stl::mem::alloc;

    template<typename Output>
    class DeserializerBase;

    class BasicDeserializer;

    template<typename T>
    concept Deserializer = concepts::IsDerived<DeserializerBase, T>;


    template<typename Output>
    class DeserializerBase {
      public:
        // virtual Fn push(Output o) -> Void = 0;

        virtual Fn deserialize_size() -> MSize = 0;

        virtual Fn deserialize_uint128() -> MUInt128 = 0;

        virtual Fn deserialize_uint64() -> MUInt64 = 0;

        virtual Fn deserialize_uint32() -> MUInt32 = 0;

        virtual Fn deserialize_uint16() -> MUInt16 = 0;

        virtual Fn deserialize_uint8() -> MUInt8 = 0;

        virtual Fn deserialize_ssize() -> MSSize = 0;

        virtual Fn deserialize_int128() -> MInt128 = 0;

        virtual Fn deserialize_int64() -> MInt64 = 0;

        virtual Fn deserialize_int32() -> MInt32 = 0;

        virtual Fn deserialize_int16() -> MInt16 = 0;

        virtual Fn deserialize_int8() -> MInt8 = 0;

        virtual Fn deserialize_bool() -> MBool = 0;

        virtual Fn deserialize_float64() -> MFloat64 = 0;

        virtual Fn deserialize_float32() -> MFloat32 = 0;

        virtual Fn deserialize_char() -> MChar = 0;

        virtual Fn deserialize_str(MStr str, Size size) -> Void = 0;

        virtual Fn deserialize_bytes(MBytes bytes, Size size) -> Void = 0;

        virtual Fn deserialize_void() -> Void = 0;

        virtual Fn finished() -> Output = 0;
    };


    class BasicDeserializer: public Mover, public DeserializerBase<MUInt8*> {
        MUInt8*          _output;
        MSize            _len;
        MSize            _cap;
        DefaultAllocator _allocator;

        BasicDeserializer() {}
        BasicDeserializer(MUInt8* ptr, Size len, Size cap) {
            this->_output = (MUInt8*) this->_allocator.allocate(cap, 1).unwrap();
            memcpy(this->_output, ptr, len);

            this->_len = len;
            this->_cap = cap;
        }


        Fn _reserve_capacity(Size size) -> Size {
            if (this->_cap == 0 || this->_cap == INVALID_ID) {
                this->_output = (MUInt8*) this->_allocator.allocate(size, sizeof(MUInt8)).unwrap();
                // soft_assert_ret_res_(is_valid_handle(this->_output), "Failed to allocate proper memory of size %ld!", size);
            }
            else {
                this->_output = (MUInt8*) this->_allocator.reallocate(this->_output, size * sizeof(MUInt8)).unwrap();
                // soft_assert_ret_res_(is_valid_handle(this->_output), "Failed to reallocate proper memory of size %ld!", size);
            }

            this->_cap = size;
            return size;
        }


        Fn _pop() -> MUInt8 {
            return this->_output[--this->_len];
        }


        Fn _pop(Size len) -> MUInt8* {
            Let ptr = (MUInt8*) this->_allocator.allocate(len, 1).unwrap();
            for (MSize idx = 0; idx < len; idx++) {
                *(ptr + idx) = *(this->_output + (this->_len - len + idx));
            }
            
            this->_len -= len;
            return ptr;
        }


        template<Primitive T>
        Fn _pop(T* val) -> Void {
            Let len = sizeof(T);
            for (MSize idx = 0; idx < len; idx++) {
                *((MChar*) val + idx) = *(this->_output + (this->_len - len + idx));
            }
            
            this->_len -= len;
        }

      public:
        BasicDeserializer(BasicDeserializer&& obj) {
            this->_output = obj._output;
            this->_cap    = obj._cap;
            this->_len    = obj._len;
        }

        static Fn from_raw_ptr(MUInt8* vec, Size len, Size cap) -> BasicDeserializer {
            return BasicDeserializer(vec, len, cap);
        }

        Fn copy() -> BasicDeserializer {
            return BasicDeserializer(this->_output, this->_len, this->_cap);
        }


        template<Deserializer DeT, typename T>
        Fn deserialize() -> T {
            CONST_IF (IsPrimitive<T>) {
                T val = (T) 0;
                this->_pop<T>(&val);
                return val;
            }
            CONST_ELSE {
                return T::deserialize(*this);
            }
        }

        
        Fn deserialize_size() -> MSize override {
            MSize val = 0;
            this->_pop<MSize>(&val);
            return val;
        }


        Fn deserialize_uint128() -> MUInt128 override {
            MUInt128 val = 0;
            this->_pop<MUInt128>(&val);
            return val;
        }


        Fn deserialize_uint64() -> MUInt64 override {
            MUInt64 val = 0;
            this->_pop<MUInt64>(&val);
            return val;
        }


        Fn deserialize_uint32() -> MUInt32 override {
            MUInt32 val = 0;
            this->_pop<MUInt32>(&val);
            return val;
        }


        Fn deserialize_uint16() -> MUInt16 override {
            MUInt16 val = 0;
            this->_pop<MUInt16>(&val);
            return val;
        }


        Fn deserialize_uint8() -> MUInt8 override {
            return this->_pop();
        }


        Fn deserialize_ssize() -> MSSize override {
            MSSize val = 0;
            this->_pop<MSSize>(&val);
            return val;
        }


        Fn deserialize_int128() -> MInt128 override {
            MInt128 val = 0;
            this->_pop<MInt128>(&val);
            return val;
        }


        Fn deserialize_int64() -> MInt64 override {
            MInt64 val = 0;
            this->_pop<MInt64>(&val);
            return val;
        }


        Fn deserialize_int32() -> MInt32 override {
            MInt32 val = 0;
            this->_pop<MInt32>(&val);
            return val;
        }


        Fn deserialize_int16() -> MInt16 override {
            MInt16 val = 0;
            this->_pop<MInt16>(&val);
            return val;
        }


        Fn deserialize_int8() -> MInt8 override {
            MInt8 val = 0;
            this->_pop<MInt8>(&val);
            return val;
        }


        Fn deserialize_bool() -> MBool override {
            return this->_pop() == 1 ? true : false;
        }


        Fn deserialize_float64() -> MFloat64 override {
            MFloat64 val = 0;
            this->_pop<MFloat64>(&val);
            return val;
        }


        Fn deserialize_float32() -> MFloat32 override {
            MFloat32 val = 0;
            this->_pop<MFloat32>(&val);
            return val;
        }


        Fn deserialize_char() -> MChar override {
            return this->_pop();
        }


        Fn deserialize_str(MStr str, Size size) -> Void override {
            Let ptr = this->_pop(size);
            memcpy(str, ptr, size);
            this->_allocator.deallocate(ptr);
        }


        Fn deserialize_bytes(MBytes bytes, Size size) -> Void override {
            Let ptr = this->_pop(size);
            memcpy(bytes, ptr, size);
            this->_allocator.deallocate(ptr);
        }


        Fn deserialize_void() -> Void override {
            // this->output.push((MUInt8*) v, size);
        }


        Fn finished() -> MUInt8* override {
            return this->_output;
        }

        
        ~BasicDeserializer() {
            this->_allocator.deallocate(this->_output);
        }
    };


    // template<typename S, typename T /*, typename O */>
    // concept Deserializable = Deserializer<S> && requires(T t, S& s) {
    //     { t.deserialize(s) } -> std::convertible_to<Vec<MUInt8> /* O */ >;
    // };
    
} // namespace io 
} // namespace stl 
    
#endif /* _IO_SERIALIZATION_INCLUDE_DESERIALIZE_ */
