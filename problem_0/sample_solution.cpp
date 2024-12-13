#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <algorithm>

std::istream& operator>>(std::istream& in, __int128& value) {
    std::string s;
    in >> s;
    value = 0;
    bool negative = false;
    size_t i = 0;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        value = value * 10 + (s[i] - '0');
    }
    if (negative) value = -value;
    return in;
}

std::ostream& operator<<(std::ostream& out, __int128 value) {
    if (value == 0) {
        out << '0';
        return out;
    }
    std::string s;
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    while (value > 0) {
        s += '0' + static_cast<int>(value % 10);
        value /= 10;
    }
    if (negative) s += '-';
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}

std::mutex factor_mutex;

void find_factors(__int128 start, __int128 end, __int128& n, std::vector<__int128>& factors) {
    for (__int128 p = start; p <= end; ++p) {
        while (n % p == 0) {
            std::lock_guard<std::mutex> lock(factor_mutex);
            factors.push_back(p);
            n /= p;
        }
    }
}

int main() {
    __int128 n;
    std::cin >> n;
    if (n <= 1) {
        return 0;
    }
    std::vector<__int128> factors;
    const int num_threads = 4;  
    std::vector<std::thread> threads;
    __int128 max_factor = static_cast<__int128>(std::sqrt(static_cast<double>(n)) + 1);
    __int128 chunk_size = max_factor / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        __int128 start = 2 + i * chunk_size;
        __int128 end = (i == num_threads - 1) ? max_factor : start + chunk_size - 1;
        threads.emplace_back(find_factors, start, end, std::ref(n), std::ref(factors));
    }
    for (auto& t : threads) {
        t.join();
    }
    if (n > 1) {
        factors.push_back(n);
    }
    std::sort(factors.begin(), factors.end());
    for (const auto& factor : factors) {
        std::cout << factor << ' ';
    }
    std::cout << '\n';
    return 0;
}
