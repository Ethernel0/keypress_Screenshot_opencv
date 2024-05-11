#include <iostream>

/// OPENCV LIB
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>

/// C++ LIB
#include <Windows.h>
#include <thread>
#include <mutex>

/// https://github.com/Ethernel0

cv::Mat getScreen (HWND hWND)
{
    /// VARS
    HDC deviceContext;
    HDC memoryDeviceContext;
    RECT windowRect;
    HBITMAP bitmap;
    int height;
    int width;
    cv::Mat mat;

    /// GET WINDOW RECT
    deviceContext = GetDC(hWND);
    memoryDeviceContext = CreateCompatibleDC(deviceContext);
    GetClientRect(hWND, &windowRect);

    height = windowRect.bottom;
    width = windowRect.right;

    bitmap = CreateCompatibleBitmap(deviceContext, width, height);
    SelectObject(memoryDeviceContext, bitmap);
    BitBlt(memoryDeviceContext, 0, 0, width, height, deviceContext, 0, 0, SRCCOPY);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    /// GET BITMAP
    mat = cv::Mat(height, width, CV_8UC3);

    /// BitCount = 32 = CV_8UC4 - With Alpha
    /// BitCount = 24 = CV_8UC3 - Without Alpha

    //  Transform data and store into mat.data
    GetDIBits(memoryDeviceContext, bitmap, 0, height, mat.data, (BITMAPINFO *) &bi, DIB_RGB_COLORS);

    //  Clean up to avoid memory leaks
    DeleteObject(bitmap);
    DeleteDC(memoryDeviceContext);
    ReleaseDC(hWND, deviceContext);
    return mat;
}

void handle(type type, int &filenum, cv::Mat const &screen)
{
    std::string directory;
    switch (type)
    {
        case type::positive:
            directory = "C:\\Users\\Ethernel\\Desktop\\ores\\iron_ore\\positive\\";
            break;
        case type::negative:
            directory = "C:\\Users\\Ethernel\\Desktop\\ores\\iron_ore\\negative\\";
            break;
    }
    std::string const filename = directory + std::to_string(filenum) + ".png";
    cv::imwrite(filename, screen);
    std::cout << filename;
    std::cout.put(std::cout.widen('\n'));
    ++filenum;
}

void screenCaptureKeys() {
    // File Nums at 0 will increment every time a file is saved
    int filenumF1 {0}, filenumF2 {0};
    ScreenCapture screencap;
    cv::Mat image_screen;

    while (true) {
        image_screen = screencap.getScreen(GetDesktopWindow());
        auto key = cv::waitKey(1);

        if (GetAsyncKeyState(VK_F1))
            handle(type::positive, filenumF1, image_screen);
        else if (GetAsyncKeyState(VK_F2))
            handle(type::negative, filenumF2, image_screen);
    }
}

int main()
{
    // call the function here !
}
