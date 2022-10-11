#include "Framework/STL/String/Include/String.h"

#include "../Include/Serialize.h"
#include "../Include/Deserialize.h"
#include "Framework/STL/Sys/Fs/Inc"
#include "Framework/STL/Collections/Vec/Include/Vec.h"

using namespace stl;
using namespace stl::io;
using namespace stl::sys::fs;
using namespace stl::collections;

struct MyStruct: public Mover, public CanBeCopied<MyStruct> {
    MUInt8   a;
    MUInt16  b;
    MInt32   c;
    MUInt64  d;
    MFloat32 e;
    MBool     f;

  public:
    MyStruct() { }
    MyStruct(MyStruct&& my): a(my.a), b(my.b), c(my.c), d(my.d), e(my.e), f(my.f) { };
    MyStruct(
        MUInt8   a,
        MUInt16  b,
        MInt32   c,
        MUInt64  d,
        MFloat32 e,
        Bool     f
    ): a(a), b(b), c(c), d(d), e(e), f(f) { };

    Fn copy() -> MyStruct {

    }

    Fn serialize(BasicSerializer& ser) -> Serialized<MUInt8*> {
        ser.serialize_uint8(a);
        ser.serialize_uint16(b);
        ser.serialize_int32(c);
        ser.serialize_uint64(d);
        ser.serialize_float32(e);
        ser.serialize_bool(f);
        return ser.finished();
    }

    static Fn deserialize(BasicDeserializer& de) -> MyStruct {
        Let f = de.deserialize_bool();
        Let e = de.deserialize_float32();
        Let d = de.deserialize_uint64();
        Let c = de.deserialize_int32();
        Let b = de.deserialize_uint16();
        Let a = de.deserialize_uint8();
        return MyStruct(a, b, c, d, e, f);
    }

    Fn operator =(MyStruct&& obj) -> Void {
        this->a = obj.a;
        this->b = obj.b;
        this->c = obj.c;
        this->d = obj.d;
        this->e = obj.e;
        this->f = obj.f;
    }
};


struct MyClass: public Mover, public CanBeCopied<MyClass> {
    MyStruct my;
    String   s;

  public:
    MyClass() { }

    MyClass(MyClass&& obj) {
        this->my = move_obj(obj.my);
        this->s  = move_obj(obj.s);
    }
    
    MyClass(
        MUInt8   a,
        MUInt16  b,
        MInt32   c,
        MUInt64  d,
        MFloat32 e,
        Bool     f,
        String   s
    ): my(a, b, c, d, e, f), s(move_obj(s)) { }

    Fn copy() -> MyClass {

    }

    MyClass(MyStruct my, String s): my(move_obj(my)), s(move_obj(s)) { }

    Fn serialize(BasicSerializer& ser) -> Serialized<MUInt8*> {
        this->s.serialize(ser);
        my.serialize(ser);
        return ser.finished();
    }

    static Fn deserialize(BasicDeserializer& de) -> MyClass {
        Let my  = MyStruct::deserialize(de);
        Let str = String::deserialize(de);
        return MyClass(move_obj(my), move_obj(str));
    }

    Fn operator =(MyClass&& obj) -> Void {
        this->my = move_obj(obj.my);
        this->s  = move_obj(obj.s);
    }
};


int main() {
    // Let s = MyClass(5, 18, -3243, 1234567, 654, true, String::from("Salam man mostafa hastam!"));

    Let ser = BasicSerializer::create();
    // Let out = s.serialize(ser);

    // Let file = File::create("result.bin");

    // Let vec = Vec<MUInt8>::from_raw_ptr(out.data, out.len, out.len);
    // file.write_from_vec(vec);

    Let b = Vec<MyClass>::create();
    b.push(MyClass(5, 18, -3243, 1234567, 654.43, true, String::from("Salam man mostafa hastam!")));
    b.push(MyClass(12, 97, -43, 2314, 33.0, false, String::from("This is the second element in vector!")));
    b.push(MyClass(42, 21, 3243, 43512, -32.02, true, String::from("Good bye to the next!")));

    // Let out = b.serialize(ser);

    for (size_t i = 0; i < 10000000; i++) {
        b.serialize(ser);
    }

    // Let file = File::create("result.bin");

    // Let vec = Vec<MUInt8>::from_raw_ptr(out.data, out.len, out.len);
    // file.write_from_vec(vec);

    // Let file = File::open("result.bin");

    // Let vec = Vec<MUInt8>::create();
    // file.read_to_vec(vec);

    
    // for (size_t i = 0; i < 1000000; i++) {
    //     Let de = BasicDeserializer::from_raw_ptr((MUInt8*) vec.as_ptr(), vec.len(), vec.capacity());
    //     Let s = Vec<MyClass>::deserialize(de);
    
    //     // // printf("%s\n", &s.s);
    //     // for (size_t i = 0; i < s.len(); i++) {
    //     //     printf("%s\n", &(s[i].s));
    //     //     printf("%d, %d, %d, %ld, %f, %d\n", s[i].my.a, s[i].my.b, s[i].my.c, s[i].my.d, s[i].my.e, s[i].my.f);
    //     // }
    // }
    
    

    return 0;
}