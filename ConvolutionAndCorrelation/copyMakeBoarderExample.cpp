#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;
// Reading reference link : https://docs.opencv.org/3.4/dc/da3/tutorial_copyMakeBorder.html
/*
    In this tutorial, briefly going over two ways to define extra padding (border) for an image

    BORDER_CONSTANT: Pad the image with a constant value (i.e. black or 0
    BORDER_REPLICATE: The row or column at the very edge of the original is replicated to the extra border.

    What the program does
        - Load an image
        - Let the user pick what kind of padding to use in the image
        - Two options
            - Constant value border
                - Applies padding of a constant value for the whole border
                - The value will be updated randomly every 0.5 seconds
            - Replicated boarder
                - The border will be replicated from the pixel values at the edges of the original image
*/
// Declare the variables
Mat src, dst;
int top = 0, bottom = 0, leftPadding = 0, rightPadding = 0;
int borderType = BORDER_CONSTANT;
const char *window_name = "copyMakeBorder Demo";
RNG rng(12345);
int main(int argc, char **argv)
{
    // terinary operator : condition ? expression1 : expression2
    // const char *imageName = argc >= 2 ? argv[1] : "Lena_1024.png";
    //  Loads an image
    // src = imread(samples::findFile(imageName), IMREAD_COLOR); // Load an image
    Mat src = imread(samples::findFile("Lena_1024.png"), IMREAD_COLOR);
    // Check if image is loaded fine
    if (src.empty())
    {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default lena.jpg] \n");
        return -1;
    }
    cv::resize(src, src, cv::Size(640, 480));
    // Brief how-to for this program
    printf("\n \t copyMakeBorder Demo: \n");
    printf("\t -------------------- \n");
    printf(" ** Press 'c' to set the border to a random constant value \n");
    printf(" ** Press 'r' to set the border to be replicated \n");
    printf(" ** Press 'ESC' to exit the program \n");
    namedWindow(window_name, WINDOW_AUTOSIZE);

    // Initialize arguments for the filter
    // Give them a value of 5% of the size of SRC
    top = (int)(0.05 * src.rows);
    bottom = top;
    leftPadding = (int)(0.05 * src.cols);
    rightPadding = leftPadding;

    printf("Top padding : [ %d ] \t bottom padding [ %d ] \t left padding : [ %d ] \t right padding : [ %d ] \r\n", top, bottom, leftPadding, rightPadding);
    for (;;)
    {
        // Randomly changes border color
        Scalar value(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        // void cv::copyMakeBorder(cv::InputArray src, cv::OutputArray dst, int top, int bottom, int left, int right, int borderType, const cv::Scalar &value = cv::Scalar())
        copyMakeBorder(src, dst, top, bottom, leftPadding, rightPadding, borderType, value);
        imshow(window_name, dst);
        char c = (char)waitKey(500);
        if (c == 27)
        {
            break;
        }
        else if (c == 'c')
        {
            borderType = BORDER_CONSTANT;
        }
        else if (c == 'r')
        {
            borderType = BORDER_REPLICATE;
        }
    }
    return 0;
}