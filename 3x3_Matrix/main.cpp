#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // Create an empty 3x3 grayscale matrix (8-bit unsigned)
    cv::Mat mat(3, 3, CV_8UC1);

    // Fill it with increasing values
    uchar value = 0;
    for (int row = 0; row < mat.rows; ++row)
    {
        for (int col = 0; col < mat.cols; ++col)
        {
            mat.at<uchar>(row, col) = value;
            value += 32; // Choose increment (e.g., 32) to reach toward 255 by 9th value
        }
    }
    cv::resize(mat, mat, cv::Size(640, 480), 0, 0, cv::INTER_AREA);

    cv::imshow("3x3 Matrix 0 --> 255", mat);
    cv::waitKey(0);

    return 0;
}
