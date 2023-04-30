/*
author:zyr
function:jpeg文件恢复的相关头文件，定义了一些常量
notice:
*/
#pragma once
#include<stdio.h>
#include<iostream>

using namespace std;

#define SOI         0xFFD8		/* Start Of Image */ 
#define APP0		0xFFE0      /* mask of Application-specific,JFIF*/
#define APP1		0xFFE1      /* mask of Application-specific,EXIF*/
#define APP2		0xFFE2      /* mask of Application-specific*/
#define APP3		0xFFE3      /* mask of Application-specific*/
#define APP4		0xFFE4      /* mask of Application-specific*/
#define APP5		0xFFE5      /* mask of Application-specific*/
#define APP6		0xFFE6      /* mask of Application-specific*/
#define APP7		0xFFE7      /* mask of Application-specific*/
#define APP8		0xFFE8      /* mask of Application-specific*/
#define APP9		0xFFE9      /* mask of Application-specific*/
#define APP10		0xFFEA      /* mask of Application-specific*/
#define APP11		0xFFEB      /* mask of Application-specific*/
#define APP12		0xFFEC      /* mask of Application-specific*/
#define APP13		0xFFED      /* mask of Application-specific*/
#define APP14		0xFFEE      /* mask of Application-specific*/
#define APP15		0xFFEF      /* mask of Application-specific*/
#define SOF0        0xFFC0      /* Start Of Frame (baseline DCT) */
#define SOF2        0xFFC2      /* Start Of Frame (progressive DCT) */
#define DHT         0xFFC4      /* Define Huffman Table(s) */
#define DQT         0xFFDB      /* Define Quantization Table(s) */
#define DRI         0xFFDD      /* Define Restart Interval */
#define SOS         0xFFDA      /* Start Of Scan */
#define RST0		0xFFD0      /* mask of Restart */
#define RST1		0xFFD1      /* mask of Restart */
#define RST2		0xFFD2      /* mask of Restart */
#define RST3		0xFFD3      /* mask of Restart */
#define RST4		0xFFD4      /* mask of Restart */
#define RST5		0xFFD5      /* mask of Restart */
#define RST6		0xFFD6      /* mask of Restart */
#define RST7		0xFFD7      /* mask of Restart */
#define COM         0xFFFE      /* Comment */
#define EOI         0xFFD9      /* End Of Image */

typedef uint16_t WORD;

/*
输入参数：文件指针，缓冲区，输出路径
功能：在对文件中可能的jpeg文件进行恢复后，将数据写入到输出路径中
*/
void rebuild_JPG(FILE* fp, uint8_t* buffer, const char* OutputPath);