#include "FindZip.h"

void FindContinueZip(FILE* fp, unsigned char* buffer, const char* OutputPath) {
	int curSector = 0;//��ǰ��ȡ������
	int State = 0;//0������zip���Ҳ���δ����ļ���1����zip��
	int EndLoc = 0;//zip�ļ��ڻ������еĽ���
	int cnt = 0;//�ҵ����ļ�����
	char FileName[200];
	int SectorNum = CalculateFileSize(fp) / SECTOR_SIZE;

	while (curSector < SectorNum) {
		ReadSector(fp, curSector, buffer);
		if (!CheckHeader(buffer)) {
			curSector++;
			continue;
		}
		sprintf_s(FileName, "%s\\%08d.zip", OutputPath, curSector);

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
	printf_s("�ѳɹ��ָ�%d�����ܵ�zip�ļ�\n", cnt);
	return;
}

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
