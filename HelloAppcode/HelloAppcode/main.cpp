//
//  main.cpp
//  HelloAppcode
//
//  Created by 区彦成 on 2018-02-10.
//  Copyright © 2018 区彦成. All rights reserved.
//

#include <cstdio>
class ExampleClass {
public:
    ExampleClass() {}
    int func(int a, int b) {
        return (a + b) * 2;
    }
};
template <typename T, typename ...Args>
inline auto callFunction(T func, Args&&... args) {
    return func(args...);
}
template <typename F, typename C>
struct BoundMemberFunc {
    BoundMemberFunc(F f, C &c) : f(f), c(c) {}
    template <typename ...Args>
    auto operator ()(Args&&... args) {
        return (c.*f)(args...);
    }
    F f;
    C &c;
};
template <typename F, typename C>
BoundMemberFunc<F, C> bindMemberFunc(F f, C &c) {
    return BoundMemberFunc<F, C>(f, c);
}
int add(int a, int b) {
    return a + b;
}
int main(int argc, const char * argv[]) {
    // insert code here...
    // std::cout << "Hello, World!\n";
    ExampleClass exampleClass;
    printf("%d\n", callFunction(add, 1, 2));
    printf("%lf\n", callFunction([](auto a, auto b) {return a * b;}, 4, 4.3));
    printf("%d\n", callFunction(bindMemberFunc(&ExampleClass::func, exampleClass), 1, 2));
    // auto example_class_func = &ExampleClass::func;
    // printf("%d\n", (exampleClass.*example_class_func)(1, 2));
    auto bmf = bindMemberFunc(&ExampleClass::func, exampleClass);
    auto bmf2 = bmf;
    printf("%d\n", &(bmf.c) == &(bmf2.c));
    return 0;

}