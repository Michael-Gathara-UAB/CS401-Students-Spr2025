#include <memory>
#include <iostream>

struct Resource {
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource released\n"; }
};

int X() {
    std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>();
    {
        std::shared_ptr<Resource> ptr2 = ptr1;  // Reference count increases
    }  // ptr2 goes out of scope; reference count decreases
    // Resource is released when ptr1 goes out of scope and the count drops to zero
}


int Y() {
    std::unique_ptr<Resource> ptr = std::make_unique<Resource>();
    // Resource is automatically released when ptr goes out of scope
}




