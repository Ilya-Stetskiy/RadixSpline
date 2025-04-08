#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <utility>
#include <chrono>
#include "greedy.hpp"
#define ERRSTART 2
#define ERRFIN 75

int main() {
    std::string filename = "cinema.csv";
    std::vector<std::pair<unsigned long, int>> data = read_csv(filename);
    for (size_t err = ERRSTART; err <= ERRFIN; err++)
    {
        std::cout << "err = " <<err<< std::endl;
        auto start_create_time = std::chrono::high_resolution_clock::now();
        GreedySpline A(err);
        load_data(A, data);
        auto end_create_time = std::chrono::high_resolution_clock::now();
        std::cout << "vector lenght : " <<A.greedyspline.size() << std::endl;
        auto create_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_create_time - start_create_time);
        std::cout << "create time: " << create_duration.count() << " ms" << std::endl;

        auto start_time = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < data.size(); ++i) {
            try {
                std::pair<unsigned long, int> key = data[i];
                A.get_key(data, key);
            } catch (const std::exception& e) {
                std::cerr << "ERROR (" << data[i].first << ", " << data[i].second << "): " << e.what() << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Search time: " << duration.count() << " ms" << std::endl;
    }

    return 0;
}