//
//  main.cpp
//  HelloAppcode
//
//  Created by 区彦成 on 2018-02-10.
//  Copyright © 2018 区彦成. All rights reserved.
//

#include <cstdio>
#include <memory>
#include <functional>
class ExampleClass {
public:
    ExampleClass() {}
    int func(int a, int b) {
        return (a + b) * 2;
    }
    void print() {
        printf("This is an example class\n");
    }
    template <typename T1, typename T2>
    auto universalAdd(T1 &t1, T2 &t2) {
        return t1 + t2;
    };
};
int add(int a, int b) {
    return a + b;
}
template <typename T1>
T1 nochange(T1 &a) {
    return a;
}
template <typename T, typename ...Args>
inline auto callFunction(T func, Args... args) {
    return func(std::forward<Args>(args)...);
}
template <typename T, typename C>
struct BoundMemberFunc;
template <typename T, typename C>
struct BoundMemberFunc {
    BoundMemberFunc(T func, C &c) : func(func), c(c) {}
    template <typename ...Args>
    inline auto operator ()(Args... args) {
        return (c.*func)(args...);
    }
    T func;
    C &c;
};
template <typename T, typename C>
BoundMemberFunc<T, C> bindMemberFunc(T func, C &c) {
    return BoundMemberFunc<T, C>(func, c);
}
template <typename T>
class FuncWrapper;
template <typename T, typename ...Args>
class FuncWrapper<T(Args...)> {
    typedef T(universal_func_t)(void*, Args&&...);
public:
    FuncWrapper(T(func)(Args...)) :
            func(reinterpret_cast<universal_func_t*>(universal_func<T(Args...)>)),
            func_ptr((char*)func),
            destructor(reinterpret_cast<void(*)(void*)>(do_nothing)) {}
    template <typename C>
    FuncWrapper(const C &func) :
            func(reinterpret_cast<universal_func_t*>(universal_func<C>)),
            func_ptr((char*)new C(func)),
            destructor(reinterpret_cast<void(*)(void*)>(delete_func_ptr<C>)){}
    ~FuncWrapper() {
        destructor(func_ptr);
    }
    inline auto operator ()(Args... args) {
        return func(func_ptr, std::forward<Args>(args)...);
    }
private:
    template <typename F>
    inline static T universal_func(F *function, Args&&... args) {
        return (*function)(std::forward<Args>(args)...);
    }
    template <typename C>
    static void delete_func_ptr(void *func_ptr) {
        delete ((C*)func_ptr);
    }
    static void do_nothing(void *func_ptr) {}
    universal_func_t *func;
    void *func_ptr;
    void(*destructor)(void *ptr);
};
void testFuncWrapper() {
    ExampleClass exampleClass2;
    auto fw = FuncWrapper<int(int,int)>(add);
    printf("%d\n", fw(1, 2));
    auto fw2 = FuncWrapper<int(int,int)>(bindMemberFunc(&ExampleClass::func, exampleClass2));
    printf("%d\n", fw2(1, 2));
    auto fw3 = FuncWrapper<void()>(bindMemberFunc(&ExampleClass::print, exampleClass2));
    fw3();
    auto fw4 = FuncWrapper<int(int&)>(nochange<int>);
    int a = 1024;
    printf("%d\n", fw4((a)));
    auto fw5 = FuncWrapper<int(int&,int&)>(bindMemberFunc(&ExampleClass::universalAdd<int, int>, exampleClass2));
    int b = 128, c = 128;
    printf("%d\n", fw5((b), (c)));
    auto fw6 = FuncWrapper<int(int,int)>([=](int a, int b){return a + b;});
    printf("%d\n", fw6(1, 2));
}
void testCallFunction() {
    ExampleClass exampleClass;
    auto uptrec = std::make_unique<ExampleClass>();
    printf("%d\n", callFunction(add, 1, 2));
    printf("%lf\n", callFunction([](auto a, auto b) {return a * b;}, 4, 4.3));
    printf("%d\n", callFunction(bindMemberFunc(&ExampleClass::func, exampleClass), 1, 2));
    callFunction(bindMemberFunc(&ExampleClass::print, exampleClass));
    printf("%f\n", callFunction(bindMemberFunc(&ExampleClass::universalAdd<float, float>, exampleClass), 4.0f, 4.3f));
    // auto example_class_func = &ExampleClass::func;
    // printf("%d\n", (exampleClass.*example_class_func)(1, 2));
    auto bmf = bindMemberFunc(&ExampleClass::func, exampleClass);
    auto bmf2 = bmf;
    printf("%d\n", &(bmf.c) == &(bmf2.c));
    auto uptrbmf = std::make_unique<decltype(bindMemberFunc(&ExampleClass::func, *uptrec))>(
            bindMemberFunc(&ExampleClass::func, *uptrec)
    );
    auto ptrbmf = new decltype(bindMemberFunc(&ExampleClass::func, *uptrec))(
            bindMemberFunc(&ExampleClass::func, *uptrec)
    );
    printf("%d\n", &(uptrbmf->c) == &(ptrbmf->c));
    delete ptrbmf;
    uptrbmf.reset();
    uptrec->print();
}
int main(int argc, const char * argv[]) {
    // insert code here...
    // std::cout << "Hello, World!\n";
    // testCallFunction();
    testFuncWrapper();
    std::function<int(int&,int&)> func([=](int &a, int &b) {return a + b;});
    int a = 1, b = 2;
    printf("%d\n", func(a, b));
    auto bindf = std::bind(nochange<int>, a);
    bindf();
    return 0;

}