#include "Misc/List.h"
#include "Misc/Vector.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cstdio>

#include <map>
#include <iomanip>

template<typename Iterator>
Iterator high(Iterator begin, Iterator end)
{
    Iterator highest = begin;
    for(Iterator it=begin; it!=end; ++it)
        if(*highest < *it) highest = it;

    return highest;
}


template<typename In, typename In2, typename T, typename BinOp, typename BinOp2>
T inner_product(In first, In last, In2 first2, T init, BinOp op, BinOp2 op2)
{
    while(first != last)
    {
        init = op(init, op2(*first, *first2));
        ++first;
        ++first2;
    }

    return init;
};

template<typename T>
struct adds
{
    adds() = default;

    inline T operator()(T left, T right)
    {
        return left + right;
    }
};

struct multiplyPairsValues
{
    multiplyPairsValues() = default;

    inline int operator()(const std::pair<std::string, int>& left, const std::pair<std::string, int>& right)
    {
        return left.second * right.second;
    }
};


int main()
{
    std::vector<int> a { 1, 3, 5, 7, 9 };
    std::vector<int> b { 2, 4, 6, 8, 10 };

    int vProduct = std::inner_product(a.begin(), a.end(), b.begin(), 0, std::plus<int>(), std::multiplies<int>());
    std::cout << vProduct << "\n";

    std::map<std::string, int> book;
    book["John"] = 1;
    book["Gaya"] = 3;
    book["Luci"] = 5;
    book["Leonardo"] = 7;
    book["Arthur"] = 9;

    std::map<std::string, int> book2;
    book2["Za"] = 2;
    book2["Ze"] = 4;
    book2["Zi"] = 6;
    book2["Zo"] = 8;
    book2["Zu"] = 10;

    int mProduct = std::inner_product(book.begin(), book.end(), book2.begin(), 0, std::plus<int>(), multiplyPairsValues());
    std::cout << mProduct << "\n";

    return 0;
}