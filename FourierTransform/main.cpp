#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// Function to display an image
void showImage(const string &winName, Mat &img)
{
    Mat display;
    normalize(img, display, 0, 255, NORM_MINMAX);
    display.convertTo(display, CV_8U);
    imshow(winName, display);
    // waitKey(0);
}

// Function to compute DFT and return the complex image
Mat performDFT(Mat &img, Mat &magnitudeImg)
{
    Mat planes[] = {Mat_<float>(img), Mat::zeros(img.size(), CV_32F)};
    Mat complexImg;

    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);
    split(complexImg, planes);

    magnitude(planes[0], planes[1], magnitudeImg);
    magnitudeImg += Scalar::all(1);
    log(magnitudeImg, magnitudeImg);

    normalize(magnitudeImg, magnitudeImg, 0, 1, NORM_MINMAX);
    return complexImg;
}

// Function to compute inverse DFT
Mat performIDFT(Mat &complexImg)
{
    Mat inverseTransform;
    idft(complexImg, inverseTransform, DFT_REAL_OUTPUT);
    normalize(inverseTransform, inverseTransform, 0, 1, NORM_MINMAX);
    return inverseTransform;
}

// Function to create a low-pass filter
Mat createLowPassFilter(Size size, int cutoff)
{
    Mat filter(size, CV_32F, Scalar(0));
    Point center(size.width / 2, size.height / 2);

    for (int i = 0; i < size.height; i++)
    {
        for (int j = 0; j < size.width; j++)
        {
            double distance = sqrt(pow(i - center.y, 2) + pow(j - center.x, 2));
            if (distance < cutoff)
                filter.at<float>(i, j) = 1.0;
        }
    }
    return filter;
}

// Function to create a high-pass filter
Mat createHighPassFilter(Size size, int cutoff)
{
    Mat filter(size, CV_32F, Scalar(1));
    Point center(size.width / 2, size.height / 2);

    for (int i = 0; i < size.height; i++)
    {
        for (int j = 0; j < size.width; j++)
        {
            double distance = sqrt(pow(i - center.y, 2) + pow(j - center.x, 2));
            if (distance < cutoff)
                filter.at<float>(i, j) = 0.0;
        }
    }
    return filter;
}

// Function to apply a frequency filter
Mat applyFilter(Mat &complexImg, Mat &filter)
{
    Mat planes[2];
    split(complexImg, planes);
    planes[0] = planes[0].mul(filter);
    planes[1] = planes[1].mul(filter);
    Mat filteredImg;
    merge(planes, 2, filteredImg);
    return filteredImg;
}

// Function to compute optimal cutoff frequency based on energy retention
int computeOptimalCutoff(Mat &magnitudeImg, double energyPercentage)
{
    // Compute total energy
    double totalEnergy = sum(magnitudeImg)[0];

    // Find the radius that retains the desired energy
    double accumulatedEnergy = 0.0;
    int optimalCutoff = 1;
    Point center(magnitudeImg.cols / 2, magnitudeImg.rows / 2);

    for (int r = 1; r < min(center.x, center.y); r++)
    {
        for (int i = 0; i < magnitudeImg.rows; i++)
        {
            for (int j = 0; j < magnitudeImg.cols; j++)
            {
                double distance = sqrt(pow(i - center.y, 2) + pow(j - center.x, 2));
                if (distance < r)
                    accumulatedEnergy += magnitudeImg.at<float>(i, j);
            }
        }
        if (accumulatedEnergy / totalEnergy >= energyPercentage)
        {
            optimalCutoff = r;
            break;
        }
    }
    return optimalCutoff;
}

int main()
{
    // Load image in grayscale
    Mat img = imread("/home/digital101/ImageProcessingBook/FourierTransform/denoise.jpg", IMREAD_GRAYSCALE);
    Mat cloneImg = img.clone();
    if (img.empty())
    {
        cout << "Error: Could not load image!" << endl;
        return -1;
    }

    // Convert image to float type
    img.convertTo(img, CV_32F);

    // Compute DFT and get magnitude spectrum
    Mat magnitudeImg;
    Mat complexImg = performDFT(img, magnitudeImg);
    showImage("Frequency Domain", magnitudeImg);

    // Compute optimal cutoff frequency (retain 95% of total energy)
    int optimalCutoff = computeOptimalCutoff(magnitudeImg, 0.95);
    cout << "Optimal Cutoff Frequency: " << optimalCutoff << endl;

    // Create and apply a low-pass filter
    Mat lowPassFilter = createLowPassFilter(img.size(), optimalCutoff);
    Mat lowPassFilteredImg = applyFilter(complexImg, lowPassFilter);
    Mat lowPassReconstructed = performIDFT(lowPassFilteredImg);
    showImage("Low-Pass Filtered Image", lowPassReconstructed);

    // Create and apply a high-pass filter
    Mat highPassFilter = createHighPassFilter(img.size(), optimalCutoff);
    Mat highPassFilteredImg = applyFilter(complexImg, highPassFilter);
    Mat highPassReconstructed = performIDFT(highPassFilteredImg);
    showImage("High-Pass Filtered Image", highPassReconstructed);
    showImage("Original Image", cloneImg);
    waitKey(0);
    return 0;
}
