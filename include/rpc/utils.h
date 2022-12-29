//
// Created by phanh on 29/12/2022.
//

#ifndef CRPC_UTILS_H
#define CRPC_UTILS_H

#include <utility>
#include <iostream>

namespace utils {
    template<class TupType, std::size_t... I>
    void print(const TupType& _tup, std::index_sequence<I...>)
    {
        std::cout << "(";
        (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(_tup)));
        std::cout << ")\n";
    }

    template<class... T>
    void print(const std::tuple<T...>& _tup)
    {
        print(_tup, std::make_index_sequence<sizeof...(T)>());
    }
}

#endif //CRPC_UTILS_H
