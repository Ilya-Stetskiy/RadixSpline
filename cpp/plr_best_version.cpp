
#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <optional>
#include <cmath>
#include "csv.hpp"


using namespace std;

template <typename SplineType>
void load_data(SplineType& spline, const std::vector<std::pair<unsigned long, int>>& data) {
    for (const auto& point : data) {
        spline.new_spline(point);
    }
    spline.line.end_line();
    spline.PLRSpline.push_back({spline.line.startLine, spline.line.endLine});
}

// Функция для вычисления тангенса угла наклона между двумя точками
double tg(const pair<double, double>& left_point, const pair<double, double>& right_point) {
    if (left_point.first == right_point.first) {
        return numeric_limits<double>::infinity() * (right_point.second - left_point.second); // Corrected the sign
    }
    
    return (left_point.second - right_point.second) / (left_point.first - right_point.first); // Corrected the order of subtraction
}

class OneLine {
public:
    OneLine(int error = 10) : error(error) {}
    void start(const pair<unsigned long, int>& start_point_){
        if (!points.empty()){
            points.clear();
            cvx_top.clear();
            cvx_bot.clear();

        }

        start_point=start_point_;
        finish_point=start_point_;
        points.push_back(start_point);
        points.push_back(finish_point);

        auto [s_a, s_b] = err_diapason(start_point);
        auto [s_d, s_c] = err_diapason(finish_point);
        pmin = tg(s_a, s_c);

        pmax = tg(s_b, s_d);

        cvx_top.push_back(s_a);
        cvx_bot.push_back(s_b);
    }

    // Возвращает точки с учетом ошибки
    pair<pair<unsigned long, int>, pair<unsigned long, int>> err_diapason(const pair<unsigned long, int>& point) {
        return {{point.first, point.second + error}, {point.first, point.second - error}};
    }

    // Проверка, что точка не выходит за пределы допустимого диапазона
    bool check_not_outside(const pair<unsigned long, int>& plus_err, const pair<unsigned long, int>& minus_err) {
       return tg(cvx_top[0], minus_err) <= pmax && tg(cvx_bot[0], plus_err) >= pmin;
    }

    // Добавление новой точки
    bool add_point(const pair<unsigned long, int>& point) {
        auto [plus_err, minus_err] = err_diapason(point);
        bool result = check_not_outside(plus_err, minus_err);
        if (result) {
            points.push_back(point);
            // add_top_err_point(plus_err);
            if (!add_bot_err_point(minus_err, plus_err)){
                // std::cout <<"PROBLEMS"<< std::endl;
                points.pop_back();
                end_line();
                return false;
            }
            finish_point = point;
        } else {
            end_line();
        }
        return result;
    }

    // Завершение линии
    void end_line() {
        auto intersection = intersection_point();
        if (intersection) {
            
            double p = (pmin + pmax) / 2;
            // std::cout <<pmin<< "  "<< pmax<< "  "<<p<< std::endl;
            startLine = point_on_line(*intersection, p, start_point.first);
            endLine = point_on_line(*intersection, p, finish_point.first);
            // std::cout << bool(pmin < pmax)<< std::endl;
        } else {
            startLine = endLine = start_point;
        }
    }

    // Вычисление точки на линии
    pair<double, double> point_on_line(const pair<double, double>& intersection_point, double p, double target_x) {
        // std::cout <<"INTERSECTION    "<< (long)intersection_point.first<<"  "<<intersection_point.second<< std::endl;
        double x0 = intersection_point.first;
        double y0 = intersection_point.second;

        double b = y0 - p * x0;
        double y = p * target_x + b;
        return {target_x, y};
    }

    // Вычисление точки пересечения
    optional<pair<double, double>> intersection_point() {
        double x1 = cvx_top[0].first, y1 = cvx_top[0].second;
        double x2 = cvx_bot.back().first, y2 = cvx_bot.back().second;
        double x3 = cvx_bot[0].first, y3 = cvx_bot[0].second;
        double x4 = cvx_top.back().first, y4 = cvx_top.back().second;

        double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (denominator == 0) {
            return nullopt;
        }

        double x_numerator = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
        double y_numerator = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);

        double x = x_numerator / denominator;
        double y = y_numerator / denominator;

        return make_pair(x, y);
    }

    // Добавление точки с ошибкой в нижнюю границу
    bool add_bot_err_point(const pair<unsigned long, int>& new_point_bot_err, const pair<unsigned long, int>& new_point_top_err) {
        double previos_pmin=pmin, previos_pmax=pmax;
        double temp_pmin= tg(cvx_top[0], new_point_bot_err);
        double temp_pmax= tg(cvx_bot[0], new_point_top_err);
        // std::cout <<pmin<< "  "<< pmax<< "  "<<temp_pmin<< "  "<< temp_pmax<< "  "<< std::endl;
        // if (std::max(pmin, temp_pmin)> std::min(pmax,temp_pmax)){
            
        //     return false;
        // }
        int pmin_counter=0;
        int pmax_counter=0;
        // std::cout<<pmin << ' '<<pmax<<std::endl;
        if (temp_pmin > pmin) {
            while (pmin_counter< cvx_top.size()-1 && tg(cvx_top[pmin_counter+1], new_point_bot_err) > temp_pmin) {
                pmin_counter++;
                temp_pmin = tg(cvx_top[pmin_counter], new_point_bot_err);
                // cvx_top.erase(cvx_top.begin());
            }
            pmin=temp_pmin;
        }
        if (temp_pmax< pmax) {
            while (pmax_counter<cvx_bot.size()-1  && tg(cvx_bot[pmax_counter+1], new_point_top_err) < temp_pmax) {
                pmax_counter++;
                temp_pmax = tg(cvx_bot[pmax_counter], new_point_top_err);
                // cvx_bot.erase(cvx_bot.begin());
                
            }
            pmax=temp_pmax;
        }
        if (pmin > pmax){
            // std::cout<<pmin << ' '<<pmax<<std::endl;
            pmin=previos_pmin;
            pmax=previos_pmax;
            return false;
        }
        for (size_t i = 0; i < pmin_counter; ++i){cvx_top.erase(cvx_top.begin());}
        for (size_t i = 0; i < pmax_counter; ++i){cvx_bot.erase(cvx_bot.begin());}

        // std::cout <<pmin<< "  "<< pmax<< "  "<<temp_pmin<< "  "<< temp_pmax<< "  "<< std::endl;
        size_t ln;
        double last_p;
        // if (pmin>previos_pmin){
        // Обновляем cvx_bot
        ln = cvx_bot.size();
        last_p = tg(cvx_bot.back(), new_point_bot_err);
        while (ln > 1 && tg(cvx_bot[cvx_bot.size() - 2], new_point_bot_err) < last_p) {
            last_p = tg(cvx_bot[cvx_bot.size() - 2], new_point_bot_err);
             cvx_bot.pop_back(); // Удаляем последний элемент
             ln--;
            }
        cvx_bot.push_back(new_point_bot_err); // Добавляем новую точку
        // }
        // if (pmax>previos_pmax){   
        ln = cvx_top.size();
        last_p = tg(cvx_top.back(), new_point_top_err);
        while (ln > 1 && tg(cvx_top[cvx_top.size() - 2], new_point_top_err) > last_p) {
            last_p = tg(cvx_top[cvx_top.size() - 2], new_point_top_err);
            cvx_top.pop_back(); // Удаляем последний элемент
            ln--;
        }
        cvx_top.push_back(new_point_top_err);

        // }
        return true;
    }

    // // Добавление точки с ошибкой в верхнюю границу
    // void add_top_err_point(const pair<unsigned long, int>& new_point_top_err) {
    //     if (tg(cvx_bot[0], new_point_top_err) < pmax) {
    //         double temp_pmax = tg(cvx_bot[0], new_point_top_err);
    //         while (cvx_bot.size() > 1 && tg(cvx_bot[1], new_point_top_err) < temp_pmax) {
    //             temp_pmax = tg(cvx_bot[1], new_point_top_err);
    //             cvx_bot.erase(cvx_bot.begin());
    //         }
    //         pmax = temp_pmax;
    //     }
    //     // Обновляем cvx_top
    //     size_t ln = cvx_top.size();
    //     double last_p = tg(cvx_top.back(), new_point_top_err);
    //     while (ln > 1 && tg(cvx_top[cvx_top.size() - 2], new_point_top_err) > last_p) {
    //         last_p = tg(cvx_top[cvx_top.size() - 2], new_point_top_err);
    //         cvx_top.pop_back(); // Удаляем последний элемент
    //         ln--;
    //     }
    //     cvx_top.push_back(new_point_top_err);
        
    // }


public:
    pair<unsigned long, double> start_point;
    pair<unsigned long, double> finish_point;
    int error;

    vector<pair<unsigned long, int>> points;
    vector<pair<unsigned long, int>> cvx_top;
    vector<pair<unsigned long, int>> cvx_bot;

    double pmin;
    double pmax;

    pair<unsigned long, double> startLine;
    pair<unsigned long, double> endLine;

};


// Функция для чтения CSV-файла
std::vector<std::pair<unsigned long, int>> read_csv(const std::string& filename) {
    std::vector<std::pair<unsigned long, int>> data;
    csv::CSVReader reader(filename);

    // Предполагаем, что CSV-файл имеет колонки "timestamp" и "value"
    for (csv::CSVRow& row : reader) {
        int id = row[""].get<int>();
        unsigned long timestamp = row["timestamp"].get<unsigned long>();
        data.push_back({timestamp, id});
    }

    return data;

}


class PLR {
    public:
    vector<pair<pair<unsigned long, double>, pair<unsigned long, double>>> PLRSpline;
    PLR(int error = 10) : error(error), line(OneLine(error)) {}

    void start(const std::pair<unsigned long, int>& new_point)  {
        line.start(new_point);
    }
    void new_spline(const std::pair<unsigned long, int>& new_point) {
        if (line.points.empty()) {
            start(new_point);
            return;
        } 
        if (!line.add_point(new_point)) {
            
            PLRSpline.push_back({line.startLine, line.endLine});
            // line.end_line();
            line.start(new_point);
        }


    }
int bin_search_spline(unsigned long key) {
        auto it = std::lower_bound(PLRSpline.begin(), PLRSpline.end(), key,
            [](const std::pair<pair<unsigned long, int>, pair<unsigned long, int>>& elem, unsigned long key) {
                return elem.first.first <= key;
            });

        if (it == PLRSpline.begin()) {
            return {0};
        } else if (it == PLRSpline.end()) {
            return {static_cast<int>(PLRSpline.size()) - 1};
        } else {
            unsigned long mid = std::distance(PLRSpline.begin(), it);
            return mid-1;
        }
    }
double give_answer(unsigned long key) {
        unsigned long bin_ans = bin_search_spline(key);
        if (bin_ans == -1) {
            throw std::runtime_error("Key is out of spline range");
        }
        double kl = PLRSpline[bin_ans].first.first;
        double pl = PLRSpline[bin_ans].first.second;
        double kr = PLRSpline[bin_ans].second.first;
        double  pr = PLRSpline[bin_ans].second.second;

        return pl + (key - kl) * (pr - pl) / std::max(0.1, (kr - kl));
    }


int bin_search(const std::vector<std::pair<unsigned long, int>>& data, double key, int left, int right) {
        auto it = lower_bound(data.begin() + left, data.begin() + right + 1, key,
            [](const std::pair<unsigned long, int>& elem, double key) {
                return elem.first < key;
            });

        if (it != data.end() && it->first == key) {
            return distance(data.begin(), it); // Возвращаем индекс найденного элемента
        }
        return -1; // Если элемент не найден
    }

int get_key(const std::vector<std::pair<unsigned long, int>>& data, const std::pair<unsigned long, int>& key) {
        double radix_answer = give_answer(key.first);
        double left = static_cast<double>(radix_answer - error);
        double right = static_cast<double>(radix_answer + error);

        // left = std::max(left, 0.0);

        int res = bin_search(data, key.first, left, right);

        if (res != key.second) {
            throw std::runtime_error("FAIL: " + std::to_string(res) + ", " + std::to_string(radix_answer) +
                                    ", " + std::to_string(left) + ", " + std::to_string(right));
        }
        return res;
    }


    OneLine line;
    int error;

};






// vector<pair<pair<double, double>, pair<double, double>>> PLR(const vector<pair<double, double>>& data, double err) {
//     OneLine line(data[0], err);
//     vector<pair<pair<double, double>, pair<double, double>>> PLRSpline;

//     for (size_t i = 1; i < data.size(); ++i) {
//         if (!line.add_point(data[i])) {
//             line.end_line();
//             PLRSpline.push_back({line.startLine, line.endLine});
//             line = OneLine(data[i], err);
//         }
//     }
//     line.end_line();
//     PLRSpline.push_back({line.startLine, line.endLine});

//     return PLRSpline;
// }



int main() {
    // Чтение данных из CSV-файла
    std::string filename = "../cinema1.csv"; // Укажите путь к вашему CSV-файлу
    std::vector<std::pair<unsigned long, int>> data = read_csv(filename);

    // Создание и использование GreedySpline
    auto start_create_time = std::chrono::high_resolution_clock::now();
    PLR A(3);
    load_data(A, data);
    auto end_create_time = std::chrono::high_resolution_clock::now();
    // A.print_spline();
    std::cout << "vector lenght : " <<A.PLRSpline.size() << std::endl;
    auto create_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_create_time - start_create_time);
    std::cout << "Creatr time: " << create_duration.count() << " ms" << std::endl;
    for (const auto& segment : A.PLRSpline) {
        cout << "(" << segment.first.first << ", " << segment.first.second << ") -> "
             << "(" << segment.second.first << ", " << segment.second.second << ")" << endl;
    }


    // Начало измерения времени
    auto start_time = std::chrono::high_resolution_clock::now();

    // Цикл для поиска каждого элемента из data
    for (size_t i = 0; i < data.size(); ++i) {
        try {
            std::pair<unsigned long, int> key = data[i];
            int result = A.get_key(data, key);
            // std::cout <<"hui" << std::endl;
            // std::cout << "Элемент (" << key.first << ", " << key.second << ") найден на позиции: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "ERROR (" << data[i].first << ", " << data[i].second << "): " << e.what() << std::endl;
        }
    }

    // Конец измерения времени
    auto end_time = std::chrono::high_resolution_clock::now();

    // Вычисление времени выполнения
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Search time: " << duration.count() << " ms" << std::endl;

    return 0;
}