#pragma once
#include<stdio.h>
#include"BasicFunction.h"
#include<string.h>
#include<set>
using namespace std;

//将找到的文本文件直接输出到硬盘，Html部分单独调用FindHtml处理，不会重复输出
//输入文件指针，缓冲区基地址，输出路径
void FindText(FILE* fp, unsigned char* buffer, const char* OutputPath);

//将找到的Html文件直接输出到硬盘
//输入文件指针，缓冲区基地址，输出路径，会在usedSector中更新已经找到的扇区
void FindHtml(FILE* fp, unsigned char* buffer, const char* OutputPath, set<int>& usedSector);

//初始化字符集
void InitTextTable();

//检查某个扇区是否为纯文本，纯文本会返回1
int CheckSector(unsigned char* buffer);
