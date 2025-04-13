#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Function to shift the DFT quadrants to center the low frequencies
void shiftDFT(Mat &magnitude)
{
    int cx = magnitude.cols / 2;
    int cy = magnitude.rows / 2;

    Mat q0(magnitude, Rect(0, 0, cx, cy));   // Top-Left
    Mat q1(magnitude, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magnitude, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magnitude, Rect(cx, cy, cx, cy)); // Bottom-Right

    // Swap quadrants diagonally
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

// Function to compute and display the magnitude spectrum
Mat computeMagnitudeSpectrum(Mat &img)
{
    // Convert image to float and expand to optimal size
    Mat padded;
    int m = getOptimalDFTSize(img.rows);
    int n = getOptimalDFTSize(img.cols);
    copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

    // Create planes for real and imaginary parts
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;

    merge(planes, 2, complexImg); // Merge into a complex image

    // Compute DFT
    dft(complexImg, complexImg);

    // Split into real and imaginary parts
    split(complexImg, planes);
    Mat magnitudeImg;
    magnitude(planes[0], planes[1], magnitudeImg);

    // Switch to logarithmic scale
    magnitudeImg += Scalar::all(1); // Avoid log(0)
    log(magnitudeImg, magnitudeImg);

    // Center the low frequencies
    shiftDFT(magnitudeImg);

    // Normalize to display
    normalize(magnitudeImg, magnitudeImg, 0, 255, NORM_MINMAX);
    magnitudeImg.convertTo(magnitudeImg, CV_8U);

    return magnitudeImg;
}

int main()
{
    // Load image in grayscale
    Mat img = imread("/home/digital101/ImageProcessingBook/FourierTransform/denoise.jpg", IMREAD_GRAYSCALE);
    if (img.empty())
    {
        cout << "Error: Image not found!" << endl;
        return -1;
    }

    // Compute and display magnitude spectrum
    Mat magnitudeSpectrum = computeMagnitudeSpectrum(img);
    imshow("Original Image", img);

    imshow("Magnitude Spectrum", magnitudeSpectrum);
    waitKey(0);

    return 0;
}
