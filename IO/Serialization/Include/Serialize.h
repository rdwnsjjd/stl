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

#ifndef _IO_SERIALIZATION_INCLUDE_SERIALIZE_
#define _IO_SERIALIZATION_INCLUDE_SERIALIZE_

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

    #define create_proper_alloc_size_(size_)  32 * (size_ / 32 + (size_ % 32 == 0 ? 0 : 1))

    template<typename Output>
    class SerializerBase;

    class BasicSerializer;

    template<typename T>
    concept Serializer = concepts::IsDerived<SerializerBase, T>;


    template<typename OutputT>
    struct Serialized {
        OutputT     data;
        Size        len;
    };

    template<typename S, typename T>
    concept Serializable = Primitive<T> || requires(T t, S& s) {
        { t.serialize(s) } -> std::convertible_to<MUInt8*>;
    };

    template<typename Output>
    class SerializerBase {
      public:
        virtual Fn push(Output o) -> Void = 0;

        virtual Fn serialize_size(MSize v) -> Void = 0;

        virtual Fn serialize_uint128(MUInt128 v) -> Void = 0;

        virtual Fn serialize_uint64(MUInt64 v) -> Void = 0;

        virtual Fn serialize_uint32(MUInt32 v) -> Void = 0;

        virtual Fn serialize_uint16(MUInt16 v) -> Void = 0;

        virtual Fn serialize_uint8(MUInt8 v) -> Void = 0;

        virtual Fn serialize_ssize(MSSize v) -> Void = 0;

        virtual Fn serialize_int128(MInt128 v) -> Void = 0;

        virtual Fn serialize_int64(MInt64 v) -> Void = 0;

        virtual Fn serialize_int32(MInt32 v) -> Void = 0;

        virtual Fn serialize_int16(MInt16 v) -> Void = 0;

        virtual Fn serialize_int8(MInt8 v) -> Void = 0;

        virtual Fn serialize_bool(MBool v) -> Void = 0;

        virtual Fn serialize_float64(MFloat64 v) -> Void = 0;

        virtual Fn serialize_float32(MFloat32) -> Void = 0;

        virtual Fn serialize_char(MChar v) -> Void = 0;

        virtual Fn serialize_str(Str v) -> Void = 0;

        virtual Fn serialize_bytes(Bytes v, Size size) -> Void = 0;

        virtual Fn serialize_void() -> Void = 0;

        virtual Fn finished() -> Serialized<Output> = 0;
    };


    class BasicSerializer: public Mover, public SerializerBase<MUInt8*> {
        MUInt8*          _output;
        MSize            _len = 0;
        MSize            _cap = 0;
        DefaultAllocator _allocator;

        BasicSerializer() {}
        BasicSerializer(MUInt8* output): _output(output) { }


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


        Fn _append(MUInt8 data) -> Void {
            if (this->_len >= this->_cap) {
                this->_reserve_capacity(create_proper_alloc_size_(1) + this->_cap);
            }
            this->_output[this->_len++] = data;
        }


        Fn _append(MUInt8* byt, Size size) -> Void {
            if (this->_len + size >= this->_cap) {
                this->_reserve_capacity(create_proper_alloc_size_(size) + this->_cap);
            }

            // for (MSSize i = size - 1; i >= 0; i--) {
            for (MIdx i = 0; i < size; i++) {
                // TODO: change this to copy data
                this->_output[this->_len++] = *(byt + i);
            }
        }


      public:
        BasicSerializer(BasicSerializer&& obj) {
            this->_output = obj._output;
            this->_cap    = obj._cap;
            this->_len    = obj._len;
        }

        static Fn create() -> BasicSerializer {
            return BasicSerializer();
        }

        Fn copy() -> BasicSerializer {
            return BasicSerializer(this->_output);
        }

        Fn push(MUInt8* o) -> Void {
            this->_append(*o);
        }

        // template<Serializer SerT, Serializable<SerT> T>
        template<Serializer SerT, typename T>
        Fn serialize_elem(T& v) -> Void {
            CONST_IF (IsPrimitive<T>) {
                this->_append((MUInt8*) &v, sizeof(T));
            }
            CONST_ELSE {
                v.serialize(*this);
            }
        }

        
        Fn serialize_size(MSize v) -> Void {
            this->_append((MUInt8*) &v, sizeof(Size));
        }


        Fn serialize_uint128(MUInt128 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MUInt128));
        }


        Fn serialize_uint64(MUInt64 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MUInt64));
        }


        Fn serialize_uint32(MUInt32 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MUInt32));
        }


        Fn serialize_uint16(MUInt16 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MUInt16));
        }


        Fn serialize_uint8(MUInt8 v) -> Void {
            this->_append(v);
        }


        Fn serialize_ssize(MSSize v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MSSize));
        }


        Fn serialize_int128(MInt128 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MInt128));
        }


        Fn serialize_int64(MInt64 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MInt64));
        }


        Fn serialize_int32(MInt32 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MInt32));
        }


        Fn serialize_int16(MInt16 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MInt16));
        }


        Fn serialize_int8(MInt8 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MInt8));
        }


        Fn serialize_bool(MBool v) -> Void {
            this->_append(v ? 1 : 0);
        }


        Fn serialize_float64(MFloat64 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MFloat64));
        }


        Fn serialize_float32(MFloat32 v) -> Void {
            this->_append((MUInt8*) &v, sizeof(MFloat32));
        }


        Fn serialize_char(MChar v) -> Void {
            this->_append(move_obj(v));
        }


        Fn serialize_str(Str v) -> Void {
            this->_append((MUInt8*) v, str_len(v));
        }


        Fn serialize_bytes(Bytes v, Size size) -> Void {
            this->_append((MUInt8*) v, size);
        }


        Fn serialize_void() -> Void {
            // this->_append((MUInt8*) v, size);
        }


        // template<Serializable<BasicSerializer> S>
        // Fn serialize_struct(S obj) -> Void {
        //     obj.serialize(this);
        // }


        Fn finished() -> Serialized<MUInt8*> {
            return (Serialized<MUInt8*>) {
                .data = this->_output,
                .len  = this->_len
            };
        }

        
        ~BasicSerializer() {
            this->_allocator.deallocate(this->_output);
        }
    };
    
} // namespace io 
} // namespace stl 


#endif /* _IO_SERIALIZATION_INCLUDE_SERIALIZE_ */
