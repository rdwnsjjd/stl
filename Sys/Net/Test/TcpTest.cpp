#include "Framework/STL/Sys/Ipc/Inc"
#include "Framework/STL/String/Inc"

using namespace stl::sys::ipc;
using namespace stl;

Fn main() -> MInt32 {

    Let stream = LocalSocket::create();
    stream.bind();
    
    // Loop {
        Let peer = stream.listen();

        peer.unwrap().send();
    // }
    
    // stream.unwrap().send();
    return -1;
}