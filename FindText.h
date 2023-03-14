#pragma once
#include<stdio.h>
#include"BasicFunction.h"
#include<string.h>

void FindText(FILE* fp,unsigned char* buffer);

//输入文件指针，缓冲区基地址，输出路径
void FindHtml(FILE* fp, unsigned char* buffer,const char* OutputPath);

void InitTextTable();

//检查某个扇区是否为纯文本，纯文本会返回1
int CheckSector(unsigned char* buffer);
