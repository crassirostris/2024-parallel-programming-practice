#include <iostream>
#include <vector>
#include <algorithm>

void generate_sampled_output(size_t length, size_t a, size_t b, size_t p, size_t output_sampling) {
    std::vector<size_t> sampled_elements;
    size_t current = a % p;

    // Collect samples directly
    for (size_t i = 0; i < length; ++i) {
        if (i % output_sampling == 0) {
            sampled_elements.push_back(current);
        }
        current = (current * a + b) % p;
    }

    // Sort the sampled elements
    std::sort(sampled_elements.begin(), sampled_elements.end());

    // Output the results
    for (const auto& elem : sampled_elements) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

int main() {
    size_t length, a, b, p, output_sampling;
    std::cin >> length >> a >> b >> p >> output_sampling;
    generate_sampled_output(length, a, b, p, output_sampling);
    return 0;
}
