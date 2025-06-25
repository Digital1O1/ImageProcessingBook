#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Point start_point, end_point;
bool selecting = false;
Mat image, roi;
cv::Size tilGridSize(8, 8); // Size of tiles for clahe
float clipLimit = 2.0;      // Threshold for contrast limiting
                            // Pixels that exceed this limit are clipped/redistributed 
                            // Default value is at 40

cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clipLimit, tilGridSize);
cv::Mat equalized_image;

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

        Rect rect(
            Point(min(start_point.x, end_point.x), min(start_point.y, end_point.y)),
            Point(max(start_point.x, end_point.x), max(start_point.y, end_point.y)));

        if (rect.width > 0 && rect.height > 0)
        {
            roi = image(rect).clone();
            resize(roi, roi, Size(640, 480), INTER_LANCZOS4);

            // RGB CLAHE
            vector<Mat> CLAHE_channels;
            cv::Mat equalized_roi_bgr;
            split(roi, CLAHE_channels);
            for (int i = 0; i < 3; ++i)
                clahe->apply(CLAHE_channels[i], CLAHE_channels[i]);

            merge(CLAHE_channels, equalized_roi_bgr);
            // Sanity check
            // cv::imshow("CLAHE_channels1", CLAHE_channels[0]);
            // cv::imshow("CLAHE_channels2", CLAHE_channels[1]);
            // cv::imshow("CLAHE_channels3", CLAHE_channels[2]);

            // Compute and show CLAHE histogram

            int histSize = 256; // Define number of bins in the histogram
            float range[] = {0, 256};
            const float *histRange[] = {range};          // 'wraps' the range array in the format OpenCV expects
                                                         // In the form of const float* ranges[]
            int bin_w = cvRound((double)512 / histSize); // Draws the histogram image
                                                         // Draws the histogram into an image that's 512 pixels wide
                                                         // You want to compute the width of each bin in pixels so you can draw it
                                                         // In this line, each bin will occupy 2 pixels horizontally in the histogram image
                                                         // Meaning the histogram will be 512 pixels wide with 256 bins drawn as 2-pixel wide bars
                                                         // In short, this line sets the width of each bar whendrawing the histogram image

            cv::Mat bChannelHist, gChannelHist, rChannelHist;

            /*
                         Parameters for calcHist()
                         - &bgr_planes[0]: The source array(s)
                         - 1: The number of source arrays (in this case we are using 1. We can enter here also a list of arrays )
                         - 0: The channel (dimensions) to be measured. In this case it is just the intensity (each array is single-channel) so we just write 0.
                         - Mat(): A mask to be used on the source array ( zeros indicating pixels to be ignored ). If not defined it is not used
                         - b_hist: The Mat object where the histogram will be stored
                         - 1: The histogram dimensionality.
                         - histSize: The number of bins per each used dimension
                         - histRange: The binRange  of values to be measured per each dimension
                         - uniform and accumulate: The bin sizes are the same and the histogram is cleared at the beginning.

            */

            // Calculate histogram for BGR channels
            calcHist(&CLAHE_channels[0], 1, 0, Mat(), bChannelHist, 1, &histSize, &histRange[0], true, false);
            calcHist(&CLAHE_channels[1], 1, 0, Mat(), gChannelHist, 1, &histSize, &histRange[0], true, false);
            calcHist(&CLAHE_channels[2], 1, 0, Mat(), rChannelHist, 1, &histSize, &histRange[0], true, false);

            // The 'window' that's suppose to display everything?
            cv::Mat claheImg(400, 512, CV_8UC3, Scalar(0, 0, 0));

            // Normalize
            normalize(bChannelHist, bChannelHist, 0, claheImg.rows, NORM_MINMAX);
            normalize(gChannelHist, gChannelHist, 0, claheImg.rows, NORM_MINMAX);
            normalize(rChannelHist, rChannelHist, 0, claheImg.rows, NORM_MINMAX);

            // This iterates throughotu the entire 256 values
            for (int i = 1; i < histSize; i++)
            {

                /*
                            cv::line() parameters
                                img: The image on which the line will be drawn. It is a cv::Mat object.
                                pt1: The first endpoint of the line, specified as a cv::Point object.
                                pt2: The second endpoint of the line, specified as a cv::Point object.
                                color: The color of the line, specified as a cv::Scalar object. For example, Scalar(255, 0, 0) represents blue, Scalar(0, 255, 0) represents green, and Scalar(0, 0, 255) represents red.
                                thickness: The thickness of the line in pixels. A thickness of 1 is a single-pixel wide line.
                                lineType: The type of line. Common options include cv::LINE_8 (8-connected line), cv::LINE_4 (4-connected line), and cv::LINE_AA (antialiased line).
                                shift: The number of fractional bits in the point coordinates.
                 */
                line(claheImg, Point(bin_w * (i - 1), 400 - cvRound(bChannelHist.at<float>(i - 1))),
                     Point(bin_w * (i), 400 - cvRound(bChannelHist.at<float>(i))),
                     Scalar(255, 0, 0), 2, 8, 0);
                line(claheImg, Point(bin_w * (i - 1), 400 - cvRound(gChannelHist.at<float>(i - 1))),
                     Point(bin_w * (i), 400 - cvRound(gChannelHist.at<float>(i))),
                     Scalar(0, 255, 0), 2, 8, 0);
                line(claheImg, Point(bin_w * (i - 1), 400 - cvRound(rChannelHist.at<float>(i - 1))),
                     Point(bin_w * (i), 400 - cvRound(rChannelHist.at<float>(i))),
                     Scalar(0, 0, 255), 2, 8, 0);
            }

            // Convert ROI to grayscale and apply CLAHE
            // Mat roiGray;
            // cvtColor(roi, roiGray, COLOR_BGR2GRAY);
            // clahe->apply(roiGray, equalized_image);

            // Compute and show CLAHE histogram
            // Mat grayHist;
            // int histSize = 256;
            // float range[] = {0, 256};

            // const float *histRange[] = {range};
            // calcHist(&equalized_image, 1, 0, Mat(), grayHist, 1, &histSize, &histRange[0], true, false);
            // normalize(grayHist, grayHist, 0, 400, NORM_MINMAX);
            // // Mat (row,column,data type, scalar)
            // Mat grayHistImg(400, 512, CV_8UC3, Scalar(0, 0, 0));
            // int bin_w = cvRound((double)512 / histSize);
            // for (int i = 1; i < histSize; i++)
            // {
            //     line(grayHistImg, Point(bin_w * (i - 1), 400 - cvRound(grayHist.at<float>(i - 1))),
            //          Point(bin_w * (i), 400 - cvRound(grayHist.at<float>(i))),
            //          Scalar(200, 200, 200), 2, 8, 0);
            // }

            // Compute BGR ROI histogram
            vector<Mat> roi_planes;
            split(roi, roi_planes);

            Mat roi_b_hist, roi_g_hist, roi_r_hist;

            /*
                   Parameters for calcHist()
                   - &bgr_planes[0]: The source array(s)
                   - 1: The number of source arrays (in this case we are using 1. We can enter here also a list of arrays )
                   - 0: The channel (dimensions) to be measured. In this case it is just the intensity (each array is single-channel) so we just write 0.
                   - Mat(): A mask to be used on the source array ( zeros indicating pixels to be ignored ). If not defined it is not used
                   - b_hist: The Mat object where the histogram will be stored
                   - 1: The histogram dimensionality.
                   - histSize: The number of bins per each used dimension
                   - histRange: The binRange  of values to be measured per each dimension
                   - uniform and accumulate: The bin sizes are the same and the histogram is cleared at the beginning.

               */

            /*
                 void cv::normalize(
                 cv::InputArray src,
                 cv::OutputArray dst,
                 double alpha = 1,
                 double beta = 0,
                 int norm_type = cv::NORM_L2,
                 int dtype = -1,
                 cv::InputArray mask = cv::noArray()
                 );

                 Here's a breakdown of the parameters:
                     - src: The input array (image or matrix).
                     - dst: The output array (normalized image or matrix).
                     - alpha: The lower range boundary for the normalization.
                     - beta: The upper range boundary for the normalization.
                     - norm_type: The type of normalization to perform. It can be one of the following:
                         - cv::NORM_L1: Normalizes the array so that the sum of its absolute values is equal to alpha.
                         - cv::NORM_L2: Normalizes the array so that the Euclidean norm (L2 norm) is equal to alpha.
                         - cv::NORM_INF: Normalizes the array so that the maximum absolute value is equal to alpha.
                         - cv::NORM_MINMAX: Normalizes the array so that the minimum value is equal to alpha and the maximum value is equal to beta.
                     - dtype: The desired data type of the output array. If set to -1, the output array will have the same data type as the input array.
                     - mask: An optional mask that specifies which elements of the input array to normalize.
             */

            // normalize(roi_b_hist, roi_b_hist, 0, roiHistImage.rows, NORM_MINMAX);
            // normalize(roi_g_hist, roi_g_hist, 0, roiHistImage.rows, NORM_MINMAX);
            // normalize(roi_r_hist, roi_r_hist, 0, roiHistImage.rows, NORM_MINMAX);

            // for (int i = 1; i < histSize; i++)
            // {
            /*
                cv::line() parameters
                    img: The image on which the line will be drawn. It is a cv::Mat object.
                    pt1: The first endpoint of the line, specified as a cv::Point object.
                    pt2: The second endpoint of the line, specified as a cv::Point object.
                    color: The color of the line, specified as a cv::Scalar object. For example, Scalar(255, 0, 0) represents blue, Scalar(0, 255, 0) represents green, and Scalar(0, 0, 255) represents red.
                    thickness: The thickness of the line in pixels. A thickness of 1 is a single-pixel wide line.
                    lineType: The type of line. Common options include cv::LINE_8 (8-connected line), cv::LINE_4 (4-connected line), and cv::LINE_AA (antialiased line).
                    shift: The number of fractional bits in the point coordinates.
             */

            // line(roiHistImage, Point(bin_w * (i - 1), 400 - cvRound(roi_b_hist.at<float>(i - 1))),
            //      Point(bin_w * (i), 400 - cvRound(roi_b_hist.at<float>(i))),
            //      Scalar(255, 0, 0), 2, 8, 0);
            // line(roiHistImage, Point(bin_w * (i - 1), 400 - cvRound(roi_g_hist.at<float>(i - 1))),
            //      Point(bin_w * (i), 400 - cvRound(roi_g_hist.at<float>(i))),
            //      Scalar(0, 255, 0), 2, 8, 0);
            // line(roiHistImage, Point(bin_w * (i - 1), 400 - cvRound(roi_r_hist.at<float>(i - 1))),
            //      Point(bin_w * (i), 400 - cvRound(roi_r_hist.at<float>(i))),
            //      Scalar(0, 0, 255), 2, 8, 0);
            //  }

            // imshow("ROI Histogram", roiHistImage);
            //  imshow("CLAHE Grayscale Histogram", grayHistImg);
            // imshow("ROI Selected", roi);
            // imshow("CLAHE Equalized ROI", equalized_image);
            // imshow("CLAHE Equalized BGR ROI", equalized_roi_bgr);
            cv::imshow("equalized_roi_bgr", equalized_roi_bgr);
            cv::imshow("claheImg", claheImg);
        }

        imshow("Select ROI With Mouse", image);
    }
}

int main()
{
    std::string imagePath = "/home/digital101/ImageProcessingBook/PicturesForProcessing/DSC_0017.jpeg";
    Mat src = imread(imagePath, IMREAD_COLOR);
    if (src.empty())
    {
        cerr << "Error: Image not found!" << endl;
        return EXIT_FAILURE;
    }

    resize(src, src, Size(640, 480));
    image = src.clone();

    vector<Mat> bgr_planes;
    split(src, bgr_planes);

    int histSize = 256;
    float binRange[] = {0, 256};
    const float *histRange[] = {binRange};
    bool uniform = true, accumulate = false;

    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);

    int hist_wide = 512, hist_height = 400;
    int bin_w = cvRound((double)hist_wide / histSize);
    Mat histImage(hist_height, hist_wide, CV_8UC3, Scalar(0, 0, 0));

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX);
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX);
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX);

    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_height - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_height - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_height - cvRound(g_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_height - cvRound(g_hist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_height - cvRound(r_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_height - cvRound(r_hist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8, 0);
    }

    // imshow("Input Image", image);
    imshow("Full Image Histogram", histImage);
    namedWindow("Select ROI With Mouse", WINDOW_AUTOSIZE);
    setMouseCallback("Select ROI With Mouse", mouse_callback, NULL);
    imshow("Select ROI With Mouse", image);

    waitKey(0);
    return 0;
}
