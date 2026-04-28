/*
 * File      : image_processing.h
 * Author    : Rick Wertenbroek
 * E-mail    : rick.wertenbroek@heig-vd.ch
 * Institute : REDS - HES-SO HEIG-VD
 * Year      : 2016
 */

#ifndef __IMAGE_PROCESSING_H__
#define __IMAGE_PROCESSING_H__

/* PNG file format allows for padding between rows of data
 * We will not use padding (padding of 0 bytes) */
#define PNG_STRIDE_IN_BYTES 0

#define COMPONENT_RGBA      4 /* RGB 3 colors + alpha */
#define COMPONENT_RGB       3 /* RGB 3 colors */
#define COMPONENT_GRAYSCALE 1 /* Grayscale only 1 component */

/* RGB Weights for grayscale conversion */
#define LUMINOSITY_R 0.21
#define LUMINOSITY_G 0.72
#define LUMINOSITY_B 0.07

/* Offsets
 * Images are coded as RGB or RGBA */
#define R_OFFSET 0
#define G_OFFSET 1
#define B_OFFSET 2
#define A_OFFSET 3

/* Global Constants */
#define KERNEL_SIZE 3
#define CMD_SIZE 100

/* File paths */
#define IMAGE_FILE "img/cpu.png"
#define RESULT_FILE "img/expected_result.png"
#define STUDENT_FILE "img/student_result.png"
#define DIFF_FILE "img/diff.png"

/* FLAGS */
#define SAME 0
#define DIFFERENT 1

#endif /* __IMAGE_PROCESSING_H__ */
