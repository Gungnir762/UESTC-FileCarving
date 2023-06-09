﻿#pragma once
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <io.h>
#define SECTOR_SIZE 512
#define MAX_SIZE 50*1024*1024

//打印红色错误信息
void printf_error(const char* s);
//打印绿色成功信息
void printf_success(const char* s);

//输入参数为File*文件指针,unsigned int 扇区号，返回为unsigned char*,错误时全填0F
void ReadSector(FILE* file, unsigned int sector, unsigned char* buffer);

//输入参数为char*文件名,unsigned char*缓冲区，unsigned int 缓冲区中文件起始地址，unsigned int 偏移量
void OutputFile(const char* filename, unsigned char* buffer, unsigned int begin, unsigned int offset);

//计算文件字节数
int CalculateFileSize(FILE* fp);

//从当前文件指针处一直读取文件，直到读到关键词后停止
//未使用KMP算法，关键词过长时效率可能很低
//输入关键词，关键词长度，文件指针，接收缓冲区
//输出读取字节个数，如果直到文件结束都未读到关键词，返回-1
int ReadFileUntil(const unsigned char* keyword, int len, FILE* fp, unsigned char* buffer);

//将小端存储的字节码转换为无符号整数
//输入缓冲区位置，字节长度，字节长度不可大于4
unsigned int Bytes2Int(const unsigned char* buffer, int len);

//创建文件夹
//返回0：成功；返回1：已存在；返回-1：失败
int CreateFolder(const char* FolderPath);