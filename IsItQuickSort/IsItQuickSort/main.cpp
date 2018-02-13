//
//  main.cpp
//  IsItQuickSort
//
//  Created by 区彦成 on 2018-02-12.
//  Copyright © 2018 区彦成. All rights reserved.
//

#include <iostream>
#include <vector>
#include <deque>
#include <list>
using std::vector;
using std::deque;
using std::list;
using std::cout;
using std::endl;
template <typename T>
struct TypeExtractor {
    typedef typename T::value_type value_type;
};
template <typename T>
struct TypeExtractor<T*> {
    typedef T value_type;
};
template <typename Iterator>
inline void iterSwap(Iterator &a, Iterator &b) {
    typename TypeExtractor<Iterator>::value_type temp = std::move(*a);
    (*a) = std::move(*b);
    (*b) = std::move(temp);
}
template <typename BidirectionalIterator,
        typename Compare = std::less<typename TypeExtractor<BidirectionalIterator>::value_type>>
void whatSortIsThis(BidirectionalIterator begin, BidirectionalIterator end,
        const Compare &cmp = std::less<typename TypeExtractor<BidirectionalIterator>::value_type>()) {
    if (begin == end || begin == --end) return; // --end will be executed after this line
    BidirectionalIterator i = begin;
    BidirectionalIterator j = end;
    BidirectionalIterator p = i;
    while (i != j) {
        while (cmp(*p, *j) && i != j) --j;
        while (cmp(*i, *p) && i != j) ++i;
        if (i != j) iterSwap(i, j);
    }
    if (i == j) iterSwap(i, p);
    whatSortIsThis(begin, i, cmp);
    whatSortIsThis(++i, ++end, cmp);
}
template <typename BidirectionalIterator,
        typename Compare = std::less<typename TypeExtractor<BidirectionalIterator>::value_type>>
void whatSortIsThisAgain(BidirectionalIterator begin, BidirectionalIterator end,
        const Compare &cmp = std::less<typename TypeExtractor<BidirectionalIterator>::value_type>()) {
    if (begin == end || begin == --end) return; // As is stated above
    BidirectionalIterator i = begin;
    BidirectionalIterator j = end;
    BidirectionalIterator p = i;
    while (i != j) {
        while (cmp(*p, *j) && i != j) --j;
        while ((cmp(*i, *p) || p == i) && i != j) ++i;
        if (i != j) iterSwap(i, j);
    }
    if (i == j) iterSwap(i, p);
    whatSortIsThisAgain(begin, i, cmp);
    whatSortIsThisAgain(++i, ++end, cmp);
}
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    list<int, std::allocator<int>> list0 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    whatSortIsThis(list0.begin(), list0.end());
    for (auto i : list0) {
        cout << i << " ";
    }
    cout << endl;
    list<int, std::allocator<int>> list1 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    whatSortIsThisAgain(list1.begin(), list1.end());
    for (auto i : list1) {
        cout << i << " ";
    }
    cout << endl;


    return 0;

}
