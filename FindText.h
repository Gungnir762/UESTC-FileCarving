#pragma once
#include<stdio.h>
#include"BasicFunction.h"
#include<string.h>
#include<set>
using namespace std;

//���ҵ����ı��ļ�ֱ�������Ӳ�̣�Html���ֵ�������FindHtml���������ظ����
//�����ļ�ָ�룬����������ַ�����·��
void FindText(FILE* fp, unsigned char* buffer, const char* OutputPath);

//���ҵ���Html�ļ�ֱ�������Ӳ��
//�����ļ�ָ�룬����������ַ�����·��������usedSector�и����Ѿ��ҵ�������
void FindHtml(FILE* fp, unsigned char* buffer, const char* OutputPath, set<int>& usedSector);

//��ʼ���ַ���
void InitTextTable();

//���ĳ�������Ƿ�Ϊ���ı������ı��᷵��1
int CheckSector(unsigned char* buffer);
