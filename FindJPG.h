#pragma once
#include<stdio.h>
#include<iostream>

using namespace std;

#define SOI         0xFFD8		/* Start Of Image */ 
#define SOF0        0xFFC0      /* Start Of Frame (baseline DCT) */
#define SOF2        0xFFC2      /* Start Of Frame (progressive DCT) */
#define DHT         0xFFC4      /* Define Huffman Table(s) */
#define DQT         0xFFDB      /* Define Quantization Table(s) */
#define DRI         0xFFDD      /* Define Restart Interval */
#define SOS         0xFFDA      /* Start Of Scan */
#define RST_mask    0xFFD0      /* mask of Restart */
#define APP0		0xFFE0      /* mask of Application-specific,JFIF*/
#define APP1		0xFFE1      /* mask of Application-specific,EXIF*/
#define COM         0xFFFE      /* Comment */
#define EOI         0xFFD9      /* End Of Image */

typedef uint16_t WORD;

void rebuild_JPG(FILE* fp, unsigned char* buffer, const char* OutputPath);