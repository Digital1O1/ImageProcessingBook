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
        - Write your own
    - Write a function:
    - cv::Mat convolve(const cv::Mat &image, const cv::Mat &kernel);
        - Write your own
    - Write a function to flip the kernel 180°.
    - Use cv::copyMakeBorder for padding.
        - Use opnecv's
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

    uchar data[] = {50, 125, 125, 50, 60, 25, 175, 150, 100};
    cv::Mat srcImage(SRC_ROWS, SRC_COLS, CV_8UC1, data);

    // OpenCV uses interpolation by default which will blur the pixel values:
    // cv::resize(srcImage, srcImage, cv::Size(640, 480));

    // Use this instead for visualization
    // cv::imshow("srcImage", displaySrc);

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
    int randomKernel = random_number;

    cv::Mat kernel = (cv::Mat_<int>(3, 3) << 1, 0, -1,
                      2, 1, -2,
                      1, 0, -1);

    printf("\nKernel value to be used : %d\r\n", randomKernel);
    printf("\n\nsrcImage values\r\n");

    // Just display srcImage
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

    // Traverse srcImage and do stuff
    cv::Mat resultImage = cv::Mat::zeros(srcImage.size(), srcImage.type());

    for (int srcRow = 1; srcRow < srcImage.rows - 1; ++srcRow)
    {
        for (int srcCol = 1; srcCol < srcImage.cols - 1; ++srcCol)
        {
            int sum = 0;
            // apply 3x3 kernel centered at (x, y)

            for (int kRow = -1; kRow <= 1; ++kRow)
            {
                for (int kCol = -1; kCol <= 1; ++kCol)
                {
                    /*
                        To access the correct values in the kernel
                            - That's indexed from [0][0] --> [2][2]
                            - Since we're starting at -1 in the for loops
                                - We must shift the indices by 1 to access the indicies from [0][0] --> [2][2] in the kernel
                    */
                    // Get the value from the kernel (0–2 range)
                    int kernelVal = kernel.at<int>(kRow + 1, kCol + 1);
                    // Get the corresponding value from the 3×3 region centered at (row, col) in the matrix.
                    int matrixVal = srcImage.at<uchar>(srcRow + kRow, srcCol + kCol);
                    sum += kernelVal * matrixVal;
                }
            }
            resultImage.at<uchar>(srcRow, srcCol) = cv::saturate_cast<uchar>(sum);
        }

        std::cout << std::endl;
    }

    for (int i = 0; i < resultImage.rows; ++i)
    {
        for (int j = 0; j < resultImage.cols; ++j)
        {
            // uchar has to be used since we told OpenCV to use unsigned
            // 8-bit integers CV_8UC1 and not regular ints
            std::cout << static_cast<int>(resultImage.at<uchar>(i, j)) << "\t";
        }
        std::cout << std::endl;
    }

    cv::Mat dispalyResult, sideBySide;
    cv::Mat displaySrc;

    // Doesn't guarantee the resized matrix will have the same type as displaySrc so you'll have to use convertTo()
    cv::resize(resultImage, dispalyResult, cv::Size(640, 480), 0, 0, cv::INTER_NEAREST);
    cv::resize(srcImage, displaySrc, cv::Size(640, 480), 0, 0, cv::INTER_NEAREST);

    displaySrc.convertTo(displaySrc, CV_8UC1);
    dispalyResult.convertTo(dispalyResult, CV_8UC1);

    cv::hconcat(displaySrc, dispalyResult, sideBySide);
    cv::imshow("Before | After", sideBySide);
    cv::waitKey(0);

    return 0;
}
