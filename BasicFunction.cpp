#include"BasicFunction.h"

void printf_error(const char* s)
{
	printf("\033[0m\033[1;31m%s\033[0m", s);
}

void printf_success(const char* s)
{
	printf("\033[0m\033[1;32m%s\033[0m", s);
}

void ReadSectorError(unsigned char* buffer)
{
	for (int i = 0; i < SECTOR_SIZE; i++) {
		buffer[i] = 0x0F;
	}
	return;
}

void ReadSector(FILE* file, unsigned int sector, unsigned char* buffer)
{
	int tmp;
	fseek(file, sector * SECTOR_SIZE, SEEK_SET);
	for (int i = 0; i < SECTOR_SIZE; i++) {
		tmp = fgetc(file);
		if (tmp == EOF) {
			ReadSectorError(buffer);
			return;
		}
		buffer[i] = (unsigned char)tmp;
	}
	return;
}

void OutputFile(const char* filename, unsigned char* buffer, unsigned int begin, unsigned int offset)
{
	FILE* fp = NULL;
	printf_s("building file:%s\n", filename);
	if (begin + offset > MAX_SIZE) {
		printf_error("\noffset overflow\n");
		return;
	}
	if (fopen_s(&fp, filename, "wb+")) {
		printf_error("\nfilename unsuitable or this file is using by another process\n");
		return;
	}
	for (unsigned int i = 0; i < offset; i++) {
		if (fputc((int)buffer[begin + i], fp) == EOF) {
			printf_error("\nwrite file error\n");
			fclose(fp);
			return;
		}
	}
	fclose(fp);
	printf_success("\nbuild file complete\n");
	return;
}

int CalculateFileSize(FILE* fp)
{
	FILE* tmpfp = fp;
	fseek(fp, 0, SEEK_END);
	int res = ftell(fp);
	fp = tmpfp;
	return res;
}

int ReadFileUntil(const unsigned char* keyword, int len, FILE* fp, unsigned char* buffer)
{
	int cnt = 0;//读取字节数
	int flag = 0;//是否找到关键词标识
	while (true) {
		int tmp = fgetc(fp);
		if (tmp == EOF) {
			return -1;
		}
		buffer[cnt] = tmp;
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
	}
	return res;
}

int CreateFolder(const char* FolderPath)
{
	int res;
	if (_access(FolderPath, 0) != 0) {
		res = _mkdir(FolderPath);
	}
	else {
		res = 1;
	}
	return res;
}


