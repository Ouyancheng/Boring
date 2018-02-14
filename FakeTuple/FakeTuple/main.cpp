//
//  main.cpp
//  FakeTuple
//
//  Created by 区彦成 on 2018-02-14.
//  Copyright © 2018 区彦成. All rights reserved.
//

#include <iostream>

template <typename T, typename ...Args>
class Tuple {
public:
    Tuple<Args...> next;
    T value;
    typedef T value_t;
    Tuple(const T &value, const Args&... args) : next(args...), value(value) {}
};
template <typename T>
class Tuple<T> {
public:
    T value;
    typedef T value_t;
    Tuple(const T &value) : value(value) {}

};
template <size_t i, typename T, typename ...Args>
class GetTupleType;
template <size_t i, typename  T, typename ...Args>
class GetTupleType<i, Tuple<T, Args...>> : GetTupleType<i-1, Tuple<Args...>> {
public:
    typedef typename GetTupleType<i-1, Tuple<Args...>>::value_t value_t;
};
template <typename T, typename ...Args>
class GetTupleType<0, Tuple<T, Args...>> {
public:
    typedef typename Tuple<T, Args...>::value_t value_t;
};

int main(int argc, const char * argv[]) {
    // insert code here...
    // std::cout << "Hello, World!\n";
    Tuple<int, double, char> t(4, 4.3, 'A');
    printf("%lf\n", t.next.value);
    printf("%c\n", t.next.next.value);
    typename GetTupleType<2, Tuple<int, double, char>>::value_t num = 'A';
    printf("%c\n", num);
    return 0;

}