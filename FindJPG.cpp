#include <cstdint>
#include<set>
#include"FindJPG.h"
#include"BasicFunction.h"

//从buffer中的offset开始读取2字节数据
uint32_t read2Bytes(unsigned char* buffer, unsigned offset)
{
	uint32_t value = 0;
	value |= buffer[offset] << 8;
	value |= buffer[offset + 1];
	return value;
}


bool JFIF_head_found(unsigned char* buffer)
{
	if (read2Bytes(buffer, 0) == SOI && read2Bytes(buffer, 2) == APP0)
		return true;
	return false;
}

bool EXIF_head_found(unsigned char* buffer)
{
	if (read2Bytes(buffer, 0) == SOI && read2Bytes(buffer, 2) == APP1)
		return true;
	return false;
}

//从sector_begin开始找第一个FFC4，返回一个包含所有哈夫曼编码的集合
set<string> get_huffmanCode_set(FILE* fp, unsigned int sector_begin)
{
	set<string> huffmanCode_set;

	return huffmanCode_set;
}

//从sector_begin开始一直往sector_end找,直到找到第一个0xFFD9，返回其所在的扇区号
int get_JPG_end(FILE* fp, set<string> huffmanCode_set, unsigned int sector_begin, unsigned int sector_end)
{
	fseek(fp, 0, SEEK_SET);
	static unsigned char* temp = (unsigned char*)malloc(SECTOR_SIZE * sizeof(unsigned char));
	if (temp)
	{
		for (unsigned int i = sector_begin; i < sector_end; i++)
		{
			ReadSector(fp, i, temp);
			for (unsigned int j = 0; j < SECTOR_SIZE - 1; ++j) {
				if (read2Bytes(temp, j) == EOI)
				{
					printf_s("end at sector: %d\n", i);
					return i;
				}
			}
		}
		return -1;
	}
	else
	{
		printf_s("内存分配失败\n");
		return -2;
	}
}

void rebuild_JPG(FILE* fp, unsigned char* buffer, const char* output_path)
{
	unsigned int curSector = 0;//当前读取扇区号
	int cnt = 0;//找到的文件个数
	char fileName[200];
	fseek(fp, 0, SEEK_END);
	unsigned int SectorNum = ftell(fp) / SECTOR_SIZE;//总扇区数
	fseek(fp, 0, SEEK_SET);
	for (curSector = 0; curSector < SectorNum; curSector++) {
		ReadSector(fp, curSector, buffer);
		if (JFIF_head_found(buffer)) {
			printf_s("JFIF begin at sector: %d\n", curSector);
			set<string> huffmanCode_set = get_huffmanCode_set(fp, curSector);

			int sector_end = get_JPG_end(fp, huffmanCode_set, curSector, SectorNum);
			if (sector_end == -1)
				printf_s("未找到文件尾\n");
		}
		else if (EXIF_head_found(buffer)) {
			printf_s("EXIF begin at sector:%d\n", curSector);
		}
		else {
		}
	}
}



