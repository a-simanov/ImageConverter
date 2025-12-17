#ifndef COLORS_CHANGER_H
#define COLORS_CHANGER_H

#pragma once

#include "img_lib.h"

#include <algorithm>
#include <cmath>

using namespace std;

inline static void ChangeColors(img_lib::Image& image,int r, int g, int b) {
    const int w = image.GetWidth();
    const int h = image.GetHeight();
    for (int y = 0; y < h; ++y) {
        img_lib::Color* line = image.GetLine(y);
        for (int x = 0; x < w; ++x) {
            line[x].r = static_cast<std::byte>(std::clamp(static_cast<int>(line[x].r) + r, 0, 255));
            line[x].b = static_cast<std::byte>(std::clamp(static_cast<int>(line[x].b) + b, 0, 255));
            line[x].g = static_cast<std::byte>(std::clamp(static_cast<int>(line[x].g) + g, 0, 255));
        }
    }
}


#endif // COLORS_CHANGER_H
