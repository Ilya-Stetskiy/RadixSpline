#include "greedy.hpp"



GreedySpline::GreedySpline(int error) : error(error) {}
void GreedySpline::start(const std::pair<unsigned long, int>& new_point) {
    tg_err_top = std::numeric_limits<double>::infinity();
    tg_err_bot = -std::numeric_limits<double>::infinity();
    greedyspline.push_back(new_point);
    greedyspline.push_back(new_point);
    last_point = new_point;
}

double GreedySpline::tg_spline(const std::pair<unsigned long, int>& a, const std::pair<unsigned long, int>& b, int err) {
    return (a.second + error * err - static_cast<double>(b.second)) / (a.first - b.first);
}
void GreedySpline::new_coridor(const std::pair<unsigned long, int>& a, const std::pair<unsigned long, int>& b) {
    tg_err_top = std::min(tg_err_top, tg_spline(a, b, 1));
    tg_err_bot = std::max(tg_err_bot, tg_spline(a, b, -1));
}
void GreedySpline::new_spline(const std::pair<unsigned long, int>& new_point) {
    if (greedyspline.empty()) {
        start(new_point);
        return;
    }
    double tg_new_point = tg_spline(new_point, greedyspline[greedyspline.size() - 2],0);
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
void GreedySpline::print_spline() const {
        for (const auto& point : greedyspline) {
            std::cout << "(" << point.first << ", " << point.second << ")" << std::endl;
        }
    }
int GreedySpline::bin_search(const std::vector<std::pair<unsigned long, int>>& data, double key, int left, int right) {
        auto it = lower_bound(data.begin() + left, data.begin() + right + 1, key,
            [](const std::pair<unsigned long, int>& elem, double key) {
                return elem.first < key;
            });
    
        if (it != data.end() && it->first == key) {
            return distance(data.begin(), it);
        }
        return -1;
    }
std::vector<int> GreedySpline::bin_search_spline(unsigned long key, int start_index, int end_index) {
        if (start_index < 0 || end_index >= (int)greedyspline.size() || start_index > end_index) {
            return {-1, -1};
        }
    
        auto start_it = greedyspline.begin() + start_index;
        auto end_it = greedyspline.begin() + end_index+1;
    
        auto it = std::lower_bound(start_it, end_it, key,
            [](const std::pair<unsigned long, int>& elem, unsigned long key) {
                return elem.first < key;
            });
    
        if (it == start_it) {
            return {start_index, start_index + 1}; 
        } else if (it == end_it) {
            return {end_index - 1, end_index};
        } else {
            int mid = std::distance(greedyspline.begin(), it);
            return {mid - 1, mid};
        }
    }
std::vector<int> GreedySpline::bin_search_spline(unsigned long key) {
        return bin_search_spline(key, 0, static_cast<int>(greedyspline.size()) - 1);
    }
    
double GreedySpline::give_answer(unsigned long key) {
    auto bin_ans = bin_search_spline(key);
    if (bin_ans[0] == -1 || bin_ans[1] == -1) {
        throw std::runtime_error("Key is out of spline range");
    }

    unsigned long kl = greedyspline[bin_ans[0]].first;
    int pl = greedyspline[bin_ans[0]].second;
    unsigned long kr = greedyspline[bin_ans[1]].first;
    int pr = greedyspline[bin_ans[1]].second;
    // std::cout << "key " <<kr - kl<< std::endl;
    

    return pl + (key - kl) * (pr - pl) / std::max(1.,((kr - kl)*1.));}

int GreedySpline::get_key(const std::vector<std::pair<unsigned long, int>>& data, const std::pair<unsigned long, int>& key) {
        double radix_answer = give_answer(key.first);
        int left = static_cast<int>(radix_answer - error);
        int right = static_cast<int>(radix_answer + error);

        left = std::max(left, 0);
        right = std::min(right, last_point.second);

        int res = bin_search(data, key.first, left, right);

        if (res != key.second) {
            throw std::runtime_error("FAIL: " + std::to_string(radix_answer) + ", " + std::to_string(key.second) +
                                    ", " + std::to_string(left) + ", " + std::to_string(right));
        }
        return res;
    }












// Функция для чтения CSV-файла
std::vector<std::pair<unsigned long, int>> read_csv(const std::string& filename) {
    std::vector<std::pair<unsigned long, int>> data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string timestamp_str, value_str;


        if (!std::getline(iss, value_str, ',')) {
            continue;  
        }
        if (!std::getline(iss, timestamp_str)) {
            continue;
        }

        try {
            unsigned long timestamp = std::stoul(timestamp_str);
            int value = std::stoi(value_str);
            data.push_back({timestamp, value});
        } catch (const std::invalid_argument& e) {
            continue; 
        } catch (const std::out_of_range& e) {
            continue; 
        }
    }

    return data;
}

