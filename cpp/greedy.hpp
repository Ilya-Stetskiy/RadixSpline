#ifndef GREEDY_HPP
#define GREEDY_HPP

#include <vector>
#include <utility>
#include "csv.hpp"
// Шаблонная функция для загрузки данных
template <typename SplineType>
void load_data(SplineType& spline, const std::vector<std::pair<unsigned long, int>>& data) {
    for (const auto& point : data) {
        spline.new_spline(point);
    }
}

std::vector<std::pair<unsigned long, int>> read_csv(const std::string& filename);

class GreedySpline {
public:
GreedySpline(int error = 10);
void start(const std::pair<unsigned long, int>& new_point);
double tg_spline(const std::pair<unsigned long, int>& a, const std::pair<unsigned long, int>& b, int err);
void new_coridor(const std::pair<unsigned long, int>& a, const std::pair<unsigned long, int>& b);
void new_spline(const std::pair<unsigned long, int>& new_point);
int bin_search(const std::vector<std::pair<unsigned long, int>>& data, double key, int left, int right);
std::vector<int> bin_search_spline(int key, int start_index, int end_index);
std::vector<int> bin_search_spline(int key);
unsigned long give_answer(int key);

 
    int get_key(const std::vector<std::pair<unsigned long, int>>& data, const std::pair<unsigned long, int>& key);
    void print_spline() const;

    std::vector<std::pair<unsigned long, int>> greedyspline;
    int error;
    double tg_err_top;
    double tg_err_bot;
    std::pair<unsigned long, int> last_point;

private:


};

#endif
