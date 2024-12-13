#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <thread>
#include <mutex>


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
__int128 n;
std::mutex mtx;

void factorize_range(__int128 start, __int128 end, std::vector<__int128>& factors) {
    for (__int128 p = start; p <= end; ++p) {
        if (n % p == 0) {
            std::lock_guard<std::mutex> lock(mtx);
            factors.push_back(p);
        }
    }
}

__int128 find_sqrt(__int128 n) {
    __int128 lo = 0;
    __int128 hi = n;
    while (lo < hi) {
        __int128 mid = lo + (hi - lo) / 2;
        if (mid * mid < n) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int main() {
    std:: cin >> n;
    if (n <= 1) {
        return 0;
    }

    std::vector<__int128> factors;
    std::vector<std::thread> threads;
    __int128 num_threads = 30;
    __int128 n_sqrt = find_sqrt(n);
    __int128 range = n_sqrt / num_threads;

    for (__int128 i = 0; i < num_threads; ++i) {
        __int128 start = 1 + i * range;
        start = (start < 2) ? 2 : start;
        __int128 end = (i == num_threads - 1) ? n_sqrt : start + range;
        threads.emplace_back(factorize_range, start, end, std::ref(factors));
    }

    for (auto& t : threads) {
        t.join();
    }
    std::sort(factors.begin(), factors.end());
    
    for (size_t i = 0; i < factors.size(); ++i) {
        while(n % factors[i] == 0) {
            std::cout << factors[i] << ' ';
            n /= factors[i];
        }
    }
    if (n > 1) {
        std::cout << n;
    }   
    std::cout << '\n';

    return 0;
}