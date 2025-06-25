#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ctime>
#include <utility>
#define SRC_ROWS 3
#define SRC_COLS 3
/*
Overall goal
- Load a grayscale image.
- Pad the image appropriately (zero-padding).
- Allow the user to define a custom odd-sized kernel (e.g., 3×3).

Apply both:
    - Correlation
    - Convolution

Display both results side-by-side.
Save the output images to disk.

Functional Requirements
    - Write a function:
    - cv::Mat correlate(const cv::Mat &image, const cv::Mat &kernel);
    - Write a function:
    - cv::Mat convolve(const cv::Mat &image, const cv::Mat &kernel);
    - Write a function to flip the kernel 180°.
    - Use cv::copyMakeBorder for padding.
    - Use cv::imshow() to display results.
    - Use cv::imwrite() to save them.
*/
int main()
{
    // Seed the random number
    std::srand(std::time(0));

    // Generate a random number between 1 and 100 (inclusive)
    int min_val = 1;
    int max_val = 100;
    int random_number = min_val + (std::rand() % (max_val - min_val + 1));

    uchar data[] = {50, 0, 125, 50, 60, 25, 175, 255, 0};
    cv::Mat srcImage(SRC_ROWS, SRC_COLS, CV_8UC1, data);

    // OpenCV uses interpolation by default which will blur the pixel values:
    // cv::resize(srcImage, srcImage, cv::Size(640, 480));

    // Use this instead for visualization
    cv::Mat displayImage;
    cv::resize(srcImage, displayImage, cv::Size(640, 480), 0, 0, cv::INTER_NEAREST);
    cv::imshow("srcImage", displayImage);

    // To determine kernel size and ensure a well-defined center
    // Must use : M = 2A + 1 | N = 2B + 1

    // Calculate kernel size
    int kernelRows = (SRC_ROWS - 1) / 2; // A
    int kernelCols = (SRC_COLS - 1) / 2; // B

    printf("\n\nKernel size : [ %d ] X [ %d ] \r\n", kernelRows, kernelCols);

    // Assign kernel value

    // Use if kernel size > 1
    // std::pair<int,int>kernelSize = {kernelRows,kernelCols};

    // Use if kernel size == 1
    int kernel = random_number;

    printf("\nKernel value to be used : %d\r\n", kernel);

    printf("\n\nsrcImage values\r\n");

    for (int i = 0; i < srcImage.rows; ++i)
    {
        for (int j = 0; j < srcImage.cols; ++j)
        {
            // uchar has to be used since we told OpenCV to use unsigned
            // 8-bit integers CV_8UC1 and not regular ints
            std::cout << static_cast<int>(srcImage.at<uchar>(i, j)) << "\t";
        }
        std::cout << std::endl;
    }

    // Traverse srcImage
    for (int y = kernelRows; y < srcImage.rows - kernelRows; ++y)
    {
        for (int x = kernelCols; x < srcImage.cols - kernelCols; ++x)
        {
            // apply kernel centered at (x, y)
            // std::cout << static_cast<int>(srcImage.at<uchar>(x, y)) << "\t";
        }
        std::cout << std::endl;
    }

    cv::waitKey(0);

    return 0;
}
