# What's `loopupTable.ptr()` is doing?
- Remember, openCV Mat objects store image data in a `contiguous memory block`
- The function 
    ```cpp 
    uchar* lut = lookupTable.ptr(); 
    ```
    - Retrivies a pointer called `lut` to the `first element of the memory block 
    - Thus allowing direct acces to the pixel intensity mapping 
- Instead of modifying pixels `one by one` using : `Mat.at<uchar>`
  - We're `precomputing` all the possible transformations (0 to 255)
  - Then storing them in a LUT
  - Thus making gamma correction super fast
    - Since we're directly accessing the memory 

# Why is `lut` a `uchar*`
- lookupTable is a single-row Mat of type CV_8U (unsigned 8-bit integer).
- Since pixel values range from 0-255, 
  - Each entry in the LUT corresponds to a pixel intensity.

- The uchar (unsigned char) type ensures that values are stored efficiently as 0-255 instead of larger data types like int or float.
  - Since uchars are 8 bits 
    - Every pixel is one byte 
    - With values that range from 0 - 255
  - Ints are 32 bits
    - And take 4 bytes per pixel 
    - 4 times more memory than uchar
  - Floats are also 32 bits
    - And also take up 4 bytes per pixel like ints
  - So using either `ints` or `floats` would unnecessarily increase memory usage by 4x
  - OpenCV is also optimized for `uchar` because:
    - SIMD optimization 
      - Modern CPUs handle 8-bit integer math faster than float point operations 
    - Better Cache utlization
      - uchars fit better in the CPU cache 
      - Thus reducing cahce misses and also speeds up operations