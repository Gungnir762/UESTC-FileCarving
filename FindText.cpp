#include"FindText.h"

int TextTable[256] = { 0 };

void InitTextTable() {
	for (int i = 0; i < 256; i++) {
		TextTable[i] = 2;
	}
	for (int i = 32; i <= 126; i++) {
		TextTable[i] = 1;
	}
	for (int i = 9; i <= 13; i++) {
		TextTable[i] = 1;
	}
	TextTable[153] = 1;
	TextTable[169] = 1;
	TextTable[174] = 1;
}

int CheckSector(unsigned char* buffer) {
	int res = 0;
	for (int i = 0; i < SECTOR_SIZE; i++) {
		res |= TextTable[buffer[i]];
	}
	return res;
}

//输入文件指针，缓冲区基地址，输出路径
void FindHtml(FILE* fp, unsigned char* buffer, const char* OutputPath) {
	int curSector = 0;//当前读取扇区号
	int State = 0;//0：不是html段且不含未完成文件；1：是html段
	int StartLoc = 0, EndLoc = 0;//html段在缓冲区中的起始和结束
	int curLoc = 0;//指向缓冲区中当前有效html文件的最末端
	int cnt = 0;//找到的文件个数
	int cntWind = 0, cntEnd = 0;//出现文件缠绕时，缠绕个数和结束的文件个数
	char FileName[200];
	fseek(fp, 0, SEEK_END);
	int SectorNum = ftell(fp) / SECTOR_SIZE;
	fseek(fp, 0, SEEK_SET);

	InitTextTable();
	for (curSector = 0; curSector < SectorNum; curSector++) {
		if (State == 0) {
			curLoc = -1;
			ReadSector(fp, curSector, buffer);
			for (int i = 0; i < SECTOR_SIZE - 10; i++) {
				if ((char)buffer[i] == '<') {
					if (!_strnicmp((const char*)(buffer + i + 1), "html", 4)) {
						printf_s("%d ", curSector);
						State = 1;
						StartLoc = 0;
						cntWind = 0;
						cntEnd = 0;
						curSector--;
						break;
					}
				}
			}
		}
		else if (State == 1) {
			ReadSector(fp, curSector, buffer + curLoc + 1);
			if (CheckSector(buffer + curLoc + 1) == 1) {
				for (int i = curLoc + 1; i < curLoc + 1 + SECTOR_SIZE - 10; i++) {
					if ((char)buffer[i] == '<') {
						if (!_strnicmp((const char*)(buffer + i + 1), "html", 4)) {
							cntWind++;
							break;
						}
					}
				}
				curLoc += SECTOR_SIZE;
			}
			else {
				int f = 0;
				for (int i = curLoc + SECTOR_SIZE; i > curLoc; i--) {
					if (i < 6)break;
					if ((char)buffer[i] == '>') {
						if (!_strnicmp((const char*)(&buffer[i - 6]), "</html", 6)) {
							f = 1;
							EndLoc = i;
							break;
						}
					}
				}
				if (f == 1) {
					cntEnd++;
					if (cntWind == cntEnd) {
						cnt++;
						sprintf_s(FileName, "%s\\%d", OutputPath, cnt);
						if (cntWind > 1) {
							char tmp[20];
							sprintf_s(tmp, "_%dfiles", cntWind);
							strcat_s(FileName, tmp);
						}
						strcat_s(FileName, ".html");
						while (buffer[EndLoc + 1] == '\n') {
							EndLoc++;
						}
						printf_s("%d\n", curSector);
						OutputFile(FileName, buffer, StartLoc, EndLoc - StartLoc + 1);
						State = 0;
					}
					else {
						curLoc += SECTOR_SIZE;
					}
				}
			}
		}
	}
	printf_s("已成功恢复%d个可能的html文件\n", cnt);
}

void FindText(FILE* fp, unsigned char* buffer) {
}