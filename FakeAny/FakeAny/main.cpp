//
//  main.cpp
//  FakeAny
//
//  Created by 区彦成 on 2018-02-11.
//  Copyright © 2018 区彦成. All rights reserved.
//

#include <iostream>
#include <vector>
struct Any {
    struct virtual_holder {
        virtual void * const get() = 0;
        virtual ~virtual_holder() {}
        virtual virtual_holder *clone() = 0;
    };
    template <typename T>
    struct holder : public virtual_holder {
        virtual void * const get() {
            return (void*)&value;
        }
        holder(const T &value) : value(value) {}
        virtual ~holder() {}
        virtual virtual_holder *clone() {
            return new holder<T>(value);
        }
        T value;
    };
    virtual_holder *vh;
    template <typename T>
    Any(const T &value) : vh(new holder<T>(value)) {}
    template <typename T>
    Any(T value[]) : vh(new holder<T*const>(value)) {}
    template <typename T>
    Any(std::initializer_list<T> init) : vh(new holder<std::vector<T>>(std::move(std::vector<T>(init)))) {}

    Any(const Any &another) {
        delete vh;
        vh = another.vh->clone();
    }
    Any(Any &&another) {
        this->vh = another.vh;
        another.vh = nullptr;
    }

    ~Any() {
        delete vh;
    }
    template <typename T>
    static const T &get(const Any &any) {
        return *(T*)(any.vh->get());
    }
};
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    Any a0 = 4.0f;
    Any a1 = 4.3;
    Any a2 = 1;
    Any a3 = "geek";
    Any a4 = {1, 2, 3, 4, 5, 6};
    Any a5 = &a0;
    Any a6 = a4;
    std::cout << Any::get<float>(a0) << std::endl;
    std::cout << Any::get<double>(a1) << std::endl;
    std::cout << Any::get<int>(a2) << std::endl;
    std::cout << Any::get<char*>(a3) << std::endl;
    std::cout << Any::get<std::vector<int>>(a4)[0] << std::endl;
    std::cout << Any::get<float>(*Any::get<Any*>(a5)) << std::endl;
    std::cout << Any::get<std::vector<int>>(a6)[1] << std::endl;
    return 0;

}