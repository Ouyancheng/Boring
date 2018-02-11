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
template <typename T, typename ...Args>
inline auto callFunction(T func, Args&&... args) {
    return func(args...);
}
template <typename T>
struct BoundMemberFunc {
    BoundMemberFunc() = delete;
};
template <typename T, typename C, typename ...Args>
struct BoundMemberFunc<T(C::*)(Args...)> {
    BoundMemberFunc(T(C::*func)(Args...), C &c) : func(func), c(c) {}
    // BoundMemberFunc(const BoundMemberFunc<T(C::*)(Args...)> &bmf) : func(bmf.func), c(bmf.c) {}
    template <typename ...FArgs>
    auto operator ()(FArgs&&... args) {
        return (c.*func)(args...);
    }
    T(C::*func)(Args...);
    C &c;
};
template <typename T, typename C, typename ...Args>
BoundMemberFunc<T(C::*)(Args...)> bindMemberFunc(T(C::*func)(Args...), C &c) {
    return BoundMemberFunc<T(C::*)(Args...)>(func, c);
}
int add(int a, int b) {
    return a + b;
}
template <typename T1>
T1 nochange(T1 &a) {
    return a;
}
template <typename T>
class FuncWrapper {
public:
    FuncWrapper(T func) : func(func) {}
    template <typename ...Args>
    auto operator ()(Args&&... args) {
        return func(args...);
    }

private:
    T func;
};
template <typename T, typename ...Args>
class FuncWrapper<T(Args...)> {
public:
    FuncWrapper(T (fn)(Args...)) : func(fn) {}
    auto operator ()(Args&&... args) {
        return func(args...);
    }
private:
    T (*func)(Args...);
};
template <typename C, typename T, typename ...Args>
class FuncWrapper<BoundMemberFunc<T(C::*)(Args...)>> {
public:
    FuncWrapper(BoundMemberFunc<T(C::*)(Args...)> bmf) : func(bmf) {}
    auto operator ()(Args&&... args) {
        return func(args...);
    }
private:
    BoundMemberFunc<T(C::*)(Args...)> func;
};
template <typename T, typename ...Args>
FuncWrapper<T(Args...)> makeFuncWrapper(T (func)(Args...)) {
    return FuncWrapper<T(Args...)>(func);
}
template <typename T, typename C, typename ...Args>
FuncWrapper<BoundMemberFunc<T(C::*)(Args...)>> makeFuncWrapper(BoundMemberFunc<T(C::*)(Args...)> bmf) {
    return FuncWrapper<BoundMemberFunc<T(C::*)(Args...)>>(bmf);
}
template <typename T>
FuncWrapper<T> makeFuncWrapper(T func) {
    return FuncWrapper<T>(func);
}
void testFuncWrapper() {
    ExampleClass exampleClass2;
    auto fw = makeFuncWrapper(add);
    printf("%d\n", fw(1, 2));
    auto fw2 = makeFuncWrapper(bindMemberFunc(&ExampleClass::func, exampleClass2));
    printf("%d\n", fw2(1, 2));
    auto fw3 = makeFuncWrapper(bindMemberFunc(&ExampleClass::print, exampleClass2));
    fw3();
    auto fw4 = makeFuncWrapper(nochange<int>);
    int a = 1024;
    printf("%d\n", fw4(a));
    auto fw5 = makeFuncWrapper(bindMemberFunc(&ExampleClass::universalAdd<int, int>, exampleClass2));
    int b = 128, c = 128;
    printf("%d\n", fw5(b, c));
    auto fw6 = makeFuncWrapper([=](int a, int b){return a + b;});
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
    testCallFunction();
    testFuncWrapper();
    std::function<int(int,int)> func([=](int a, int b) {return a + b;});
    printf("%d\n", func(1, 2));
    return 0;

}