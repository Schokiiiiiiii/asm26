#ifndef __KERNELS_H__
#define __KERNELS_H__

#include <stdint.h>

#define KERNEL_SIZE_3 3
extern int8_t ridge_detection_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3];
extern int8_t edge_detection_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3];
extern int8_t sharpen_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3];
extern int8_t emboss_kernel_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3];
#define BOX_BLUR_FACTOR 9
extern int8_t *box_blur_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3];
#define GAUSSIAN_BLUR_FACTOR 16
extern int8_t *gaussian_blur_3x3[KERNEL_SIZE_3 * KERNEL_SIZE_3];

#endif /* __KERNELS_H__ */
