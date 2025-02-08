#ifndef RADIX_HPP
#define RADIX_HPP

#include <vector>
#include <utility>
#include "csv.hpp"
#include "greedy.hpp"


class RadixSpline {
public:
    RadixSpline(const GreedySpline& greedySpline, int bitcount);

    void print_spline() const ;

    // Метод для получения длины числа в битах
    int bit_length(int n);

    // Метод для подсчета not_prefix и mod
    std::pair<int, int> not_prefix_count();

    // Метод для получения первых битов числа
    int first_bits(int n);

    // Метод для построения RadixSpline
    void get_RadixSpline();


    // Метод для получения ответа
    unsigned long give_answer(int key);
    GreedySpline splineType;
    std::vector<int> radixspline;
private:
    
    int bitcount;
    std::vector<std::pair<unsigned long, int>> spline;
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
