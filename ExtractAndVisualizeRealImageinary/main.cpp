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

int main()
{
    // Step 1: Load image in grayscale
    Mat img = imread("/home/digital101/ImageProcessingBook/FourierTransform/denoise.jpg", IMREAD_GRAYSCALE);
    if (img.empty())
    {
        cout << "Error: Image not found!" << endl;
        return -1;
    }

    // Step 2: Convert to float and prepare for DFT
    Mat padded;
    int m = getOptimalDFTSize(img.rows);
    int n = getOptimalDFTSize(img.cols);
    copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

    // Step 3: Create planes for real and imaginary parts
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg); // Merge into a complex image

    // Step 4: Compute DFT
    dft(complexImg, complexImg);

    // Step 5: Split into real and imaginary parts
    Mat realPart, imagPart;
    split(complexImg, planes);
    realPart = planes[0]; // Real part
    imagPart = planes[1]; // Imaginary part

    // Normalize real and imaginary parts for visualization
    normalize(realPart, realPart, 0, 255, NORM_MINMAX);
    normalize(imagPart, imagPart, 0, 255, NORM_MINMAX);
    realPart.convertTo(realPart, CV_8U);
    imagPart.convertTo(imagPart, CV_8U);

    // Step 6: Compute Magnitude Spectrum
    Mat magnitudeImg;
    magnitude(planes[0], planes[1], magnitudeImg);
    magnitudeImg += Scalar::all(1); // Avoid log(0)
    log(magnitudeImg, magnitudeImg);
    shiftDFT(magnitudeImg); // Shift the quadrants to center the spectrum
    normalize(magnitudeImg, magnitudeImg, 0, 255, NORM_MINMAX);
    magnitudeImg.convertTo(magnitudeImg, CV_8U);

    // Step 7: Display results
    imshow("Original Image", img);
    imshow("Real Part", realPart);
    imshow("Imaginary Part", imagPart);
    imshow("Magnitude Spectrum", magnitudeImg);

    waitKey(0);
    return 0;
}
