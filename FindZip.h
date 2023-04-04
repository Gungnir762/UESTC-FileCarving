#pragma once
#include "BasicFunction.h"

//找到连续的zip文件
//输入文件指针，缓冲区基地址，文件输出的绝对路径
void FindContinueZip(FILE* fp, unsigned char* buffer, const char* OutputPath);

//检查一个zip记录的文件头是否合法，若合法则返回该文件头的长度，否则返回0
//输入指向文件头起始处的指针
int CheckHeader(unsigned char* buffer);

//检查一个zip文件的文件尾是否合法，若合法则返回文件尾的长度，否则返回0
//输入指向文件头起始处的指针
int CheckEnder(unsigned char* buffer);