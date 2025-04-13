#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat originalImg, adjustedImg;
int gammaValue = 100; // Initial gamma value (scaled by 100 for precision)

// Function to apply gamma correction
void applyGammaCorrection(int, void *)
{
    double gamma = gammaValue / 100.0; // Convert trackbar value to gamma (range: 0.01 - 5.0)

    // Create the LUT
    Mat lookupTable(1, 256, CV_8U);
    // Retreieve the pointer 'lut' to the first element of that memory block and allowing access to the pixel intensity mapping
    uchar *lut = lookupTable.ptr();

    // Precompute all possible values in the LUT
    for (int i = 0; i < 256; i++)
    {
        lut[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }

    // Apply gamma correction
    LUT(originalImg, lookupTable, adjustedImg); 
    imshow("Gamma Correction", adjustedImg);
}

int main()
{
    // Load grayscale image
    originalImg = imread("input.jpg", IMREAD_GRAYSCALE);
    if (originalImg.empty())
    {
        cout << "Error: Could not open image!" << endl;
        return -1;
    }

    namedWindow("Gamma Correction", WINDOW_AUTOSIZE);

    // Create trackbar (value from 1 to 500, representing gamma 0.01 to 5.00)
    createTrackbar("Gamma x100", "Gamma Correction", &gammaValue, 500, applyGammaCorrection);

    applyGammaCorrection(0, 0); // Initial call to display the image

    waitKey(0);
    return 0;
}
