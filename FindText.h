#pragma once
#include<stdio.h>
#include"BasicFunction.h"
#include<string.h>
#include<set>
using namespace std;

//输入文件指针，缓冲区基地址，输出路径，会调用FindHtml
void FindText(FILE* fp, unsigned char* buffer, const char* OutputPath);

//输入文件指针，缓冲区基地址，输出路径，会usedSector中更新已经找到的扇区
void FindHtml(FILE* fp, unsigned char* buffer, const char* OutputPath, set<int>& usedSector);

//初始化字符集
void InitTextTable();

//检查某个扇区是否为纯文本，纯文本会返回1
int CheckSector(unsigned char* buffer);
