#include <stdint.h>
#define KERNEL_SIZE_3 3

int8_t ridge_detection_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3] =
  { 0,-1, 0,
   -1, 4,-1,
    0,-1, 0};

int8_t edge_detection_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3] =
  { 1, 1, 1,
    1,-8, 1,
    1, 1, 1};

int8_t sharpen_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3] =
  { 0,-1, 0,
   -1, 5,-1,
    0,-1, 0};

int8_t box_blur_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3] =
  { 1, 1, 1,
    1, 1, 1,
    1, 1, 1};

int8_t gaussian_blur_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3] =
  { 1, 2, 1,
    2, 4, 2,
    1, 2, 1};

int8_t emboss_kernel_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3] = 
{
       -2, -1,  0,
       -1,  1,  1,
        0,  1,  2
};
