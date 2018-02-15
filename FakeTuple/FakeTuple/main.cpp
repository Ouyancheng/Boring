//
//  main.cpp
//  FakeTuple
//
//  Created by 区彦成 on 2018-02-14.
//  Copyright © 2018 区彦成. All rights reserved.
//

#include <iostream>
template <size_t i, typename T, typename ...Args>
class GetTupleType;
template <typename T, typename ...Args>
class Tuple {
public:
    Tuple<Args...> next;
    T value;
    typedef T value_t;
    Tuple(const T &value, const Args&... args) : next(args...), value(value) {}
    const Tuple<Args...> & getNext() const {
        return next;
    }
    Tuple(Tuple<T, Args...> &&t) : next(std::move(t.next)), value(std::move(t.value)) {}
    Tuple(const Tuple<T, Args...> &t) : next(t.next), value(t.value) {}
    Tuple<T, Args...> &operator = (const Tuple<T, Args...> &t) {
        next = (t.next);
        value = t.value;
        return (*this);
    }
    Tuple<T, Args...> &operator = (Tuple<T, Args...> &&t) {
        next = std::move(t.next);
        value = std::move(t.value);
        return (*this);
    }
};
template <typename T>
class Tuple<T> {
public:
    T value;
    typedef T value_t;
    Tuple(const T &value) : value(value) {}
    Tuple(const Tuple<T> &t) : value(t.value) {}
    Tuple(Tuple<T> &&t) : value(std::move(t.value)) {}
    Tuple<T> &operator = (const Tuple<T> &t) {
        value = t.value;
        return (*this);
    }
    Tuple<T> &operator = (Tuple<T> &&t) {
        value = std::move(t.value);
        return (*this);
    }
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

template <size_t i, typename T, typename ...Args>
class GetTupleValue;
template <size_t i, typename T, typename ...Args>
class GetTupleValue<i, Tuple<T, Args...>> {
public:
    const typename GetTupleType<i, Tuple<T, Args...>>::value_t &value;
    GetTupleValue(const Tuple<T, Args...> &t) : value(GetTupleValue<i-1, Tuple<Args...>>(t.next).value) {}
};
template <typename T, typename ...Args>
class GetTupleValue<0, Tuple<T, Args...>> {
public:
    const T &value;
    GetTupleValue(const Tuple<T, Args...> &t) : value(t.value) {}
};
template <size_t i, typename T, typename ...Args>
constexpr auto getValue(const Tuple<T, Args...> &t) -> decltype(GetTupleValue<i, Tuple<T, Args...>>(t).value) {
    return GetTupleValue<i, Tuple<T, Args...>>(t).value;
}
template <typename ...Args>
constexpr Tuple<Args...> makeTuple(const Args&... args) {
    return std::move(Tuple<Args...>(args...));
}
int main(int argc, const char * argv[]) {
    // insert code here...
    // std::cout << "Hello, World!\n";
    int a = 4;
    std::cin >> a;
    auto t = makeTuple(a, 4.3, 'A');
    Tuple<int, double, char> t1 = t;
    printf("%lf\n", t.next.value);
    printf("%c\n", t.next.next.value);
    typename GetTupleType<2, Tuple<int, double, char>>::value_t num = 'A';
    printf("%c\n", num);
    std::cout << getValue<0>(t1) << std::endl;
    return 0;

}