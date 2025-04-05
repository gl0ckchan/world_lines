#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <cmath>

#include "visualize/process_image.h"

std::vector<float> divergence_number_generator(float lower_limit, float upper_limit, int num)
{
    std::mt19937 mt { std::random_device{}() };
    std::vector<float> result;
    for (int i = 0; i < num; ++i) {
        float random_number = lower_limit + std::generate_canonical<float, 128>(mt) * (upper_limit - lower_limit);
        float rounded = std::round(random_number * 1e6) / 1e6;
        result.push_back(rounded);
    }

    return result;
}

int main(int argc, char *argv[])
{
    const float STEINS_GATE_DIVERGENCE = 1.048596;

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <number of world lines> [output.png]" << std::endl;
        std::cout << "\toutput.png - output image with divergence" << std::endl;
        std::cerr << "ERROR: Expected number of world lines" << std::endl;
        return 1;
    }

    int world_lines = std::stoi(argv[1]);

    int attractor_fields = 2; // Alpha and Beta

    float divergence_base = 0.0;
    float divergence_capacity = (0.99 + static_cast<float>(attractor_fields) - 1.0);

    std::vector<float> alpha;
    std::vector<float> beta;

    std::vector<float> divergence_numbers { divergence_number_generator(divergence_base, divergence_capacity, world_lines) };
    float total = 0.0;
    for (float divergence_num : divergence_numbers) {
        total += divergence_num;
        if (divergence_num <= 0.99 && divergence_num >= 0.0)
            alpha.push_back(divergence_num);
        else
            beta.push_back(divergence_num);
    }

    float average = total / divergence_numbers.size();
    float relative_divergence = std::abs(average - STEINS_GATE_DIVERGENCE);

    std::cout << "Alpha lines: [ ";
    for (float num : alpha) {
        std::cout << num << ' ';
    }
    std::cout << "]" << std::endl;

    std::cout << "Beta lines: [ ";
    for (float num : beta) {
        std::cout << num << ' ';
    }
    std::cout << "]" << std::endl;

    std::cout << "Divergence: " << relative_divergence << std::endl;

    if (argc == 3) {
        std::string output_image = argv[2];
        if (!output_image.ends_with(".png")) {
            std::cerr << "ERROR: Incorrect output file name" << std::endl;
            return 1;
        }
        process_image(output_image, relative_divergence);
    }

    return 0;
}
