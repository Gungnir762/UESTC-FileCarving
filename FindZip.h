#pragma once
#include "BasicFunction.h"

//�ҵ�������zip�ļ�
//�����ļ�ָ�룬����������ַ���ļ�����ľ���·��
void FindContinueZip(FILE* fp, unsigned char* buffer, const char* OutputPath);

//���һ��zip��¼���ļ�ͷ�Ƿ�Ϸ������Ϸ��򷵻ظ��ļ�ͷ�ĳ��ȣ����򷵻�0
//����ָ���ļ�ͷ��ʼ����ָ��
int CheckHeader(unsigned char* buffer);

//���һ��zip�ļ����ļ�β�Ƿ�Ϸ������Ϸ��򷵻��ļ�β�ĳ��ȣ����򷵻�0
//����ָ���ļ�ͷ��ʼ����ָ��
int CheckEnder(unsigned char* buffer);