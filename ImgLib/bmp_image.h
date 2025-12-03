#pragma once
#include "img_lib.h"

#include <filesystem>
#include <iostream>

const uint32_t SIZE_FILE_HEADER = 14;
const uint32_t SIZE_FILE_INFO = 40;

namespace img_lib {
using Path = std::filesystem::path;

bool SaveBMP(const Path& file, const Image& image);
Image LoadBMP(const Path& file);

} // namespace img_lib