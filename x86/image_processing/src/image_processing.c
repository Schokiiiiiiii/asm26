/*
 * File      : image_processing.c
 * Author    : Rick Wertenbroek
 * E-mail    : rick.wertenbroek@heig-vd.ch
 * Institute : REDS - HES-SO HEIG-VD
 * Year      : 2017
 *
 * Coding Style : Linux Kernel - but with 4 space tabs
 * https://www.kernel.org/doc/Documentation/CodingStyle
 *
 * ~ 80 Characters width
 */

#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "image_processing.h"
#include "kernels.h"

/* Single-file public domain librairies for C/C++
   https://github.com/nothings/stb */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct {
    int width;
    int height;
    int comp; /* Number of components per pixel, eg RGB => 3 */

    uint8_t *data;
} image_container;

/*************************
 * Function declarations *
 *************************/
void print_usage();
image_container *allocate_container(size_t width,
                                    size_t height,
                                    size_t comp);
image_container *load_image(const char *src_img_path);
image_container *grayscale_conversion(image_container *img);
image_container *apply_filter(image_container *img);
image_container *apply_filter_student(image_container *img);
void save_image(const char *dest_img_path, const image_container *img);
void free_container(image_container *img);
char show_differences(image_container *imgA,
                      image_container *imgB,
		      bool list);

/* Filters */
uint8_t median_filter_x_y(image_container *img, int32_t x, int32_t y);
uint8_t conv_filter_x_y(image_container *img, int32_t x, int32_t y);
uint8_t emboss_filter_x_y(image_container *img, int32_t x, int32_t y);
uint8_t ridge_filter_x_y(image_container *img, int32_t x, int32_t y);

//#define FILTER_TO_USE median_filter_x_y
#define FILTER_TO_USE ridge_filter_x_y
#define KERNEL_3X3_TO_USE ridge_detection_3x3
uint8_t (*filter_x_y) (image_container *img,
		       int32_t x, int32_t y) = FILTER_TO_USE;

/* External Assembly Function declaration */
extern void asm_filter(uint8_t *src, uint8_t *dest,
                       int32_t width, int32_t height,
                       int32_t x, int32_t y);

/********
 * MAIN *
 ********/
int main(int argc, char **argv)
{
  image_container *img, *img_grayscale, *img_result, *img_result_student;
    char cmd[CMD_SIZE];
    bool show_error = false;
    char *image_path = IMAGE_FILE;
    int option;

    /* Option handling */
    while ((option = getopt(argc, argv,"f:s")) != -1) {
        switch (option) {
	    case 'f' : image_path = optarg;
               break;
	    case 's' : show_error = true;
                break;
            default: print_usage();
                exit(EXIT_FAILURE);
        }
    }

    /* Load image */
    img = load_image(image_path);

    /* Check if grayscale */
    if (img->comp != COMPONENT_GRAYSCALE) {
        img_grayscale = grayscale_conversion(img);
    } else {
        img_grayscale = img;
    }

    /* Display original image */
    /*sprintf(cmd, "display %s &", image_path);*/
    /*system(cmd);*/

    /* Do the filtering */
    img_result = apply_filter(img_grayscale);
    img_result_student = apply_filter_student(img_grayscale);

    /* Save the results */
    save_image(RESULT_FILE, img_result);
    save_image(STUDENT_FILE, img_result_student);

    /* Show differences if any */
    if (show_differences(img_result, img_result_student, show_error) == DIFFERENT) {
        printf("\n"
               "---------------------------------------\n"
               "The result is different than expected !\n"
               "---------------------------------------\n");
        sprintf(cmd, "compare %s %s %s", RESULT_FILE, STUDENT_FILE, DIFF_FILE);
        system(cmd);
        sprintf(cmd, "display %s &", DIFF_FILE);
        system(cmd);
    }
    else {
        printf("\n"
               "-------------------------------------\n"
               "Good job ! The result is as expected.\n"
               "-------------------------------------\n");
    }

    /* Display student's result */
    sprintf(cmd, "display %s &", STUDENT_FILE);
    system(cmd);

    /* Free the containers */
    free_container(img);
    if (img->comp != COMPONENT_GRAYSCALE) {
      free_container(img_grayscale);
    }
    free_container(img_result);
    free_container(img_result_student);

    /* Exit */
    return EXIT_SUCCESS;
}

/************************
 * Function definitions *
 ************************/

/* Prints the usage message */
void print_usage()
{
  printf("Usage : image_processing [-f] filename [-s]\n");
  printf("-f specify the image file to be processed\n");
  printf("-s shows the differences between student's result and expected result in stdout\n");
}

/* Allocates an image container and space for the image data */
image_container *allocate_container(size_t width, size_t height, size_t comp)
{
    if (comp == 0 || comp > COMPONENT_RGBA)
        return NULL;

    image_container *img;

    /* Allocate the struct */
    img = malloc(sizeof (image_container));
    if (!img) {
        fprintf(stderr, "[%s] allocation error\n", __func__);
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    img->width = width;
    img->height = height;
    img->comp = comp;

    /* Allocate space for image */
    img->data = calloc(img->width * img->height * img->comp, sizeof (uint8_t));
    if (!(img->data)) {
        fprintf(stderr, "[%s] image allocation error\n", __func__);
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return img;
}

/* Function that takes a path to an image file and loads
   the image in memory. */
image_container *load_image(const char *src_img_path)
{
    FILE *fimg;
    image_container *img;

    /* Open the image file */
    fimg = fopen(src_img_path, "rb"); /* Read Binary */
    if (!fimg) {
        fprintf(stderr, "[%s] fopen error (%s)\n", __func__, src_img_path);
        exit(EXIT_FAILURE);
    }

    /* Allocate the struct */
    img = malloc(sizeof (image_container));
    if (!img) {
        fprintf(stderr, "[%s] struct allocation error\n", __func__);
        exit(EXIT_FAILURE);
    }

    /* Load the image in the struct */
    img->data = stbi_load(src_img_path, &(img->width), &(img->height),
                          &(img->comp), 0);
    if (!(img->data)) {
        fprintf(stderr, "[%s] stb load image failed\n", __func__);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "[%s] image %s loaded (%d components, %dx%d)\n", __func__,
            src_img_path, img->comp, img->width, img->height);
    return img;
}

/* Creates a grayscale version of the image */
image_container *grayscale_conversion(image_container *img)
{
    if (img->comp != COMPONENT_RGB && img->comp != COMPONENT_RGBA) {
        fprintf(stderr, "[%s] only accepts color images with RGB components\n",
                __func__);
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    image_container *grayscale = allocate_container(img->width,
						    img->height,
						    COMPONENT_GRAYSCALE);

    size_t index = 0;
    int i, j;

    for (i = 0; i < img->height; ++i) {
        for (j = 0; j < img->width; ++j) {

            grayscale->data[i * img->width + j] =
                LUMINOSITY_R * img->data[index + R_OFFSET] +
                LUMINOSITY_G * img->data[index + G_OFFSET] +
                LUMINOSITY_B * img->data[index + B_OFFSET];

            index += img->comp;
        }
    }

    return grayscale;
}

/* 8bit Comparison function from pointers */
int compare_uint8_t(const void * a, const void * b)
{
    /* This is just like an x86 cmp */
    return (*(uint8_t*)a - *(uint8_t*)b);
}

static uint8_t _conv_filter_x_y(image_container *img, int8_t* kernel_3x3, int32_t x, int32_t y)
{
    /* If we are on the edges we keep the pixel as is */
    if ((x - KERNEL_SIZE_3/2) < 0 ||
        (y - KERNEL_SIZE_3/2) < 0 ||
        (x + KERNEL_SIZE_3/2) >= img->width ||
        (y + KERNEL_SIZE_3/2) >= img->height) {
        return img->data[y * img->width + x];
    }
    /* Else we apply the filter */
    else {
        int32_t result = 0;
	for(int j = -(KERNEL_SIZE_3/2); j <= (KERNEL_SIZE_3/2); ++j) {
	    for (int i = -(KERNEL_SIZE_3/2); i <= (KERNEL_SIZE_3/2); ++i) {
	        int32_t pixel = img->data[(y - j) * img->width + (x - i)];
		int32_t coeff = kernel_3x3[(KERNEL_SIZE/2 + j) * KERNEL_SIZE + (KERNEL_SIZE/2) + i];
		result += pixel * coeff;
	    }
	}

        result = abs(result);
	if (result > UCHAR_MAX) {
            result = UCHAR_MAX;
	}
	return result;
    }
}

uint8_t conv_filter_x_y(image_container *img, int32_t x, int32_t y)
{
    _conv_filter_x_y(img, KERNEL_3X3_TO_USE, x, y);
}

/* Apply a median filter to pixels at position x,y */
uint8_t median_filter_x_y(image_container *img, int32_t x, int32_t y)
{
    /* If we are on the edges we keep the pixel as is */
    if ((x - KERNEL_SIZE/2) < 0 ||
        (y - KERNEL_SIZE/2) < 0 ||
        (x + KERNEL_SIZE/2) >= img->width ||
        (y + KERNEL_SIZE/2) >= img->height) {
        return img->data[y * img->width + x];
    }
    /* Else we apply the filter */
    else {
        uint8_t array[5];

        /* Get the pixel and direct neighbors */
        /*
         *         N
         *        NPN
         *         N
         *
         * P : Pixel
         * N : Neighbor
         */

        /* P : [x,y] */
        array[0] = img->data[y * img->width + x];
        /* N : [x+1,y], [x-1,y], [x,y+1], [x,y-1] */
        array[1] = img->data[y * img->width + x + 1];
        array[2] = img->data[y * img->width + x - 1];
        array[3] = img->data[(y + 1) * img->width + x];
        array[4] = img->data[(y - 1) * img->width + x];

        /* Sort them */
        qsort (array, 5, sizeof(uint8_t), compare_uint8_t);

        /* Return the median (center value of the array) */
        /* [--M--] */
        return array[2];
    }
}


uint8_t _emboss_filter_x_y(image_container *img, int8_t* kernel_3x3, int32_t x, int32_t y) {
    int32_t sum = 128;
    int32_t i, j;

    
    for (j = -1; j <= 1; j++) {
        for (i = -1; i <= 1; i++) {
            int32_t nx = x + i;
            int32_t ny = y + j;
            
            
            if (nx >= 0 && nx < img->width && ny >= 0 && ny < img->height) {
                int kernel_index = (j + 1) * KERNEL_SIZE_3 + (i + 1);
                sum += img->data[ny * img->width + nx] * kernel_3x3[kernel_index];
            } else {
                return img->data[y * img->width + x];
            }
        }
    }
    
    
    if (sum < 0) sum = 0;
    if (sum > 255) sum = 255;
    
    return (uint8_t)sum;
}

uint8_t _ridge_filter_x_y(image_container *img, int8_t* kernel_3x3, int32_t x, int32_t y) {
    int32_t sum = 0;
    int32_t i, j;


    for (j = -1; j <= 1; j++) {
        for (i = -1; i <= 1; i++) {
            int32_t nx = x + i;
            int32_t ny = y + j;


            if (nx >= 0 && nx < img->width && ny >= 0 && ny < img->height) {
                int kernel_index = (j + 1) * KERNEL_SIZE_3 + (i + 1);
                sum += img->data[ny * img->width + nx] * kernel_3x3[kernel_index];
            } else {
                return img->data[y * img->width + x];
            }
        }
    }


    if (sum < 0) sum = -sum;
    if (sum > 255) sum = 255;

    return (uint8_t)sum;
}

uint8_t emboss_filter_x_y(image_container *img, int32_t x, int32_t y)
{
    return _emboss_filter_x_y(img, KERNEL_3X3_TO_USE, x, y);
}

uint8_t ridge_filter_x_y(image_container *img, int32_t x, int32_t y)
{
    return _ridge_filter_x_y(img, KERNEL_3X3_TO_USE, x, y);
}

/* Apply a filter to a grayscale image */
image_container *apply_filter(image_container *img)
{
    /* Only works for grayscale images */
    if (img->comp != COMPONENT_GRAYSCALE) {
        return NULL;
    }

    image_container *processed_img = allocate_container(img->width,
                                                        img->height,
                                                        img->comp);

    /* For each pixel apply the filter */
    int32_t x, y;
    for (y = 0; y < img->height; ++y) {
        for (x = 0; x < img->width; ++x) {

            processed_img->data[y * img->width + x] = filter_x_y(img, x, y);

        }
    }

    return processed_img;
}

/* Wrapper function to call the student's assembly code */
image_container *apply_filter_student(image_container *img)
{
    image_container *student_filtered_image = allocate_container(img->width,
                                                                 img->height,
                                                                 COMPONENT_GRAYSCALE);

    /* For each pixel apply the filter */
    int32_t x, y;
    for (y = 0; y < img->height; ++y) {
        for (x = 0; x < img->width; ++x) {
            /* Call student's assembly code */
            asm_filter(img->data, student_filtered_image->data,
                       img->width, img->height,
                       x, y);
        }
    }

    return student_filtered_image;
}

/* Save the processed image to disk */
void save_image(const char *dest_img_path, const image_container *img)
{
    if (!stbi_write_png(dest_img_path,
                        img->width,
                        img->height,
                        img->comp,
                        img->data,
                        PNG_STRIDE_IN_BYTES)) {
        fprintf(stderr, "[%s] save image failed\n", __func__);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "[%s] PNG file %s saved (%dx%d)\n", __func__, dest_img_path,
            img->width, img->height);
}

/* Release the memory used by img */
void free_container(image_container *img)
{
    if (img) {
        if (img->data) {
            stbi_image_free(img->data);
        }
        free(img);
    }
}

/* Shows the differences between imgA and imgB
 * returns SAME if both images are the same
 * The list option prints the differences */
char show_differences(image_container *imgA, image_container *imgB, bool list)
{
    if (imgA->height != imgB->height) {
        fprintf(stderr, "[%s] Images not the same height!\n", __func__);
        return DIFFERENT;
    }
    if (imgA->width != imgB->width) {
        fprintf(stderr, "[%s] Images not the same width!\n", __func__);
        return DIFFERENT;
    }
    if (imgA->comp != imgB->comp) {
        fprintf(stderr, "[%s] Images do not have the same number of color components!\n",
                __func__);
        return DIFFERENT;
    }

    char flag = SAME;
    int x, y;
    uint8_t *ptr_a = imgA->data;
    uint8_t *ptr_b = imgB->data;
    for (y = 0; y < imgA->width; ++y) {
        for (x = 0; x < imgA->height; ++x) {
            if (*ptr_a != *ptr_b) {
	        if (list) {
		    printf("[%d,%d] : expected %X != actual %X\n", x, y, *ptr_a, *ptr_b);
		    flag = DIFFERENT;
		} else {
		    return DIFFERENT;
		}
	    }
	    ptr_a++;
	    ptr_b++;
	}
    }

    return flag;
}