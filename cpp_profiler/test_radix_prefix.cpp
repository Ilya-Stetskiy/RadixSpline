#include "greedy.hpp"
#include "radix.hpp"

#define SPLINEERROR 5
#define BITLENGHTSTART 1
#define BITLENGHTFIN 15
#define FILENAME "cinema.csv"

int main() {
    std::string filename = FILENAME;
    std::vector<std::pair<unsigned long, int>> data = read_csv(filename);
    for (size_t bitlenght = BITLENGHTSTART; bitlenght <= BITLENGHTFIN; bitlenght++)
    {
        std::cout << "bitlenght : " <<bitlenght<< std::endl;
        auto start_create_time = std::chrono::high_resolution_clock::now();
        GreedySpline G(SPLINEERROR);
        load_data(G, data);
        RadixSpline R(G, bitlenght);
        auto end_create_time = std::chrono::high_resolution_clock::now();
    
        std::cout << "GreedySpline lenght : " <<G.greedyspline.size() << std::endl;
        std::cout << "RadixSpline lenght : " <<R.radixspline.size() << std::endl;
        auto create_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_create_time - start_create_time);
        std::cout << "create time: " << create_duration.count() << " ms" << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < data.size(); ++i) 
        {
            try {
                std::pair<unsigned long, int> key = data[i];
                get_key(data, R,key);
            } catch (const std::exception& e) {
                std::cerr << "ERROR (" << data[i].first << ", " << data[i].second << "): " << e.what() << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Search time: " << duration.count() << " ms" << std::endl;
        
    }
}