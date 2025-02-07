#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <utility>
#include <chrono>
#include "csv.hpp"

// Шаблонная функция для загрузки данных
template <typename SplineType>
void load_data(SplineType& spline, const std::vector<std::pair<int, unsigned long>>& data) {
    for (const auto& point : data) {
        spline.new_spline(point);
    }
}

class GreedySpline {
public:
    GreedySpline(int error = 10) : error(error) {}

    void start(const std::pair<int, unsigned long>& new_point) {
        tg_err_top = std::numeric_limits<double>::infinity();
        tg_err_bot = -std::numeric_limits<double>::infinity();
        greedyspline.push_back(new_point);
        greedyspline.push_back(new_point);
        last_point = new_point;
    }

    double tg_spline(const std::pair<int, unsigned long>& a, const std::pair<int, unsigned long>& b, int err = 0) {
        return (a.second + error * err - b.second) / (a.first - b.first);
    }

    void new_coridor(const std::pair<int, unsigned long>& a, const std::pair<int, unsigned long>& b) {
        tg_err_top = std::min(tg_err_top, tg_spline(a, b, 1));
        tg_err_bot = std::max(tg_err_bot, tg_spline(a, b, -1));
    }

    void new_spline(const std::pair<int, unsigned long>& new_point) {
        if (greedyspline.empty()) {
            start(new_point);
            return;
        }
        double tg_new_point = tg_spline(new_point, greedyspline[greedyspline.size() - 2]);
        if (tg_new_point < tg_err_top && tg_new_point > tg_err_bot) {
            new_coridor(new_point, greedyspline[greedyspline.size() - 2]);
            greedyspline[greedyspline.size() - 1] = new_point;
        } else {
            greedyspline.push_back(last_point);
            tg_err_top = tg_spline(new_point, greedyspline[greedyspline.size() - 1], 1);
            tg_err_bot = tg_spline(new_point, greedyspline[greedyspline.size() - 1], -1);
        }
        last_point = new_point;
    }

    void print_spline() const {
        for (const auto& point : greedyspline) {
            std::cout << "(" << point.first << ", " << point.second << ")" << std::endl;
        }
    }

    int bin_search(const std::vector<std::pair<int, unsigned long>>& data, double key, int left, int right) {
        auto it = lower_bound(data.begin() + left, data.begin() + right + 1, key,
            [](const std::pair<int, unsigned long>& elem, double key) {
                return elem.first < key;
            });

        if (it != data.end() && it->first == key) {
            return distance(data.begin(), it); // Возвращаем индекс найденного элемента
        }
        return -1; // Если элемент не найден
    }

    std::vector<int> bin_search_spline(int key) {
        auto it = std::lower_bound(greedyspline.begin(), greedyspline.end(), key,
            [](const std::pair<int, unsigned long>& elem, int key) {
                return elem.first < key;
            });

        if (it == greedyspline.begin()) {
            return {0, 1};
        } else if (it == greedyspline.end()) {
            return {static_cast<int>(greedyspline.size()) - 2, static_cast<int>(greedyspline.size()) - 1};
        } else {
            int mid = std::distance(greedyspline.begin(), it);
            return {mid - 1, mid};
        }
    }

    unsigned long give_answer(int key) {
        auto bin_ans = bin_search_spline(key);
        if (bin_ans[0] == -1 || bin_ans[1] == -1) {
            throw std::runtime_error("Key is out of spline range");
        }

        int kl = greedyspline[bin_ans[0]].first;
        unsigned long pl = greedyspline[bin_ans[0]].second;
        int kr = greedyspline[bin_ans[1]].first;
        unsigned long pr = greedyspline[bin_ans[1]].second;

        return pl + (key - kl) * (pr - pl) / std::max(1, (kr - kl));
    }

    int get_key(const std::vector<std::pair<int, unsigned long>>& data, const std::pair<int, unsigned long>& key) {
        unsigned long radix_answer = give_answer(key.first);
        int left = static_cast<int>(radix_answer - error);
        int right = static_cast<int>(radix_answer + error);

        left = std::max(left, 0);
        right = std::min(right, static_cast<int>(data.size()) - 1);

        int res = bin_search(data, key.first, left, right);

        if (res != key.second) {
            throw std::runtime_error("FAIL: " + std::to_string(res) + ", " + std::to_string(key.second) +
                                    ", " + std::to_string(left) + ", " + std::to_string(right));
        }
        return res;
    }

    std::vector<std::pair<int, unsigned long>> greedyspline;

private:
    double error;
    double tg_err_top;
    double tg_err_bot;
    std::pair<int, unsigned long> last_point;
};

// Функция для чтения CSV-файла
std::vector<std::pair<int, unsigned long>> read_csv(const std::string& filename) {
    std::vector<std::pair<int, unsigned long>> data;
    csv::CSVReader reader(filename);

    for (csv::CSVRow& row : reader) {
        int value = row[""].get<int>();
        unsigned long timestamp = row["timestamp"].get<unsigned long>();
        data.push_back({timestamp, value});
    }

    return data;
}

int main() {
    std::string filename = "../cinema.csv";
    std::vector<std::pair<int, unsigned long>> data = read_csv(filename);

    auto start_create_time = std::chrono::high_resolution_clock::now();
    GreedySpline A(6);
    load_data(A, data);
    auto end_create_time = std::chrono::high_resolution_clock::now();
    // A.print_spline();
    std::cout << "vector lenght : " <<A.greedyspline.size() << std::endl;
    auto create_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_create_time - start_create_time);
    std::cout << "Search time: " << create_duration.count() << " ms" << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < data.size(); ++i) {
        try {
            std::pair<int, unsigned long> key = data[i];
            int result = A.get_key(data, key);
        } catch (const std::exception& e) {
            std::cerr << "ERROR (" << data[i].first << ", " << data[i].second << "): " << e.what() << std::endl;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Search time: " << duration.count() << " ms" << std::endl;

    return 0;
}