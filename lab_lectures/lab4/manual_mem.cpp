#include <iostream>
#include <string>
#include <vector>

#define OBJECT_COUNT 5
#define DATA_SIZE 1000000

class MemoryHungryObject {
public:
    std::string name;
    int *data;

    MemoryHungryObject(const std::string &name, int value) : name(name) {
        data = new int[DATA_SIZE];
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i] = value;
        }
    }

    ~MemoryHungryObject() {
        delete[] data;
    }
};

void create_objects(std::vector<MemoryHungryObject*> &objects) {
    for (int i = 0; i < OBJECT_COUNT; i++) {
        std::string name = "object_" + std::to_string(i);
        objects.push_back(new MemoryHungryObject(name, i));
        std::cout << "Created " << objects.back()->name << std::endl;
    }
}

void print_memory_usage(const std::vector<MemoryHungryObject*> &objects) {
    std::cout << "\nObjects memory usage\n";
    for (const auto &obj : objects) {
        if (obj != nullptr) {
            std::cout << obj->name << ": " << sizeof(int) * DATA_SIZE << " Bytes" << std::endl;
        }
    }
}

int main() {
    std::vector<MemoryHungryObject*> objects;

    std::cout << "Creating objects...\n";
    create_objects(objects);
    print_memory_usage(objects);

    std::cout << "\nBefore manual cleanup:\n";
    std::cout << "Current objects: ";
    for (const auto &obj : objects) {
        if (obj != nullptr) {
            std::cout << obj->name << " ";
        }
    }
    std::cout << "\n";

    std::cout << "\nPerforming manual cleanup...\n";
    delete objects[1];
    objects[1] = nullptr;
    delete objects[3];
    objects[3] = nullptr;

    std::cout << "\nRemaining objects memory usage:\n";
    for (const auto &obj : objects) {
        if (obj != nullptr) {
            std::cout << obj->name << ": " << sizeof(int) * DATA_SIZE << " Bytes" << std::endl;
        }
    }

    std::cout << "\nAfter manual cleanup:\n";
    std::cout << "Remaining objects: ";
    for (const auto &obj : objects) {
        if (obj != nullptr) {
            std::cout << obj->name << " ";
        }
    }
    std::cout << "\n";

    for (auto &obj : objects) {
        if (obj != nullptr) {
            delete obj;
            obj = nullptr;
        }
    }

    std::cout << "\nAfter manual cleanup two:\n";
    std::cout << "Remaining objects: None\n";

    return 0;
}