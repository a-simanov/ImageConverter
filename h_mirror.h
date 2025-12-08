#ifndef H_MIRROR_H
#define H_MIRROR_H

#include "ppm_image.h"

using namespace std;

static inline void HMirrInplace(img_lib::Image& image) {
    const int w = image.GetWidth();
    const int h = image.GetHeight();
    int start = 0;
    int end = h - 1;
    while (start < end) {
        img_lib::Color* start_line = image.GetLine(start);
        img_lib::Color* end_line = image.GetLine(end);
        std::swap_ranges(start_line, start_line + w, end_line);
        start++;
        end--;
    }
}
#endif // H_MIRROR_H
