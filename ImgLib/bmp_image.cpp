#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    BitmapInfoHeader() = default;
    BitmapInfoHeader(const Image& image) {
        size_ = sizeof(BitmapInfoHeader);
        width_ = image.GetWidth();
        height_ = image.GetHeight();
        img_size_ = GetBMPStride(image.GetWidth()) * image.GetHeight();
    }
    uint32_t size_{};
    int32_t width_{};
    int32_t height_{};
    uint16_t planes_ = 1;
    uint16_t bits_ = 24;
    uint32_t compression_{};
    uint32_t img_size_{};
    int32_t horizontal_resolution_= 11811;
    int32_t vertival_resolution_ = 11811;
    int32_t used_colors_{};
    int32_t significant_colors_ = 0x1000000;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapFileHeader {
    BitmapFileHeader() = default;
    BitmapFileHeader(const Image& image) {
        size_ = sizeof(BitmapInfoHeader) + sizeof(BitmapFileHeader) + GetBMPStride(image.GetWidth()) * image.GetHeight();
        offset_ = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
    }
    std::array<char, 2> sign_{'B', 'M'};
    uint32_t size_{};
    uint32_t reserve_{};
    uint32_t offset_{};
}
PACKED_STRUCT_END

bool SaveBMP(const Path& file, const Image& image) {
    BitmapFileHeader file_header(image);
    BitmapInfoHeader info_header(image);
    
    ofstream out(file, ios::binary);
    out.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    out.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));
    
    const int h = image.GetHeight();
    const int w = image.GetWidth();
    const int strided = GetBMPStride(w);
    vector<char> buff(strided);
    
    for (int y = h - 1; y >= 0; --y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < w; ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        out.write(reinterpret_cast<const char*>(buff.data()), buff.size());
    }
    return out.good();
}

bool CheckCorrectHeader (const BitmapFileHeader &header, const BitmapInfoHeader &info) {
    if(header.sign_[0] != 'B' || header.sign_[1] != 'M') {
        return false;
    }

    if (info.width_ <= 0 || info.height_ <= 0) {
        return false;
    }
    return true;
}

Image LoadBMP(const Path& file){
    BitmapFileHeader file_header{};
    BitmapInfoHeader info_header{};    
    
    ifstream ifs(file, ios::binary);
    if (!ifs.is_open()) {
        cerr << "Cann't open the file";
        return {};
    }
    ifs.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    ifs.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));    

    if (!CheckCorrectHeader(file_header, info_header)) {
        cerr << "Incorrect reading data" << "\n";
        return {};
    }

    int w = info_header.width_;
    int h = info_header.height_;   

    int stride = GetBMPStride(w);
    Image image(w, h, Color::Black());        
    
    for (int y = h - 1; y >= 0; --y) {
        std::vector<char> buff(w * 3);
        Color* line = image.GetLine(y);
        ifs.read(buff.data(), stride);
        for (int x = 0; x < w; ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }
    return image;
}  

}  // namespace img_lib