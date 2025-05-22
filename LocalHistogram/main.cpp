#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

// Reference : https://docs.opencv.org/3.4/d8/dbc/tutorial_histogram_calculation.html

using namespace std;
using namespace cv;

Point start_point, end_point;
bool selecting = false;
Mat image, roi;

// Mouse callback function for ROI selection
void mouse_callback(int event, int x, int y, int flags, void *userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        selecting = true;
        start_point = Point(x, y);
    }
    else if (event == EVENT_MOUSEMOVE && selecting)
    {
        Mat temp_image = image.clone();
        rectangle(temp_image, start_point, Point(x, y), Scalar(255, 0, 0), 2);
        imshow("Select ROI With Mouse", temp_image);
    }
    else if (event == EVENT_LBUTTONUP)
    {
        selecting = false;
        end_point = Point(x, y);

        // Ensure the rectangle is valid and direction-independent
        Rect rect(
            Point(min(start_point.x, end_point.x), min(start_point.y, end_point.y)),
            Point(max(start_point.x, end_point.x), max(start_point.y, end_point.y)));

        // Check that the ROI has non-zero size
        if (rect.width > 0 && rect.height > 0)
        {
            roi = image(rect).clone(); // Clone to preserve independent data
            cv::resize(roi, roi, Size(640, 480), cv::INTER_LANCZOS4);

            imshow("ROI Selected", roi);
        }

        imshow("Select ROI With Mouse", image); // Reset to original
    }
}

int main()
{
    // Load input image
    Mat src = imread("/home/digital101/ImageProcessingBook/PicturesForProcessing/48366471_10156499355345528_8731503225246580736_n.jpg", IMREAD_COLOR);
    if (src.empty())
    {
        cerr << "Error: Image not found!" << endl;
        return EXIT_FAILURE;
    }

    // Resize to consistent dimensions
    resize(src, src, Size(640, 480));
    image = src.clone(); // Set global image for mouse interaction

    // Split image into BGR channels
    vector<Mat> bgr_planes;
    split(src, bgr_planes);

    // Set histogram parameters
    int histSize = 256;
    float binRange[] = {0, 256}; // upper boundary is exclusive
    const float *histRange[] = {binRange};
    bool uniform = true, accumulate = false;

    // Calculate BGR histograms
    Mat b_hist, g_hist, r_hist;

    /*
        Parameters for calcHist()
        - &bgr_planes[0]: The source array(s)
        - 1: The number of source arrays (in this case we are using 1. We can enter here also a list of arrays )
        - 0: The channel (dim) to be measured. In this case it is just the intensity (each array is single-channel) so we just write 0.
        - Mat(): A mask to be used on the source array ( zeros indicating pixels to be ignored ). If not defined it is not used
        - b_hist: The Mat object where the histogram will be stored
        - 1: The histogram dimensionality.
        - histSize: The number of bins per each used dimension
        - histRange: The binRange  of values to be measured per each dimension
        - uniform and accumulate: The bin sizes are the same and the histogram is cleared at the beginning.

    */
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);

    // Create an image to display the histogram
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    // Normalize histograms to fit the image height
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    // Draw histogram lines for each channel
    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8, 0);
    }

    // Show the original image and histogram
    imshow("Source image", src);
    imshow("calcHist Demo", histImage);

    // Prepare the mouse interaction window
    namedWindow("Select ROI With Mouse");
    setMouseCallback("Select ROI With Mouse", mouse_callback);
    imshow("Select ROI With Mouse", image);

    waitKey();
    return EXIT_SUCCESS;
}
