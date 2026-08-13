#pragma once

#include <UFile.hpp>
#include <ULogger.hpp>
#include <UMacros.hpp>
#include <URect.hpp>
#include <UTypes.hpp>
#include <vector>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <wincodec.h>
#include <winerror.h>
#include <wrl/client.h>

template<typename T> using Pointer = Microsoft::WRL::ComPtr<T>;


class UPixmap {
    private:
        uts::u32 texture_width;
        uts::u32 texture_height;
        uts::u64 texture_size;
        std::vector<uts::u8> texture_pixels;

    private:
        inline static std::once_flag co_initialize_once_flag;

    public:
        UPixmap() = default;
        UPixmap(const UFile& image_file) {
            uts::str file_path = image_file.get_file_path();
            uts::wstr image_path_wstr = uts::wstr(file_path.begin(), file_path.end());

            std::call_once(co_initialize_once_flag, [] {
                HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
                if (FAILED(result)) ULogger::ulixerr("Failed to initialize WIC");
            });

            HRESULT result = S_OK;
            Pointer<IWICImagingFactory> factory = nullptr;
            result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
            if (FAILED(result)) ULogger::ulixerr("Failed to create image factory");
            Pointer<IWICBitmapDecoder> decoder = nullptr;
            result = factory->CreateDecoderFromFilename(image_path_wstr.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
            if (FAILED(result)) ULogger::ulixerr("Failed to create image decoder");
            Pointer<IWICBitmapFrameDecode> frame = nullptr;
            result = decoder->GetFrame(0, &frame);
            if (FAILED(result)) ULogger::ulixerr("Failed to get image frame");
            result = frame->GetSize(&texture_width, &texture_height);
            if (FAILED(result)) ULogger::ulixerr("Failed to get image size");
            if (texture_width == 0 || texture_height == 0) ULogger::ulixerr("Invalid image size");
            Pointer<IWICFormatConverter> converter = nullptr;
            result = factory->CreateFormatConverter(&converter);
            if (FAILED(result)) ULogger::ulixerr("Failed to create image converter");
            result = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
            if (FAILED(result)) ULogger::ulixerr("Failed to initialize image converter");
            texture_size = texture_width * texture_height * 4;
            texture_pixels.resize(texture_size);
            result = converter->CopyPixels(nullptr, texture_width * 4, static_cast<UINT>(texture_pixels.size()), texture_pixels.data());
            if (FAILED(result)) ULogger::ulixerr("Failed to copy image pixels");
        }

    public:
        forceinline auto get_rect() const -> URect { return URect(0, 0, texture_width, texture_height); }
        forceinline auto get_pixel_size() const -> uts::u64 { return texture_size; }
        forceinline auto get_pixels() const -> const std::vector<uts::u8> { return texture_pixels; }
};
