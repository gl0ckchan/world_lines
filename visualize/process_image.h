#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <sstream>
#include <cstring>

struct Image
{
    std::string filename;
    int w = 0;
    int h = 0;
    int n = 0;
    uint8_t *data = nullptr;
};

int get_char_index(char ch);
std::string clean_float_to_string(float value);
uint8_t *extract_char_image(uint8_t *img, int img_width, int img_height, int channels, char ch);
uint8_t *render_text_to_image(const std::string &text, const Image &font_img, int &out_width, int &out_height);
void process_image(std::string &file, float divergence);
