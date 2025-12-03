#include "sobel.h"


using namespace std;

int Sum(img_lib::Color c) {
    return to_integer<int>(c.r) + to_integer<int>(c.g) + to_integer<int>(c.b);
}

img_lib::Image Sobel(const img_lib::Image& image) {
    const int w = image.GetWidth();
    const int h = image.GetHeight();
    img_lib::Image result_image(w, h, img_lib::Color::Black());
    for (int y = 0; y + 2 < h; ++y) {
        const img_lib::Color* top_line = image.GetLine(y);
        const img_lib::Color* center_line = image.GetLine(y + 1);
        const img_lib::Color* bottom_line = image.GetLine(y + 2);        
        for (int x = 0; x + 2 < w; ++x) {
            int tl = Sum(top_line[x]);
            int tc = Sum(top_line[x + 1]);
            int tr = Sum(top_line[x + 2]);
            int cl = Sum(center_line[x]);
            int cr = Sum(center_line[x + 2]);
            int bl = Sum(bottom_line[x]);
            int bc = Sum(bottom_line[x + 1]);
            int br = Sum(bottom_line[x + 2]);
            int gx = bl + 2 * bc + br - tl - 2 * tc - tr;
            int gy = tr + 2 * cr + br - tl - 2 * cl - bl;
            byte pixel_color = static_cast<byte>(clamp<double>(sqrt(gx * gx + gy * gy), 0 ,255));
            img_lib::Color* color = &result_image.GetPixel(x + 1, y + 1);
            color->r = pixel_color;
            color->b = pixel_color;
            color->g = pixel_color;
        }
    }
    return result_image;
}

