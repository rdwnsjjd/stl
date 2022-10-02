// // Copyright (C) 2022 Emad
// // 
// // This file is part of sandikCpp.
// // 
// // sandikCpp is free software: you can redistribute it and/or modify
// // it under the terms of the GNU General Public License as published by
// // the Free Software Foundation, either version 3 of the License, or
// // (at your option) any later version.
// // 
// // sandikCpp is distributed in the hope that it will be useful,
// // but WITHOUT ANY WARRANTY; without even the implied warranty of
// // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// // GNU General Public License for more details.
// // 
// // You should have received a copy of the GNU General Public License
// // along with sandikCpp.  If not, see <http://www.gnu.org/licenses/>.

// #include "../Include/Serialize.h"

// namespace stl { namespace io {

//     Fn BasicSerializer::_reserve_capacity(Size size) -> Size {
//         if (this->_cap == 0 || this->_cap == INVALID_ID) {
//             this->_output = (MUInt8*) this->_allocator.allocate(size, sizeof(MUInt8));
//             // soft_assert_ret_res_(is_valid_handle(this->_output), "Failed to allocate proper memory of size %ld!", size);
//         }
//         else {
//             this->_output = (MUInt8*) this->_allocator.reallocate(this->_output, size * sizeof(MUInt8));
//             // soft_assert_ret_res_(is_valid_handle(this->_output), "Failed to reallocate proper memory of size %ld!", size);
//         }

//         this->_cap += size;
//         return size;
//     }


//     Fn BasicSerializer::_append(MUInt8 data) -> Void {
//         if (this->_len >= this->_cap) {
//             this->_reserve_capacity(create_proper_alloc_size_(1) + this->_cap);
//         }
//         this->_output[this->_len++] = data;
//     }


//     Fn BasicSerializer::_append(MUInt8* byt, Size size) -> Void {
//         if (this->_len + size >= this->_cap) {
//             this->_reserve_capacity(create_proper_alloc_size_(size) + this->_cap);
//         }

//         // for (MSSize i = size - 1; i >= 0; i--) {
//         for (MIdx i = 0; i < size; i++) {
//             // TODO: change this to copy data
//             this->_output[this->_len++] = *(byt + i);
//         }
//     }


//     BasicSerializer::BasicSerializer(BasicSerializer&& obj) {
//         this->_output = obj._output;
//         this->_cap    = obj._cap;
//         this->_len    = obj._len;
//     }

//     Fn BasicSerializer::create() -> BasicSerializer {
//         return BasicSerializer();
//     }

//     Fn BasicSerializer::copy() -> BasicSerializer {
//         return BasicSerializer(this->_output);
//     }

//     Fn BasicSerializer::push(MUInt8* o) -> Void {
//         this->_append(*o);
//     }

    
//     Fn BasicSerializer::serialize_size(MSize v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(Size));
//     }


//     Fn BasicSerializer::serialize_uint128(MUInt128 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MUInt128));
//     }


//     Fn BasicSerializer::serialize_uint64(MUInt64 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MUInt64));
//     }


//     Fn BasicSerializer::serialize_uint32(MUInt32 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MUInt32));
//     }


//     Fn BasicSerializer::serialize_uint16(MUInt16 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MUInt16));
//     }


//     Fn BasicSerializer::serialize_uint8(MUInt8 v) -> Void {
//         this->_append(moveObj(v));
//     }


//     Fn BasicSerializer::serialize_ssize(MSSize v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MSSize));
//     }


//     Fn BasicSerializer::serialize_int128(MInt128 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MInt128));
//     }


//     Fn BasicSerializer::serialize_int64(MInt64 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MInt64));
//     }


//     Fn BasicSerializer::serialize_int32(MInt32 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MInt32));
//     }


//     Fn BasicSerializer::serialize_int16(MInt16 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MInt16));
//     }


//     Fn BasicSerializer::serialize_int8(MInt8 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MInt8));
//     }


//     Fn BasicSerializer::serialize_bool(MBool v) -> Void {
//         this->_append(v ? 1 : 0);
//     }


//     Fn BasicSerializer::serialize_float64(MFloat64 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MFloat64));
//     }


//     Fn BasicSerializer::serialize_float32(MFloat32 v) -> Void {
//         this->_append((MUInt8*) &v, sizeof(MFloat32));
//     }


//     Fn BasicSerializer::serialize_char(MChar v) -> Void {
//         this->_append(moveObj(v));
//     }


//     Fn BasicSerializer::BasicSerializer::serialize_str(Str v) -> Void {
//         this->_append((MUInt8*) v, str_len(v));
//     }


//     Fn BasicSerializer::serialize_bytes(Bytes v, Size size) -> Void {
//         this->_append((MUInt8*) v, size);
//     }


//     Fn BasicSerializer::serialize_void() -> Void {
//         // this->_append((MUInt8*) v, size);
//     }


//     // template<Serializable<BasicSerializer> S>
//     // Fn serialize_struct(S obj) -> Void {
//     //     obj.serialize(this);
//     // }


//     Fn BasicSerializer::finished() -> MUInt8* {
//         return this->_output;
//     }

// } // namespace io 
// } // namespace stl 