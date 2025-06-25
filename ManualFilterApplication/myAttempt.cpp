#pragma once
#include <utility>
#include <vector>
#include <iostream>
/*
🔧 1. Manual Filter Application (From Scratch)

Goal: Implement a spatial filter without using cv::filter2D.

    Tasks:

        Write your own function to apply a kernel (e.g., 3×3 box blur, Sobel, or sharpening filter).

        Manually compute sum-of-products using nested loops.

        Support both correlation and convolution (rotate kernel for convolution).

        Handle borders (ignore, replicate, or zero-padding).

✅ Concepts reinforced:

    Sum-of-products mechanism

    Kernel centering and neighborhood traversal

    Correlation vs. convolution difference
*/
int main()
{

    std::vector<std::vector<int>> targetMatrix = {
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 10, 11, 12},
        {13, 14, 15, 16, 17, 18}};

    std::vector<std::vector<int>> kernel = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
    };

    int targetMatrixRows = targetMatrix.size();
    int targetMatrixCols = targetMatrix[0].size();

    printf("Target matrix rows [ %d ] cols [ %d ] \r\n\n", targetMatrixRows, targetMatrixCols);

    int rowLimit = targetMatrixRows - 2;
    int colLimit = targetMatrixCols - 2;

    printf("Target matrix\r\n");

    for (std::vector<int> &row : targetMatrix)
    {
        for (int col : row)
            printf("%d ", col);
        printf("\r\n");
    }
    printf("-------------------\r\n");

    // Applying the kernel to target matrix
    int sum = 0;
    // Loops through 1 --> 7 inclusively
    for (int idxRowTargetMatrix = 1; idxRowTargetMatrix < targetMatrixRows - 1; ++idxRowTargetMatrix)
    {
        int *tempArray = new int[targetMatrixCols];

        for (int idxColTargetMatrix = 1; idxColTargetMatrix < targetMatrixCols - 1; ++idxColTargetMatrix)
        {
            sum = 0;
            // int * tempArray = new int[targetMatrixCols];
            //  Apply the kernel here
            for (int kernelRowIdx = -1; kernelRowIdx <= 1; ++kernelRowIdx)
            {
                for (int kernelColIdx = -1; kernelColIdx <= 1; ++kernelColIdx)
                {
                    // Perform operations here???
                    // This is what I originally had
                    // targetMatrix[idxRowTargetMatrix][idxColTargetMatrix] = targetMatrix[idxRowTargetMatrix][idxColTargetMatrix] + kernel[kernelRowIdx][kernelColIdx];
                    sum += targetMatrix[idxRowTargetMatrix + kernelRowIdx][idxColTargetMatrix + kernelColIdx];
                    // printf("%d ",tempArray[idxColTargetMatrix]);
                } // Outside the kernal appliaction
            }
            tempArray[idxColTargetMatrix] = sum;
        }
        for (int col = 1; col < targetMatrixCols - 1; ++col)
            targetMatrix[idxRowTargetMatrix][col] = tempArray[col];
        delete[] tempArray;
    }
    printf("\nFiltered targetMatrix:\n");
    for (const std::vector<int> &row : targetMatrix)
    {
        for (int val : row)
        {
            printf("%d ", val);
        }
        printf("\n");
    }

    // printf("\r\n");
    // for (std::vector<int> &row : targetMatrix)
    // {
    //     for (int col : row)
    //         printf("%d ", col);
    //     printf("\r\n");
    // }
}
