#ifndef RADIX_HPP
#define RADIX_HPP

#include <vector>
#include <utility>
#include <iostream>
#include <ostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <limits>
#include "greedy.hpp"


class RadixSpline {
public:
    RadixSpline(const GreedySpline& greedySpline, int bitcount);
    void print_spline() const ;
    int bit_length(int n);
    std::pair<int, int> not_prefix_count();
    int first_bits(int n);
    void get_RadixSpline();
    unsigned long give_answer(unsigned long key);
    GreedySpline splineType;
    std::vector<int> radixspline;
private:
    std::vector<std::pair<unsigned long, int>> spline;
    int bitcount;
    int error;
    int bit_len;
    int len_greedy;
    int not_prefix;
    int mod;
    int move_right;
};


// Функция для получения ключа
int get_key(const std::vector<std::pair<unsigned long, int>>& data, RadixSpline& spline, const std::pair<unsigned long, int>& key);

// Функция для построения сплайнов
RadixSpline get_spline(const std::vector<std::pair<unsigned long, int>>& data, int ERR, int bitcount);


#endif
