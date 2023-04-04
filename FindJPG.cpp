#include <cstdint>
#include"FindJPG.h"
#include"BasicFunction.h"

bool JFIF_found(unsigned char* buffer)
{
	if (buffer[0] == 0xFF && buffer[1] == 0xD8) {//SOI
		if (buffer[2] == 0xFF && buffer[3] == 0xE0)//APP0
			return true;
	}
	return false;
}

bool EXIF_found(unsigned char* buffer)
{
	if (buffer[0] == 0xFF && buffer[1] == 0xD8) {//SOI
		if (buffer[2] == 0xFF && buffer[3] == 0xE1)//APP1
			return true;
	}
	return false;
}

//从sector_begin开始一直往sector_end找，经过的块全放入缓冲区，直到找到第一个0xFFD9
bool JPG_EndFound(FILE* fp, unsigned int sector_begin, unsigned int sector_end, unsigned char* buffer)
{
	fseek(fp, 0, SEEK_SET);
	unsigned char* tmp = (unsigned char*)malloc(SECTOR_SIZE * sizeof(unsigned char));
	if (tmp)
	{
		for (unsigned int i = sector_begin; i < sector_end; i++)
		{
			/*if (i == 4428)
				printf_s(" ");*/
			ReadSector(fp, i, tmp);
			int buffer_end = strlen((char*)buffer);
			for (unsigned int j = 0; j < SECTOR_SIZE; ++j) {
				if (tmp[j] == 0xFF && tmp[j + 1] == 0xD9)
				{
					printf_s("end at sector: %d\n", i);
					memcpy(&buffer[buffer_end], tmp, j + 1);
					return true;//找到了结束块
				}
			}
			memcpy(&buffer[buffer_end], tmp, SECTOR_SIZE);
		}
		free(tmp);
		return false;
	}
	else
	{
		printf_s("内存分配失败\n");
		return false;
	}


}

void FindJPG(FILE* fp, unsigned char* buffer, const char* OutputPath)
{
	unsigned int curSector = 0;//当前读取扇区号
	int cnt = 0;//找到的文件个数
	char fileName[200];
	fseek(fp, 0, SEEK_END);
	unsigned int SectorNum = ftell(fp) / SECTOR_SIZE;//总扇区数
	fseek(fp, 0, SEEK_SET);
	for (curSector = 0; curSector < SectorNum; curSector++) {
		ReadSector(fp, curSector, buffer);
		if (JFIF_found(buffer)) {
			printf_s("JFIF begin at sector: %d\n", curSector);
			if (JPG_EndFound(fp, curSector, SectorNum, buffer))
			{
				/*sprintf_s(fileName, "%s\\%d.jpg", OutputPath, cnt);
				printf_s("已恢复文件：%s", fileName);*/
				OutputFile(fileName, buffer, 0, SECTOR_SIZE);
				cnt++;
			}
			else
				printf_s("未找到文件尾\n");
		}
		else if (EXIF_found(buffer)) {
			printf_s("EXIF begin at sector:%d\n", curSector);

		}
		else {

		}
	}
}



