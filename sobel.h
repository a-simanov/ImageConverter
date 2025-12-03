#pragma once

#include "img_lib.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string_view>

using namespace std;

int Sum(img_lib::Color c);

img_lib::Image Sobel(const img_lib::Image& image);
