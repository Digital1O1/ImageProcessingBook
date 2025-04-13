#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // Create a 3x3 image with black pixels (CV_8UC3 means 8-bit, 3 channels)
    cv::Mat image(3, 3, CV_8UC3, cv::Scalar(0, 0, 0));

    // Change the color of a specific pixel (e.g., top-left) to red
    image.at<cv::Vec3b>(0, 0) = cv::Vec3b(0, 255, 0);
    image.at<cv::Vec3b>(1, 0) = cv::Vec3b(0, 0, 255);
    image.at<cv::Vec3b>(0, 2) = cv::Vec3b(255, 0, 0);

    // Enlarge the image by a scale factor, e.g., 50 times in each direction
    cv::Mat enlargedImage;
    cv::resize(image, enlargedImage, cv::Size(), 100, 100, cv::INTER_NEAREST);
    // Using INTER_NEAREST keeps the blocky appearance, which is often desired for such a small image.

    // Create a resizable window so that the enlarged image is clearly visible
    // cv::namedWindow("Modified Image", cv::WINDOW_NORMAL);
    cv::imshow("Modified Image", enlargedImage);

    cv::waitKey(0);
    return 0;
}
