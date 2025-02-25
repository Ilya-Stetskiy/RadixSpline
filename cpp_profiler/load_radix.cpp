#include "greedy.hpp"
#include "radix.hpp"

#define SPLINEERROR 5
#define BITLENGHT 8
#define FILENAME "cinema.csv"

int main() {
    std::string filename = FILENAME;
    std::vector<std::pair<unsigned long, int>> data = read_csv(filename);

    auto start_create_time = std::chrono::high_resolution_clock::now();
    GreedySpline G(SPLINEERROR);
    load_data(G, data);
    RadixSpline R(G, BITLENGHT);
    auto end_create_time = std::chrono::high_resolution_clock::now();
    // A.print_spline();
  
    std::cout << "GreedySpline lenght : " <<G.greedyspline.size() << std::endl;
    std::cout << "RadixSpline lenght : " <<R.radixspline.size() << std::endl;
    auto create_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_create_time - start_create_time);
    std::cout << "create time: " << create_duration.count() << " ms" << std::endl;
}