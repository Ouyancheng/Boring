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
        virtual void * get() = 0;
        virtual ~virtual_holder() {}
        virtual virtual_holder *clone() = 0;
    };
    template <typename T>
    struct holder : public virtual_holder {
        virtual void * get() {
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

    Any &operator =(const Any &another) {
        delete vh;
        vh = another.vh->clone();
        return *this;
    }

    template <typename T>
    Any &operator =(const T &value) {
        delete vh;
        vh = new holder<T>(value);
        return *this;
    }
    template <typename T>
    Any &operator =(T value[]) {
        delete vh;
        vh = new holder<T*const>(value);
        return *this;
    }
    template <typename T>
    Any &operator =(std::initializer_list<T> init) {
        delete vh;
        vh = new holder<std::vector<T>>(std::move(std::vector<T>(init)));
        return *this;
    }

    ~Any() {
        delete vh;
    }
    template <typename T>
    static T &get(const Any &any) {
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

    a0 = 4;
    a1 = a6 = "best";
    a2 = {1, 2, 3, 5, 8};
    a3 = a4 = a5 = "excellent";
    std::cout << Any::get<int>(a0) << std::endl;
    std::cout << Any::get<char*>(a1) << " " << Any::get<char*>(a6) << std::endl;
    std::cout << Any::get<std::vector<int>>(a2)[4] << std::endl;
    std::cout << Any::get<char*>(a3) << " " << Any::get<char*>(a4) << " " << Any::get<char*>(a5) << std::endl;
    
    Any::get<int>(a0) = 1024;
    std::cout << Any::get<int>(a0) << std::endl;
    Any::get<const char*>(a4) = "nice";
    std::cout << Any::get<char*>(a4) << std::endl;
    return 0;

}
