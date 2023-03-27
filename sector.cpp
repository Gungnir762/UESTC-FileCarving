#include"sector.h"
#include <exception>

void ReadSector(FILE* file, unsigned int sector, unsigned char* buffer)
{
	fseek(file, sector * SECTOR_SIZE, SEEK_SET);
	fread_s(buffer, SECTOR_SIZE, sizeof(unsigned char), SECTOR_SIZE, file);
}

void BGC(FILE* file, unsigned int sector, unsigned char* buffer)
{

}

void OutputFile(char* filename, unsigned char* buffer, unsigned int begin, unsigned int offset)
{
	FILE* file = NULL;
	fopen_s(&file, filename, "wb");
	try
	{
		fwrite(&buffer[begin], sizeof(unsigned char), offset, file);
		fclose(file);
	}
	catch(exception e)
	{

	}
}

int CalculateFileSize(FILE* fp)
{
	FILE* tmpfp = fp;
	fseek(fp, 0, SEEK_END);
	int res = ftell(fp);
	fp = tmpfp;
	return res;
}

int ReadFileUntil(const unsigned char* keyword,int len, FILE* fp, unsigned char* buffer)
{
	int cnt = 0;//读取字节数
	int flag = 0;//是否找到关键词标识
	while (true){
		buffer[cnt] = fgetc(fp);
		if (buffer[cnt] == EOF) {
			return -1;
		}
		if (cnt + 1 >= len) for (int i = 0; i < len; i++) {
			if (buffer[cnt - i] != keyword[len - i]) {
				break;
			}
			if (i == len - 1) flag = 1;
		}
		cnt++;
		if (flag) {
			return cnt;
		}
	}
	return 0;
}

unsigned int Bytes2Int(const unsigned char* buffer, int len)
{
	if (len > 4) {
		return 0;
	}
	unsigned int res = 0;
	for (int i = 0; i < len; i++) {
		res += ((unsigned int)buffer[i] << (8 * i));
		printf_s("%u\n", res);
	}
	return res;
}
