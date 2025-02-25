#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <utility>
#include <chrono>
#include "csv.hpp"
#include "greedy.hpp"
#define ERR 6

int main() {
    std::string filename = "cinema.csv";
    std::vector<std::pair<unsigned long, int>> data = read_csv(filename);

    auto start_create_time = std::chrono::high_resolution_clock::now();
    GreedySpline A(ERR);
    load_data(A, data);
    auto end_create_time = std::chrono::high_resolution_clock::now();
    std::cout << "vector lenght : " <<A.greedyspline.size() << std::endl;
    auto create_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_create_time - start_create_time);
    std::cout << "create time: " << create_duration.count() << " ms" << std::endl;
    return 0;
}