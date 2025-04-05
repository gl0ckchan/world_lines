#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "process_image.h"

const int CHARS_WIDTH =  60;
const int CHARS_HEIGHT = 145;
const std::string CHARSET = ".0123456789";

int get_char_index(char ch) {
    size_t pos = CHARSET.find(ch);
    if (pos == std::string::npos) {
        std::cerr << "Unknown character: " << ch << "\n";
        return -1;
    }

    return static_cast<int>(pos);
}

std::string clean_float_to_string(float value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << value;
    std::string result = oss.str();
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    if (result.back() == '.') result.pop_back();
    return result;
}

uint8_t *extract_char_image(uint8_t *img, int img_width, int img_height, int channels, char ch) {
    int index = get_char_index(ch);
    if (index == -1) return nullptr;

    int x_offset = index * CHARS_WIDTH;

    uint8_t *output = new uint8_t[CHARS_WIDTH * CHARS_HEIGHT * channels];

    for (int y = 0; y < CHARS_HEIGHT; ++y) {
        for (int x = 0; x < CHARS_WIDTH; ++x) {
            for (int c = 0; c < channels; ++c) {
                output[(y * CHARS_WIDTH + x) * channels + c] =
                    img[((y) * img_width + (x + x_offset)) * channels + c];
            }
        }
    }

    return output;
}

uint8_t *render_text_to_image(const std::string &text, const Image &font_img, int &out_width, int &out_height) {
    int channels = font_img.n;
    int total_chars = text.size();
    out_width = CHARS_WIDTH * total_chars;
    out_height = CHARS_HEIGHT;

    uint8_t *final_image = new uint8_t[out_width * out_height * channels];

    for (int i = 0; i < total_chars; ++i) {
        char c = text[i];
        uint8_t *symbol = extract_char_image(font_img.data, font_img.w, font_img.h, font_img.n, c);
        if (!symbol) continue;

        int x_offset = i * CHARS_WIDTH;

        for (int y = 0; y < CHARS_HEIGHT; ++y) {
            uint8_t *dst = final_image + ((y * out_width + x_offset) * channels);
            uint8_t *src = symbol + (y * CHARS_WIDTH * channels);
            memcpy(dst, src, CHARS_WIDTH * channels);
        }

        delete[] symbol;
    }

    return final_image;
}

void process_image(std::string &file, float divergence)
{
    Image numbers { .filename = "./visualize/res/charset.png" };
    if (!stbi_info(numbers.filename.c_str(), &numbers.w, &numbers.h, &numbers.n)) {
        std::cerr << "ERROR: Could not parse image info for " << numbers.filename << std::endl;
        return;
    }

    numbers.data = stbi_load(numbers.filename.c_str(), &numbers.w, &numbers.h, &numbers.n, 0);
    if (!numbers.data) {
        std::cerr << "ERROR: Could not load image data for " << numbers.filename << std::endl;
        return;
    }

    int out_w, out_h;
    std::string divergence_str = clean_float_to_string(divergence);

    uint8_t *rendered = render_text_to_image(divergence_str, numbers, out_w, out_h);
    stbi_write_png(file.c_str(), out_w, out_h, numbers.n, rendered, out_w * numbers.n);

    delete[] rendered;
    stbi_image_free(numbers.data);
}
