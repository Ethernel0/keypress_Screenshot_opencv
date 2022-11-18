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

class  perfTimer
{   /// timer class, used for performance measurement   |   Cherno Tutorial
public:
    perfTimer()
    {
        mStartTimepoint = std::chrono::high_resolution_clock::now();
    }
    ~perfTimer()
    {
        Stop();
    }

    void Stop()
    {
        auto endTimePoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
        auto duration = end - start;
        double ms = duration * 0.001;
        std::cout << duration << "us (" << ms << "ms)" << "\n";
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTimepoint;
};

cv::Mat getScreen (HWND hWND)
{
    //  PerfTimer timer;
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

int main()
{
    // File Nums at 0 will increment every time a file is saved
    int filenumF1{0}, filenumF2{0};

    cv::Mat screen_mat;

    while (true)
    {
        perfTimer timer;                                                            /*Measure performance of the loop*/
        screen_mat = getScreen(GetDesktopWindow());                                 /*Get the screen*/

        std::string filename_f1, filename_f2;                                       /*File names for saving*/
        std::ostringstream os_positive, os_negative;                                /*String stream for file names*/

        /*Reset the file name and number every run to avoid duplicates*/
        os_positive << "C:\\Users\\Ethernel\\Desktop\\" << filenumF1 << ".png";
        os_negative << "C:\\Users\\Ethernel\\Desktop\\" << filenumF2 << ".png";

        filename_f1 = os_positive.str();                                            /*Convert the string stream to a string, this is the part which can duplicate*/
        filename_f2 = os_negative.str();

        if (GetAsyncKeyState(VK_F1))                                                /*Check if F1 is pressed    |   KeyCode VK_F1 */
        {                                                                           /*https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes*/
            cv::imwrite(filename_f1 , screen_mat);                                  /*Save the screen to a file*/
            std::cout << filename_f1 << "\n";                                       /*Print the file name to the console*/
            filenumF1++;
        }

        else if (GetAsyncKeyState(VK_F2))                                           /*Same things here instead wait for F2*/
        {
            cv::imwrite(filename_f2, screen_mat);
            std::cout << filename_f2 << "\n";
            filenumF2++;
        }
        else {continue;}
    }

    // Using the example above you can leave the loop or exit the program

}




