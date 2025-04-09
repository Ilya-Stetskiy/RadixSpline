#include "radix.hpp"

using namespace std;


RadixSpline::RadixSpline(const GreedySpline& greedySpline, int bitcount)
        : splineType(greedySpline), spline(greedySpline.greedyspline),bitcount(bitcount),  error(greedySpline.error)  {
        radixspline.resize(1 << bitcount, -1);
        bit_len = bit_length(spline.back().first);
        len_greedy = spline.size();
        pair<int, int> result = not_prefix_count();
        not_prefix = result.first;
        mod = result.second;
        move_right = bit_len - bitcount - not_prefix;
        get_RadixSpline();
    }

    void RadixSpline::print_spline() const {
        for (const auto& point : radixspline) {
            std::cout << "(" << point<<")" << std::endl;
        }
    }

    int RadixSpline::bit_length(int n) {
        if (n == 0) return 1;
        return static_cast<int>(log2(n)) + 1;
    }

    pair<int, int> RadixSpline::not_prefix_count() {
        int n = spline[0].first;
        int count = 0, pref = 0;

        if (bit_length(n) == bit_len) {
            count = bit_len - bit_length(((1 << bit_len) - 1) ^ n);
        }
        for (int i = 0; i < count; ++i) {
            pref = (pref << 1) + 1;
        }
        pref <<= (bit_len - count);
        return {count, pref};
    }

    int RadixSpline::first_bits(int n) {
        if (not_prefix) {
            n %= mod;
        }
        return n >> move_right;
    }

    void RadixSpline::get_RadixSpline() {
        int idx=-1;  
        for (size_t ind = 0; ind < spline.size(); ++ind) {
            int temp = first_bits(spline[ind].first);
            if (temp > idx) {
                radixspline[temp] = static_cast<int>(ind);
                idx=temp;
            }
        }

        if (radixspline[(1 << bitcount) - 1] == -1) {
            radixspline[(1 << bitcount) - 1] = static_cast<int>(spline.size()) - 1;
        }
        for (int i = (1 << bitcount) - 2; i >= 0; --i) {
            if (radixspline[i] == -1) {
                radixspline[i] = radixspline[i + 1];
            }
        }
    }


    unsigned long RadixSpline::give_answer(unsigned long key) {
        int first_bit = first_bits(key);

        int left = std::max(radixspline[first_bit] - 1,0);
        int right;
        if (first_bit == static_cast<int>(radixspline.size()) - 1) {
            right = static_cast<int>(spline.size()) - 1;
        } else {
            if (first_bit + 1 < static_cast<int>(radixspline.size())) {
                right = radixspline[first_bit + 1];
            } else {
                right = -1;
            }
        }
        auto bin_ans = splineType.bin_search_spline(key, left, right);
        unsigned long kl = spline[bin_ans[0]].first;
        int pl = spline[bin_ans[0]].second;
        unsigned long kr = spline[bin_ans[1]].first;
        int pr = spline[bin_ans[1]].second;
        
    
        return pl + (key - kl) * (pr - pl) / std::max(1.,((kr - kl)*1.));
    }

// int bin_search(const vector<pair<unsigned long, int>>& data, unsigned long key, int left, int right) {
//     while (left <= right) {
//         int mid = (left + right) / 2;
//         if (data[mid].first == key) {
//             return mid;
//         } else if (data[mid].first < key) {
//             left = mid + 1;
//         } else {
//             right = mid - 1;
//         }
//     }
//     return -1;
// }

int bin_search(const std::vector<std::pair<unsigned long, int>>& data, unsigned long key, int left, int right) {
    auto it = lower_bound(data.begin() + left, data.begin() + right + 1, key,
        [](const std::pair<unsigned long, int>& elem, double key) {
            return elem.first < key;
        });

    if (it != data.end() && it->first == key) {
        return distance(data.begin(), it);
    }
    return -1;
}


int get_key(const vector<pair<unsigned long, int>>& data, RadixSpline& spline, const pair<unsigned long, int>& key) {
    unsigned long radix_answer = spline.give_answer(key.first);
    // std::cout <<  radix_answer<< std::endl;
    int left = static_cast<int>(radix_answer - spline.splineType.error);
    int right = static_cast<int>(radix_answer + spline.splineType.error);

    left = max(left, 0);
    right = min(right, spline.splineType.last_point.second);
    // std::cout <<  spline.splineType.error<< std::endl;

    int res = bin_search(data, key.first, left, right);

    if (res != key.second) {
        throw runtime_error("FAIL: " + to_string(res) + ", " + to_string(radix_answer) +
                            ", " + to_string(left) + ", " + to_string(right));
    }
    return res;
}




// Функция для построения сплайнов
RadixSpline get_spline(const vector<pair<unsigned long, int>>& data, int ERR, int bitcount) {
    GreedySpline G(ERR);
    load_data(G, data);

    RadixSpline R(G, bitcount);
    return R;
}
