#pragma once
#include<stdio.h>
#include"BasicFunction.h"
#include<string.h>

void FindText(FILE* fp,unsigned char* buffer);

//�����ļ�ָ�룬����������ַ�����·��
void FindHtml(FILE* fp, unsigned char* buffer,const char* OutputPath);

void InitTextTable();

//���ĳ�������Ƿ�Ϊ���ı������ı��᷵��1
int CheckSector(unsigned char* buffer);
