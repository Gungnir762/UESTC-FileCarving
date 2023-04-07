#include <cstdint>
#include<set>
#include"FindJPG.h"
#include"BasicFunction.h"
#include"huffmanTree.h"

//从buffer中的offset开始读取2字节数据
uint16_t read2Bytes(unsigned char* buffer, unsigned offset)
{
	uint16_t value = 0;
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

static const char* huffman2binary_str(int huffman_code, int n_bits, char buf[64]) {
	int mask = 1 << (n_bits - 1);
	for (int i = 0; i < n_bits; i++) {
		if (huffman_code & mask) {
			buf[i] = '1';
		}
		else {
			buf[i] = '0';
		}
		mask >>= 1;
	}
	buf[n_bits] = 0;
	return buf;
}

//从sector_begin开始找第一个FFC4，返回一个包含所有哈夫曼编码的集合
set<string> get_huffmanCode_set(FILE* fp, unsigned int sector_begin, unsigned int* max_huffman_code_len)
{
	static unsigned char* tmp = (unsigned char*)malloc(SECTOR_SIZE * sizeof(unsigned char));
	set<string> huffman_code_set;
	*max_huffman_code_len = 0;
	for (unsigned int begin = sector_begin; begin < sector_begin + 5; begin++)//DHT可能在文件头之后1个扇区
	{
		ReadSector(fp, begin, tmp);
		for (int offset = 0; offset < SECTOR_SIZE - 1; offset++)
		{
			if (read2Bytes(tmp, offset) == DHT)
			{
				const unsigned char* numbers = tmp + offset + 5;
				const unsigned char* symbols = numbers + 16;

				char buf[64];
				int huffman_code = 0;
				for (int i = 0; i < 16; i++) {
					int num = numbers[i];
					int n_bits = i + 1;

					for (int j = 0; j < num; j++) {
						int symbol = *symbols;
						if (n_bits > *max_huffman_code_len)
							*max_huffman_code_len = n_bits;

						printf("0x%0.2x | %s\n", symbol, huffman2binary_str(huffman_code, n_bits, buf));
						huffman_code_set.insert(huffman2binary_str(huffman_code, n_bits, buf));

						huffman_code++;
						symbols++;
					}
					huffman_code <<= 1;
				}
				return huffman_code_set;
			}
		}
	}
	return huffman_code_set;
}

//set<string> create_huffman_lookup_table(set<string> huffman_code_set, unsigned int max_huffman_code_len) {
//	set<string> huffman_look_up_table;
//	for (auto code : huffman_code_set) {
//		while (code.length() < max_huffman_code_len) {
//			code = "0" + code;
//		}
//		huffman_look_up_table.insert(code);
//	}
//	return huffman_look_up_table;
//}

uint16_t string2bin(string* s)
{
	uint16_t n = 0;
	for (int i = 0; i < (*s).size(); i++)
	{
		n <<= 1;
		if ((*s)[i] == '1')
			n |= 1;
	}
	return n;
}

set<uint16_t> create_huffman_lookup_table(set<string> huffman_code_set)
{
	set<uint16_t> huffman_look_up_table;
	for (string code : huffman_code_set) {
		huffman_look_up_table.insert(string2bin(&code));
	}
	return huffman_look_up_table;
}

bool is_in_set(const std::set<uint16_t>& set, uint16_t num)
{
	return set.find(num) != set.end();
}
//
//bool is_jpg_sector(unsigned char* buffer, unsigned int offset, set<uint16_t> huffman_look_up_table)
//{
//	int warning_level = 1;//判断是不是属于jpg的块，大于10则退出
//	for (unsigned int offset_test = 0; offset_test < offset; ++offset_test)
//	{
//		if (is_in_set(huffman_look_up_table, read2Bytes(buffer, offset_test)) && warning_level != 1)
//			warning_level--;
//		else
//			warning_level *= 2;
//		if (warning_level > 10)
//			return false;
//	}
//	return true;
//}
//从sector_begin开始一直往sector_end找,直到找到第一个0xFFD9，返回其所在的扇区号
int get_JPG_end(FILE* fp, set<string> huffman_code_set, unsigned int sector_begin, unsigned int sector_end)
{
	static unsigned char* temp = (unsigned char*)malloc(SECTOR_SIZE * sizeof(unsigned char));

	for (unsigned int i = sector_begin; i < sector_end; i++)
	{
		ReadSector(fp, i, temp);
		for (unsigned int offset = 0; offset < SECTOR_SIZE - 1; ++offset) {

			if (read2Bytes(temp, offset) == EOI)
			{
				//if (is_jpg_sector(temp, offset, huffman_look_up_table))
				if (is_jpg_sector(temp, huffman_code_set) == 1)
				{
					printf_s("end at sector: %d\n", i);
					return i;
				}

			}
		}
	}
	return -1;
}

int jump_to_sos(FILE* fp, unsigned int curSector, unsigned int SectorNum)
{
	int sos_begin_sector = 0;

	return sos_begin_sector;
}

void rebuild_JPG(FILE* fp, unsigned char* buffer, const char* output_path)
{
	unsigned int curSector = 0;//当前读取扇区号
	int cnt = 0;//找到的文件个数
	char fileName[200];
	set<string> huffman_code_set;//存储找到的哈夫曼编码
	set<uint16_t> huffman_look_up_table;//哈夫曼编码查找表
	fseek(fp, 0, SEEK_END);
	unsigned int SectorNum = ftell(fp) / SECTOR_SIZE;//总扇区数
	fseek(fp, 0, SEEK_SET);
	for (curSector = 0; curSector < SectorNum; curSector++) {
		ReadSector(fp, curSector, buffer);
		if (JFIF_head_found(buffer)) {
			printf_s("JFIF begin at sector: %d\n", curSector);
			unsigned int max_huffman_code_len = 0;
			huffman_code_set = get_huffmanCode_set(fp, curSector, &max_huffman_code_len);
			//从SOI之后开始，找到压缩数据的开头，避免FFD9的误伤
			int sos_begin = jump_to_sos(fp, curSector, SectorNum);
			/*for (auto it = huffman_look_up_table.begin(); it != huffman_look_up_table.end(); it++)
			{
				cout << std::hex << *it << endl;
			}

			printf("哈夫曼编码的最大长度为：%d\n", max_huffman_code_len);*/

			int sector_end = get_JPG_end(fp, huffman_code_set, curSector, SectorNum);
			if (sector_end == -1)
				printf_s("未找到文件尾\n");
		}
		else if (EXIF_head_found(buffer)) {
			printf_s("EXIF begin at sector:%d\n", curSector);
		}
		else {
		}
		huffman_code_set.clear();
	}
}



