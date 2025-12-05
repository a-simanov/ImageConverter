#ifndef CONVERTER_H
#define CONVERTER_H

#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>
#include <bmp_image.h>

#include <filesystem>
#include <string_view>
#include <QMainWindow>

using namespace std;
using namespace img_lib;

enum Format {
    JPEG,
    PPM,
    BMP,
    UNKNOWN,
};

class ImageFormatInterface : QObject{
    Q_OBJECT
public:
    virtual bool SaveImage(const Path& file, const Image& image) const = 0;
    virtual Image LoadImage(const Path& file) const = 0;
};

class InterfacePPM : public ImageFormatInterface {
public:
    bool SaveImage(const Path& file, const Image& image) const override {
        return SavePPM(file, image);
    }
    Image LoadImage(const Path& file) const override {
        return LoadPPM(file);
    }
};

class InterfaceJPEG : public ImageFormatInterface {
public:
    bool SaveImage(const Path& file, const Image& image) const override {
        return SaveJPEG(file, image);
    }
    Image LoadImage(const Path& file) const override {
        return LoadJPEG(file);
    }
};

class InterfaceBMP : public ImageFormatInterface {
public:
    bool SaveImage(const Path& file, const Image& image) const override {
        return SaveBMP(file, image);
    }
    Image LoadImage(const Path& file) const override {
        return LoadBMP(file);
    }
};

static inline Format GetFormatByExtension(const Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }
    if (ext == ".ppm"sv) {
        return Format::PPM;
    }
    if (ext == ".bmp"sv) {
        return Format::BMP;
    }
    return Format::UNKNOWN;
}

static inline ImageFormatInterface* GetFormatInterface(const Path& path) {
    Format format = GetFormatByExtension(path);
    static InterfacePPM ppm;
    static InterfaceJPEG jpeg;
    static InterfaceBMP bmp;
    if (format == Format::JPEG) {
        return &jpeg;
    } else if (format == Format::PPM) {
        return &ppm;
    } else if (format == Format::BMP) {
        return &bmp;
    }
    return nullptr;
}
#endif
