#include "Framework/STL/Sys/Sync/Include/Arc.h"
#include "Framework/STL/Sys/Sync/Include/Mutex.h"
#include "Framework/STL/Sys/Exec/Include/Thread.h"
using namespace stl::sys::sync;
using namespace stl::sys::exec;


class Hello: public Mover, public CanBeCopied<Hello>, public Dropper {
  private:
    MInt32 data;
  public:
    Hello() { }
    Hello(Hello&& obj) {
        this->data = obj.data;
        obj.drop();
    }
    Hello(Int32 data): data(data) { }
    Fn copy() -> Hello {
        return Hello(this->data);
    }
    Fn drop() -> Void  {}

    Fn operator =(Hello&& obj) -> Void {
        this->data = obj.data;
        obj.drop();
    }
};

MUInt32 thread_callback(Arc<Mutex<Hello>> arg) {
    Let b = (*arg).lock().unwrap();

    return 5;
}


int main() {
    Let mutex = Mutex<Hello>::create(Hello(43));
    Let arc   = Arc<Mutex<Hello>>::create(moveObj(mutex));

    Let arc2 = arc.copy();

    Let thread = Thread::spawn<Arc<Mutex<Hello>>, MUInt32>(moveObj(arc), thread_callback);
    Let res = (*arc2).lock().unwrap();

    thread.join();

    return 2;
}