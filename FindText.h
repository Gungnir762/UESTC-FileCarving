#pragma once
#include<stdio.h>
#include"BasicFunction.h"
#include<string.h>
#include<set>
using namespace std;

//�����ļ�ָ�룬����������ַ�����·���������FindHtml
void FindText(FILE* fp, unsigned char* buffer, const char* OutputPath);

//�����ļ�ָ�룬����������ַ�����·������usedSector�и����Ѿ��ҵ�������
void FindHtml(FILE* fp, unsigned char* buffer, const char* OutputPath, set<int>& usedSector);

//��ʼ���ַ���
void InitTextTable();

//���ĳ�������Ƿ�Ϊ���ı������ı��᷵��1
int CheckSector(unsigned char* buffer);
