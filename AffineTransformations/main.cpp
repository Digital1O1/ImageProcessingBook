#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // --- Step 1: Create a 10x10 grayscale image with a simple pattern ---
    Mat image(10, 10, CV_8UC1);
    // Fill the image with a pattern so changes are visible.
    // For example, each pixel's value is (currentRow*25 + currentCol*2)
    for (int currentRow = 0; currentRow < image.rows; currentRow++)
    {
        for (int currentCol = 0; currentCol < image.cols; currentCol++)
        {
            image.at<uchar>(currentRow, currentCol) = saturate_cast<uchar>(currentRow * 25 + currentCol * 2);
        }
    }

    // Enlarge the original image for display
    Mat enlargedOriginal;
    resize(image, enlargedOriginal, Size(), 20, 20, INTER_NEAREST);
    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", enlargedOriginal);

    // --- Step 2: Define each transformation as a 3x3 homogeneous matrix ---
    // Note: For warpAffine, we need a 2x3 matrix; we will extract that later.

    // 1. Reflection: Flip horizontally.
    // To reflect about the vertical axis (flip left/right) of an image of width W,
    // the matrix is:
    // [ -1    0   W-1 ]
    // [  0    1    0  ]
    // [  0    0    1  ]
    float W = image.cols;
    Mat reflection = (Mat_<float>(3, 3) << 
                     -1, 0, W - 1,
                      0, 1, 0,
                      0, 0, 1);

    // 2. Scaling: Scale factors Sx and Sy.
    float Sx = 1.5f, Sy = 2.0f;
    Mat scaling = (Mat_<float>(3, 3) << 
                   Sx, 0, 0,
                   0, Sy, 0,
                   0, 0, 1);

    // 3. Shear: Horizontal shear (sh_x) and vertical shear (sh_y).
    float sh_x = 0.5f, sh_y = 0.3f;
    Mat shear = (Mat_<float>(3, 3) << 
                 1, sh_x, 0,
                 sh_y, 1, 0,
                 0, 0, 1);

    // 4. Rotation: Rotate by a given angle (in degrees) about the origin.
    float angle = 30.0f; // degrees
    float rad = angle * CV_PI / 180.0f;
    Mat rotation = (Mat_<float>(3, 3) << cos(rad), -sin(rad), 0,
                    sin(rad), cos(rad), 0,
                    0, 0, 1);

    // 5. Translation: Translate by Tx in x and Ty in y.
    float Tx = 3.0f, Ty = 2.0f;
    Mat translation = (Mat_<float>(3, 3) << 1, 0, Tx,
                       0, 1, Ty,
                       0, 0, 1);

    // --- Step 3: For each transformation, extract the 2x3 affine matrix and apply warpAffine ---
    // We will use the original 10x10 image for each transformation and then enlarge the result for display.

    // Helper lambda to apply a 3x3 transformation and return an enlarged result.
    auto applyAndEnlarge = [&](const Mat &transformation3x3, const string &winName)
    {
        // Extract the 2x3 affine matrix (the top two rows)
        Mat affineMat = transformation3x3(Range(0, 2), Range(0, 3)).clone();
        Mat transformed;
        warpAffine(image, transformed, affineMat, image.size());
        Mat enlarged;
        resize(transformed, enlarged, Size(), 20, 20, INTER_NEAREST);
        namedWindow(winName, WINDOW_AUTOSIZE);
        imshow(winName, enlarged);
    };

    // Apply each transformation and display the result.
    applyAndEnlarge(reflection, "Reflection");
    applyAndEnlarge(scaling, "Scaling");
    applyAndEnlarge(shear, "Shear");
    applyAndEnlarge(rotation, "Rotation");
    applyAndEnlarge(translation, "Translation");

    // --- Step 4: (Optional) Compose a composite transformation ---
    // For demonstration, compose: Translation * Rotation * Shear * Scaling * Reflection
    Mat composite = translation * rotation * shear * scaling * reflection;
    Mat compositeAffine = composite(Range(0, 2), Range(0, 3)).clone();
    Mat compositeTransformed;
    warpAffine(image, compositeTransformed, compositeAffine, image.size());
    Mat enlargedComposite;
    resize(compositeTransformed, enlargedComposite, Size(), 20, 20, INTER_NEAREST);
    namedWindow("Composite Transformation", WINDOW_AUTOSIZE);
    imshow("Composite Transformation", enlargedComposite);

    waitKey(0);
    return 0;
}
