# 🧩 1. Misunderstanding the Filter Operation

### What Went Wrong:
You tried to apply the kernel by just `summing values` from the matrix `without multiplying them` by corresponding kernel elements.

```cpp
sum += targetMatrix[row + kRow][col + kCol]; // Kernel values not used
```

### Concept to Review:

* **Sum-of-products in convolution/correlation**
        + The correct operation is:
        + `sum+=kernel[i][j] × image[x+i][y+j]`
        + `sum+=kernel[i][j] × image[x+i][y+j]`

# 🧩 2. Overwriting the Input Matrix During Processing

### What Went Wrong:
You stored the filtered values back into the same `targetMatrix`, which corrupted future calculations (since convolution requires original values in a 3×3 neighborhood).

### Concept to Review:

* **Why output must be stored separately**
        + If you overwrite pixels too early, neighboring computations use already-modified values.
* **Recommended Approach:**
        + Store filtered values in a separate matrix.

🧩 3. Confusion About the Kernel's Purpose

### What Went Wrong:
You said: "I just wanted to add the 3×3 matrix to the 9×9 matrix", but that’s not how filtering works. You’re not adding the kernel, you're using it as weights
for nearby pixel intensities.

### Concept to Review:

* **Filtering vs addition**
        + Kernel is not added to the matrix directly — it’s applied over each 3×3 patch of the image using sum-of-products.
* **Understanding Filtering Operations**

🧩 4. Not Normalizing the Result

### What Went Wrong:
You applied a kernel of all 1s (i.e., a box blur), but didn’t divide by the sum of the kernel (9), so the output values were too large.

### Concept to Review:

* **Normalization in filtering**
        + A box blur averages 3×3 values ⇒ divide by 9.

🧩 5. Indexing Errors and Offsets

### What Went Wrong:
Your kernel indexing didn’t clearly offset the kernel by +1 to align with its 0-based index. This could lead to accessing out-of-bounds values or wrong parts of
the kernel.

### Concept to Review:

* **Kernel index mapping**
        + When looping k = -1 to 1, kernel index is k + 1 to map into [0, 2].

🧩 6. Dynamic Allocation for Each Row (Memory Overhead)

### What Went Wrong:
You used `new int[]` for tempArray inside the outer loop, and then copied results back. This works, but is over-complicated and less safe in modern C++.

### Concept to Review:

* **Use of std::vector over raw pointers**
        + Prefer `std::vector<std::vector<int>> resultMatrix` or use one fixed-size output matrix.
* **Recommended Approach:**
        + Use `std::vector` for dynamic allocation.

**Summary**

| Topic                        | Why It's Important                              |
| ---------------------------- | ----------------------------------------------- |
| Sum-of-products in filtering | Core operation for convolution/correlation      |
| Convolution vs Correlation   | Understand kernel flipping and application      |
| Temporary output buffers     | Prevent modifying data that’s still needed      |
| Normalizing filters          | Avoid overflow and get correct intensity ranges |
| Index management             | Prevent out-of-bounds and apply kernel properly |
| std::vector vs new           | Avoid memory leaks and make safer code          |

---

# Visualization for kernel application 

```bash
# Target matrix 6x6

[  1   2   3   4   5   6 ]
[  7   8   9  10  11  12 ]
[ 13  14  15  16  17  18 ]
[ 19  20  21  22  23  24 ]
[ 25  26  27  28  29  30 ]
[ 31  32  33  34  35  36 ]
```

## Applying 3x3 kernel centered at position `row = 2` `col = 3`
- The matrix value `16` is in the `3rd row` `4th column` (0 indexed)
- Centered at `[row = 2]` `[col = 3]` = 16
  - Example : We want to apply the 3x3 kernel around the center of `row 2` `col 3`
    - The kernel has the following shape and indexing relative to the center of the kernel itself 
      ```bash
        [(-1,-1) (-1,0) (-1,+1)]
        [( 0,-1) ( 0,0) ( 0,+1)]
        [(+1,-1) (+1,0) (+1,+1)]
      ```
      - When iterating over the kernal like this, we're stating
        - For every offset around the center, go from -1 to +1 in both row and column directions.
      ```cpp
        for (int kRow = -1; kRow <= 1; ++kRow)
            for (int kCol = -1; kCol <= 1; ++kCol)
      ```
    - Applying the offsets really means 
      - Since we're at `row 2` and `col 3`
      - The `offsets` are the variables `kRow` and `kCol` in the for loops above
        - They just state how far they move from the center pixel as the kernel slides over the image
### Going back to the 3x3 window that's centered at (2,3)
- Remember indexes are `zero-based`
- To apply the kernel, need to extract 3x3 region around it
- At (2,3) we're looking at 
  ```bash
    [  1   2   3   4   5   6 ]
    [  7   8  [9][10][11] 12 ]
    [ 13  14 [15][16][17] 18 ]
    [ 19  20 [21][22][23] 24 ]
    [ 25  26  27  28  29  30 ]
    [ 31  32  33  34  35  36 ]

            ^ 
        center of kernel at (2,3) = 16
  ```
  - Assume the kernel is a box blur 
    ```bash
    [1 1 1]
    [1 1 1]
    [1 1 1]
    ```
   - So to calculate the box blur we do 
    ```bash
    sum = 9 + 10 + 11 +
        15 + 16 + 17 +
        21 + 22 + 23 = 144

    avg = sum / 9 = 144 / 9 = 16 # <---- This would replace 16 in resultMatrix[2][3]
    ```
