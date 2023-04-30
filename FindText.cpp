#include"FindText.h"

int TextTable[256] = { 0 };

void InitTextTable() {
	//将所有非ASCII码字符标记为2，ASCII码（以及一些常用的非ASCII码）标记为1
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

//检查某个扇区是否为纯文本，纯文本会返回1
int CheckSector(unsigned char* buffer) {
	int res = 0;
	for (int i = 0; i < SECTOR_SIZE; i++) {
		res |= TextTable[buffer[i]];
	}
	return res;
}

//输入文件指针，缓冲区基地址，输出路径，会在usedSector中更新已经找到的扇区
void FindHtml(FILE* fp, unsigned char* buffer, const char* OutputPath, set<int>& usedSector) {
	int curSector = 0;//当前读取扇区号
	int State = 0;//0：不是html段且不含未完成文件；1：是html段
	int StartLoc = 0, EndLoc = 0;//html段在缓冲区中的起始和结束
	int curLoc = -1;//指向缓冲区中当前有效html文件的最末端
	int cnt = 0;//找到的文件个数
	int cntWind = 0, cntEnd = 0;//出现文件缠绕时，缠绕个数和结束的文件个数
	char FileName[200];//输出文件名
	int SectorNum = CalculateFileSize(fp) / SECTOR_SIZE;//扇区个数
	int StartSector;//输出文件用

	//遍历扇区寻找html文件，重点关注文件头和尾，根据找到的头和尾转移状态
	for (curSector = 0; curSector < SectorNum; curSector++) {
		if (State == 0) {
			ReadSector(fp, curSector, buffer);
			for (int i = 0; i < SECTOR_SIZE - 10; i++) {
				if ((char)buffer[i] == '<') {
					if (!_strnicmp((const char*)(buffer + i + 1), "html", 4)) {
						StartSector = curSector;
						curLoc = -1;
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
				usedSector.insert(curSector);
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
					usedSector.insert(curSector);
					if (cntWind == cntEnd) {
						cnt++;
						sprintf_s(FileName, "%s\\%08d", OutputPath, StartSector);
						if (cntWind > 1) {
							char tmp[20];
							sprintf_s(tmp, "_%dfiles", cntWind);
							strcat_s(FileName, tmp);
						}
						strcat_s(FileName, ".html");
						while (buffer[EndLoc + 1] == '\n') {
							EndLoc++;
						}
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

//输入文件指针，缓冲区基地址，输出路径
void FindText(FILE* fp, unsigned char* buffer, const char* OutputPath) {
	set<int> usedSector;//记录已经输出的扇区，再次寻找时跳过这些扇区
	InitTextTable();
	FindHtml(fp, buffer, OutputPath, usedSector);

	int curSector = 0;//当前读取扇区号
	int State = 0;//0：不是txt段且不含未完成文件；1：是txt段
	int StartLoc = 0, EndLoc = 0;//txt段在缓冲区中的起始和结束
	int curLoc = -1;//指向缓冲区中当前有效txt文件的最末端
	int cnt = 0;//找到的文件个数
	char FileName[200];//输出文件名
	int SectorNum = CalculateFileSize(fp) / SECTOR_SIZE;//扇区个数
	int StartSector;//输出文件用

	//找到所有连续的文本文件片段并输出
	for (curSector = 0; curSector < SectorNum; curSector++) {
		if (usedSector.find(curSector) != usedSector.end()) {
			continue;
		}
		ReadSector(fp, curSector, buffer + curLoc + 1);
		int ck = CheckSector(buffer + curLoc + 1);
		int tmpcnt = 0;
		if (ck != 1) {
			for (int i = curLoc + 1; i < curLoc + 1 + SECTOR_SIZE; i++) {
				tmpcnt += (TextTable[buffer[i]] == 2);
				if (tmpcnt > 50) break;
			}
		}
		if (State == 0) {
			curLoc = -1;
			if (tmpcnt > 50) continue;
			else {
				StartSector = curSector;
				curLoc += SECTOR_SIZE;
				StartLoc = 0;
				State = 1;
			}
		}
		else if (State == 1) {
			if (ck == 1) {
				curLoc += SECTOR_SIZE;
			}
			else {
				//设置“警报等级”，当该等级达到某一个定值说明文本段结束。
				int WarningLevel = 1;
				int fail = 0;
				int LastLowLevel = curLoc;
				for (int i = curLoc + 1; i < curLoc + 1 + SECTOR_SIZE; i++) {
					if (TextTable[buffer[i]] == 1) {
						WarningLevel--;
						if (!WarningLevel)WarningLevel = 1;
					}
					else {
						WarningLevel *= 2;
					}
					if (WarningLevel == 1)LastLowLevel = i;
					if (WarningLevel > 10) {
						fail = 1;
						break;
					}
				}
				if (fail) {
					State = 0;
					curLoc = -1;
					EndLoc = max(StartLoc, LastLowLevel);
					cnt++;
					sprintf_s(FileName, "%s\\%08d.txt", OutputPath, StartSector);
					OutputFile(FileName, buffer, StartLoc, EndLoc - StartLoc + 1);
				}
				else {
					curLoc += SECTOR_SIZE;
				}
			}
		}
	}
	printf_s("已成功恢复%d个可能的txt文件\n", cnt);
}