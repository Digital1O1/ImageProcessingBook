#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm> // For std::reverse
int main()
{
    std::vector<std::vector<int>> targetMatrix = {
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 10, 11, 12},
        {13, 14, 15, 16, 17, 18}};

    std::vector<std::vector<int>> kernel = {
        {9, 1, 1},
        {1, 0, 1},
        {4, 1, 55},
    };

    int targetMatrixRows = targetMatrix.size();
    int targetMatrixCols = targetMatrix[0].size();

    printf("Target matrix rows [%d] cols [%d]\n\n", targetMatrixRows, targetMatrixCols);

    printf("Original Target Matrix:\n");
    for (std::vector<int> &row : targetMatrix)
    {
        // Include & if you want to change the values
        for (int val : row)
            printf("%d ", val);
        printf("\n");
    }

    // for (std::vector<std::vector<int>> &row : targetMatrix)
    // {
    //     for (std::vector<int> val : row)
    //         printf("%d ", val);
    //     printf("\n");
    // }
    printf("-------------------\n");

    // ✅ Apply zero-padding to the original matrix
    std::vector<std::vector<int>> paddedMatrix(
        targetMatrixRows + 2,
        std::vector<int>(targetMatrixCols + 2, 0));

    for (int i = 0; i < targetMatrixRows; ++i)
        for (int j = 0; j < targetMatrixCols; ++j)
            paddedMatrix[i + 1][j + 1] = targetMatrix[i][j];

    printf("Original Kernel for Convolution:\n");
    for (const auto &row : kernel)
    {
        for (int val : row)
            printf("%d ", val);
        printf("\n");
    }

    // ✅ Flip the kernel for true convolution (instead of correlation)
    std::reverse(kernel.begin(), kernel.end()); // Flip rows vertically
    for (auto &row : kernel)
        std::reverse(row.begin(), row.end()); // Flip each row horizontally

    // Output the flipped kernel for verification
    printf("Flipped Kernel for Convolution:\n");
    for (const auto &row : kernel)
    {
        for (int val : row)
            printf("%d ", val);
        printf("\n");
    }

    // We create a copy of targetMatrix to store results, so original values
    // are not overwritten during calculation
    std::vector<std::vector<int>> resultMatrix = targetMatrix;

    // Apply kernel to each valid pixel location (ignoring borders)
    // The two for loops below don't start at row/col = 0 or row = last since we won't have a 'full' 3x3 neighborhood
    /*
        Why we start at 1 and stop at targetMatrixRows - 1
            - If we had a 6x6 matrix like this
            row index:    0   1   2   3   4   5
                        ---------------------
                        |                     |
                    --> |  6×6 target matrix  |
                        |                     |
                        ---------------------
            - And if we're centered at (0,0) or (0,5)
                - Cant apply the 3x3 kernel since the kernel would try to access out of bound values
                - Example : row -1 or col 6
            - To apply the 3x3 kernel
                - Must be 1 row above/below the center
                - And 1 column to the left/right
            - So we must skip
                - The first row
                - The last row : targetMatrixRows - 1
                - The first column (0)
                - The last column : targetMatrixCols - 1

    */
    for (int row = 1; row < targetMatrixRows - 1; ++row)
    {
        for (int col = 1; col < targetMatrixCols - 1; ++col)
        {
            int sum = 0;
            // Apply the kernel (3x3 window)
            /*
                When applying the kernel
                    - Remember the kernel has relative positions
                        (-1,-1) (-1,0) (-1,1)
                        (0,-1)  (0,0)  (0,1)
                        (1,-1)  (1,0)  (1,1)
                    - So both kRow/kCol go from -1 --> +1 with 3 total iterations

            */
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
                    int kernelVal = kernel[kRow + 1][kCol + 1];
                    // Get the corresponding value from the 3×3 region centered at (row, col) in the matrix.
                    int matrixVal = targetMatrix[row + kRow][col + kCol];
                    sum += kernelVal * matrixVal;
                }
            }
            // Normalize sum by dividing by 9 (sum of kernel elements)
            resultMatrix[row][col] = sum / 9;
        }
    }

    printf("Filtered Target Matrix (3x3 kernel applied with zero-padding):\n");
    for (const auto &row : resultMatrix)
    {
        for (int val : row)
            printf("%d ", val);
        printf("\n");
    }

    return 0;
}
