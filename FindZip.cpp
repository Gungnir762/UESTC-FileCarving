#include "FindZip.h"

//输入文件指针，缓冲区基地址，文件输出的绝对路径
void FindContinueZip(FILE* fp, unsigned char* buffer, const char* OutputPath) {
	int curSector = 0;//当前读取扇区号
	int State = 0;//0：不是zip段且不含未完成文件；1：是zip段
	int EndLoc = 0;//zip文件在缓冲区中的结束
	int cnt = 0;//找到的文件个数
	char FileName[200];//输出文件名
	int SectorNum = CalculateFileSize(fp) / SECTOR_SIZE;//扇区个数

	while (curSector < SectorNum) {
		//寻找zip文件头，并验证合法性
		ReadSector(fp, curSector, buffer);
		if (!CheckHeader(buffer)) {
			curSector++;
			continue;
		}
		sprintf_s(FileName, "%s\\%08d.zip", OutputPath, curSector);

		//寻找zip文件尾
		int FileOffset = curSector * SECTOR_SIZE;
		int len = 0;
		int EnderLength;
		fseek(fp, FileOffset, SEEK_SET);
		do {
			len += ReadFileUntil((const unsigned char*)"\x50\x4B\x05\x06", 4, fp, buffer);
			if (len == -1)return;
			if (fread_s(&buffer[len], MAX_SIZE - len, 1, 18, fp) != 18) return;
			len += 18;
			EnderLength = CheckEnder(&(buffer[len - 23]));
			if (EnderLength > 22) {
				if (fread_s(&buffer[len], MAX_SIZE - len, 1, EnderLength - 22, fp) != EnderLength - 22) return;
				len += EnderLength - 22;
			}
		} while (EnderLength == 0);

		OutputFile(FileName, buffer, 0, len - 1);
		cnt++;
		curSector = (FileOffset + len + SECTOR_SIZE - 1) / SECTOR_SIZE;
	}
	printf_s("已成功恢复%d个可能的zip文件\n", cnt);
	return;
}

//检查一个zip记录的文件头是否合法，若合法则返回该文件头的长度，否则返回0
//输入指向文件头起始处的指针
int CheckHeader(unsigned char* buffer) {
	unsigned char signature[5] = "\x50\x4B\x03\x04";
	for (int i = 0; i < 4; i++) {
		if (buffer[i] != signature[i]) return 0;
	}
	if (Bytes2Int(buffer + 4, 1) > 30 || Bytes2Int(buffer + 4, 1) == 0) return 0;
	if (Bytes2Int(buffer + 5, 1) > 14) return 0;
	if (Bytes2Int(buffer + 10, 2) > 0xBF7D) return 0;
	if (Bytes2Int(buffer + 12, 2) > 0x6421 || Bytes2Int(buffer + 12, 2) < 0x21) return 0;
	unsigned int FileNameLength = Bytes2Int(buffer + 26, 2);
	if (FileNameLength > 200) return 0;
	unsigned int ExtraFieldLength = Bytes2Int(buffer + 26, 2);
	if (ExtraFieldLength > 1000) return 0;
	return 30 + FileNameLength + ExtraFieldLength;
}

//检查一个zip文件的文件尾是否合法，若合法则返回文件尾的长度，否则返回0
//输入指向文件头起始处的指针
int CheckEnder(unsigned char* buffer) {
	unsigned char signature[5] = "\x50\x4B\x05\x06";
	for (int i = 0; i < 4; i++) {
		if (buffer[i] != signature[i]) return 0;
	}
	if (Bytes2Int(buffer + 8, 2) != Bytes2Int(buffer + 10, 2)) return 0;
	unsigned int CommentLength = Bytes2Int(buffer + 20, 2);
	if (CommentLength > 1000) return 0;
	return 22 + CommentLength;
}
