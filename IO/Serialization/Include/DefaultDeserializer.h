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

#ifndef _IO_SERIALIZATION_INCLUDE_DEFAULTDESERIALIZER_
#define _IO_SERIALIZATION_INCLUDE_DEFAULTDESERIALIZER_

#include "Framework/Basics/Inc"
#include "Framework/Primitives/Inc"
#include "Framework/STL/Collections/Vec/Inc"
#include "Framework/STL/Collections/List/Inc"

using namespace framework::basics;
using namespace framework::primitives;
using namespace stl::collections;

namespace stl { namespace io {


    class DefaultDeserializer: public Mover, public DeserializerBase<Vec<MUInt8>> {
        Vec<MUInt8>  output;

        DefaultDeserializer() {}
        DefaultDeserializer(Vec<MUInt8>& output): output(output.copy()) { }

      public:
        DefaultDeserializer(DefaultDeserializer&& obj) {
            this->output = move_obj(obj.output);
        }

        static Fn create(Vec<MUInt8>& vec) -> DefaultDeserializer {
            return DefaultDeserializer(vec);
        }

        Fn copy() -> DefaultDeserializer {
            return DefaultDeserializer(this->output);
        }

        Fn push(Vec<MUInt8> o) -> Void {
            this->output.push(move_obj(o));
        }

        
        Fn deserialize_size() -> MSize override {
            Let vec = this->output.pop_vec(sizeof(MSize));
            return *((MSize*) vec.as_ptr());
        }


        Fn deserialize_uint128() -> MUInt128 override {
            Let vec = this->output.pop_vec(sizeof(MUInt128));
            return *((MUInt128*) vec.as_ptr());
        }


        Fn deserialize_uint64() -> MUInt64 override {
            Let vec = this->output.pop_vec(sizeof(MUInt64));
            return *((MUInt64*) vec.as_ptr());
        }


        Fn deserialize_uint32() -> MUInt32 override {
            Let vec = this->output.pop_vec(sizeof(MUInt32));
            return *((MUInt32*) vec.as_ptr());
        }


        Fn deserialize_uint16() -> MUInt16 override {
            Let vec = this->output.pop_vec(sizeof(MUInt16));
            return *((MUInt16*) vec.as_ptr());
        }


        Fn deserialize_uint8() -> MUInt8 override {
            return this->output.pop();
        }


        Fn deserialize_ssize() -> MSSize override {
            Let vec = this->output.pop_vec(sizeof(MSSize));
            return *((MSSize*) vec.as_ptr());
        }


        Fn deserialize_int128() -> MInt128 override {
            Let vec = this->output.pop_vec(sizeof(MInt128));
            return *((MInt128*) vec.as_ptr());
        }


        Fn deserialize_int64() -> MInt64 override {
            Let vec = this->output.pop_vec(sizeof(MInt64));
            return *((MInt64*) vec.as_ptr());
        }


        Fn deserialize_int32() -> MInt32 override {
            Let vec = this->output.pop_vec(sizeof(MInt32));
            return *((MInt32*) vec.as_ptr());
        }


        Fn deserialize_int16() -> MInt16 override {
            Let vec = this->output.pop_vec(sizeof(MInt16));
            return *((MInt16*) vec.as_ptr());
        }


        Fn deserialize_int8() -> MInt8 override {
            return this->output.pop();
        }


        Fn deserialize_bool() -> MBool override {
            return this->output.pop() == 1 ? true : false;
        }


        Fn deserialize_float64() -> MFloat64 override {
            Let vec = this->output.pop_vec(sizeof(MFloat64));
            return *((MFloat64*) vec.as_ptr());
        }


        Fn deserialize_float32() -> MFloat32 override {
            Let vec = this->output.pop_vec(sizeof(MFloat32));
            return *((MFloat32*) vec.as_ptr());
        }


        Fn deserialize_char() -> MChar override {
            return this->output.pop();
        }


        Fn deserialize_str(MStr str, Size size) -> Void override {
            Let vec = this->output.pop_vec(size);
            Let new_str = (MStr) vec.as_ptr();
            memcpy(str, new_str, size);
        }


        Fn deserialize_bytes(MBytes bytes, Size size) -> Bytes override {
            Let vec = this->output.pop_vec(size);
            Let new_bytes = (MBytes) vec.as_ptr();
            memcpy(bytes, new_bytes, size);
        }


        Fn deserialize_void() -> Void override {
            // this->output.push((MUInt8*) v, size);
        }


        Fn finished() -> Vec<MUInt8> override {
            return this->output.copy();
        }
    };
    
} // namespace io 
} // namespace stl 

#endif /* _IO_SERIALIZATION_INCLUDE_DEFAULTDESERIALIZER_ */
