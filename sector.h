#include <stdio.h>

using namespace std;

#define SECTOR_SIZE 512

//输入参数为File*文件指针,unsigned int 扇区号，返回为unsigned char*,错误时全填0F
void ReadSector(FILE* file, unsigned int sector, unsigned char* buffer);

//BGC算法
void BGC(FILE* file, unsigned int sector, unsigned char* buffer);

//输入参数为char*文件名,unsigned char*缓冲区，unsigned int 缓冲区中文件起始地址，unsigned int 偏移量
void OutputFile(char* filename, unsigned char* buffer, unsigned int begin, unsigned int offset);