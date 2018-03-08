//
//  main.cpp
//  iowrapper
//
//  Created by 区彦成 on 2018-03-08.
//  Copyright © 2018 区彦成. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <sstream>
class IOWrapper {
private:
    int fd;
    std::stringstream ss;
public:
    IOWrapper() : fd(STDOUT_FILENO), ss() {}
    ~IOWrapper() {}
    template <typename T, typename ...Args>
    ssize_t writeln(const T &n, const Args&... args) {
        ss << n << ' ';
        return writeln(args...);
    }
    template <typename T>
    ssize_t writeln(const T &n) {
        ss << n;
        ssize_t s = puts(ss.str().c_str());
        ss.clear();
        return s;
    }

    ssize_t writeln() {
        return putchar('\n');
    }

    template <typename ...Args>
    size_t readln(Args&... args) {
        std::string s;
        std::getline(std::cin, s);
        ss.clear();
        ss.str(s);
        readln_r(args...);
        return s.size();
    }

    template <typename T, typename ...Args>
    void readln_r(T &n, Args&... args) {
        ss >> n;
        readln_r(args...);
    }

    template <typename  T>
    void readln_r(T &n) {
        ss >> n;
        ss.clear();
    }


};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    IOWrapper iow;
    iow.writeln(1, 2, 3, "Hello", 3.14f, 4.3);
    int a, b, c;
    float ap;
    iow.readln(a, b, c, ap);
    iow.writeln(a, b, c, ap);
    return 0;

}