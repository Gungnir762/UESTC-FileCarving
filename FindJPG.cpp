/*
author:zyr
function:jpeg文件恢复的函数定义文件
notice:jump2SOS()可能存在一些bug。
若屏蔽此函数，可恢复的文件有：2d,3b,1b,3c,3g2,3i
若使用此函数，可恢复的文件有：3a,2d,3b,1b,3g2,3i
相比之下少了3c，多了3a
*/

#include <cstdint>
#include<set>
#include"FindJPG.h"
#include"BasicFunction.h"

//从buffer中的offset开始读取2字节数据
uint16_t read2Bytes(uint8_t* buffer, unsigned offset)
{
	uint16_t value = 0;
	value |= buffer[offset] << 8;
	value |= buffer[offset + 1];
	return value;
}

//寻找JFIF文件头
bool JFIF_head_found(uint8_t* buffer)
{
	if (read2Bytes(buffer, 0) == SOI && read2Bytes(buffer, 2) == APP0)
		return true;
	return false;
}

bool EXIF_head_found(uint8_t* buffer)
{
	if (read2Bytes(buffer, 0) == SOI && read2Bytes(buffer, 2) == APP1)
		return true;
	return false;
}

//判断num是否在set中
bool in_set(const std::set<uint16_t>& set, uint16_t num)
{
	return set.find(num) != set.end();
}

//从sector_begin开始一直往sector_end找,跳转到第一个压缩数据区
unsigned int jump2SOS(FILE* fp, unsigned int sector_begin, unsigned int sector_end)
{
	static uint8_t* temp_sector = (uint8_t*)malloc(SECTOR_SIZE * sizeof(uint8_t));
	set<uint16_t>key_set = { SOI,APP0,APP1,APP2,APP3,APP4,APP5,APP6,APP7,APP8,APP9,APP10,APP11,APP12,APP13,APP14,APP15,SOF0,SOF2,DHT,DQT,DRI,COM,EOI };
	set<uint16_t> none_jump_set = { SOI,RST0,RST1,RST2,RST3,RST4,RST5,RST6,RST7,0xFF00,0xFFFF };

	for (unsigned int i = sector_begin; i < sector_end; i++)
	{
		ReadSector(fp, i, temp_sector);
		for (unsigned int offset = 0; offset < SECTOR_SIZE - 1; ++offset)
		{
			uint16_t value = read2Bytes(temp_sector, offset);
			if (value == SOS)
			{
				printf_s("SOS begin at sector: %d,offset = %d\n", i, offset);
				return i;
			}
			else if (value == EOI)
			{
				continue;
			}
			else if (temp_sector[offset] == 0xFF && in_set(key_set, value) && !in_set(none_jump_set, value))
			{
				printf_s("for header: 0x%X in sector %d,offset=0x%X,", value, i, offset + (i - sector_begin) * SECTOR_SIZE);
				unsigned int len = read2Bytes(temp_sector, offset + 2);////当前扫描到的数据区长度
				printf_s("len = 0x%X\n", len);
				//跳转后的偏移量
				offset = (offset + len + 2) % SECTOR_SIZE;
				if (len > SECTOR_SIZE - offset)//数据区长度大于该块剩余大小
				{
					//跳转块号（取模）
					i = i  + (unsigned int)(len / SECTOR_SIZE);
					break;
				}
			}
		}
	}
	return sector_begin;
}


//从sector_begin开始一直往sector_end找,直到找到第一个0xFFD9，返回其所在的扇区号
int get_JPG_end(FILE* fp, unsigned int sector_begin, unsigned int sector_end, unsigned int* end_offset)
{
	static uint8_t* temp_sector = (uint8_t*)malloc(SECTOR_SIZE * sizeof(uint8_t));

	for (unsigned int curSector = sector_begin; curSector < sector_end; curSector++)
	{
		ReadSector(fp, curSector, temp_sector);
		for (unsigned int offset = 0; offset < SECTOR_SIZE - 1; ++offset)
		{
			if (read2Bytes(temp_sector, offset) == EOI)
			{
				*end_offset = offset + 2;
				printf_s("end at sector: %d,offset=%d\n", curSector, offset);
				return curSector;
			}
		}
	}
	return -1;
}

void rebuild_JPG(FILE* fp, uint8_t* buffer, const char* output_path)
{
	unsigned int curSector = 0;//当前读取扇区号
	int cnt = 0;//找到的文件个数
	char fileName[200];
	unsigned int end_offset = 0;//FFD9在文件块内的偏移量
	static uint8_t* temp_sector = (uint8_t*)malloc(SECTOR_SIZE * sizeof(uint8_t));

	fseek(fp, 0, SEEK_END);
	unsigned int SectorNum = ftell(fp) / SECTOR_SIZE;//总扇区数
	fseek(fp, 0, SEEK_SET);

	for (curSector = 0; curSector < SectorNum; curSector++) {
		ReadSector(fp, curSector, temp_sector);
		if (JFIF_head_found(temp_sector)) {
			printf_s("JFIF begin at sector: %d\n", curSector);

			unsigned int SOS_begin = jump2SOS(fp, curSector, SectorNum);//= curSector;//

			unsigned int jpg_end_sector = get_JPG_end(fp, SOS_begin, SectorNum, &end_offset);
			if (jpg_end_sector == -1)
				printf_s("未找到文件尾\n");
			else
			{
				fseek(fp, curSector * SECTOR_SIZE, SEEK_SET);
				fread(buffer, sizeof(uint8_t), (jpg_end_sector - curSector) * SECTOR_SIZE + end_offset, fp);
				sprintf_s(fileName, "%s\\%08d.jpg", output_path, curSector);//以起始扇区号为文件名
				OutputFile(fileName, buffer, 0, (jpg_end_sector - curSector) * SECTOR_SIZE + end_offset);
				cnt++;
				fseek(fp, 0, SEEK_SET);
			}

			//调试
			//return;
		}
		else if (EXIF_head_found(temp_sector)) {
			printf_s("EXIF begin at sector:%d\n", curSector);
		}
	}
	printf_s("共找到%d个文件\n", cnt);
}



