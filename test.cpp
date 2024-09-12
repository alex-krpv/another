#include <iostream>
#include <array>

struct Vtable {
    void (*func1)(void*);
};

class A {
public:
    A() {
        vtable.func1 = [](void* ptr) {
            static_cast<A*>(ptr)->PrintID_Impl();
        };
    }

    std::string PrintName() {
        return name_;
    }

    void PrintID() {
        vtable.func1(this);
    }

private:
    void PrintID_Impl() {
        std::cout << "Print A" << std::endl;
    }

    Vtable vtable;
    const static std::string name_;
};

const std::string A::name_ = "Name A";

class B {
public:
    B() {
        vtable.func1 = [](void* ptr) {
            static_cast<B*>(ptr)->PrintID_Impl();
        };
    }

    std::string PrintName() {
        return name_;
    }

    void PrintID() {
        vtable.func1(this);
    }

    operator A() {
        return *reinterpret_cast<A*>(this);
    }

private:
    void PrintID_Impl() {
        std::cout << "Print B" << std::endl;
    }

    Vtable vtable;
    const static std::string name_;
};



int main() {
    A A_obj;
    A_obj.PrintID();

    B B_obj;
    B_obj.PrintID();
    
    std::array<A, 1> objects = {B()};
    for (size_t i = 0; i < objects.size(); ++i) {
        objects[i].PrintID();
    }

    std::array<A*, 1> objects_p = {(A*)(new B())};
    for (size_t i = 0; i < objects.size(); ++i) {
        objects_p[i]->PrintID();
    }

    return 0;
}