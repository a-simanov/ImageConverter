#ifndef V_MIRROR_H
#define V_MIRROR_H

#include "ppm_image.h"

#include <algorithm>

using namespace std;

inline static void VMirrInplace(img_lib::Image& image) {
    const int w = image.GetWidth();
    const int h = image.GetHeight();
    for (int y = 0; y < h; ++y) {
        img_lib::Color* line = image.GetLine(y);
        int start = 0;
        int end = w - 1;
        while (start < end) {
            swap(line[start], line[end]);
            start++;
            end--;
        }
    }
}

#endif // V_MIRROR_H
