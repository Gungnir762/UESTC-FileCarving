#include"FindText.h"

int TextTable[256] = { 0 };

void InitTextTable() {
	//�����з�ASCII���ַ����Ϊ2��ASCII�루�Լ�һЩ���õķ�ASCII�룩���Ϊ1
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

//���ĳ�������Ƿ�Ϊ���ı������ı��᷵��1
int CheckSector(unsigned char* buffer) {
	int res = 0;
	for (int i = 0; i < SECTOR_SIZE; i++) {
		res |= TextTable[buffer[i]];
	}
	return res;
}

//�����ļ�ָ�룬����������ַ�����·��������usedSector�и����Ѿ��ҵ�������
void FindHtml(FILE* fp, unsigned char* buffer, const char* OutputPath, set<int>& usedSector) {
	int curSector = 0;//��ǰ��ȡ������
	int State = 0;//0������html���Ҳ���δ����ļ���1����html��
	int StartLoc = 0, EndLoc = 0;//html���ڻ������е���ʼ�ͽ���
	int curLoc = -1;//ָ�򻺳����е�ǰ��Чhtml�ļ�����ĩ��
	int cnt = 0;//�ҵ����ļ�����
	int cntWind = 0, cntEnd = 0;//�����ļ�����ʱ�����Ƹ����ͽ������ļ�����
	char FileName[200];//����ļ���
	int SectorNum = CalculateFileSize(fp) / SECTOR_SIZE;//��������
	int StartSector;//����ļ���

	//��������Ѱ��html�ļ����ص��ע�ļ�ͷ��β�������ҵ���ͷ��βת��״̬
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
	printf_s("�ѳɹ��ָ�%d�����ܵ�html�ļ�\n", cnt);
}

//�����ļ�ָ�룬����������ַ�����·��
void FindText(FILE* fp, unsigned char* buffer, const char* OutputPath) {
	set<int> usedSector;//��¼�Ѿ�������������ٴ�Ѱ��ʱ������Щ����
	InitTextTable();
	FindHtml(fp, buffer, OutputPath, usedSector);

	int curSector = 0;//��ǰ��ȡ������
	int State = 0;//0������txt���Ҳ���δ����ļ���1����txt��
	int StartLoc = 0, EndLoc = 0;//txt���ڻ������е���ʼ�ͽ���
	int curLoc = -1;//ָ�򻺳����е�ǰ��Чtxt�ļ�����ĩ��
	int cnt = 0;//�ҵ����ļ�����
	char FileName[200];//����ļ���
	int SectorNum = CalculateFileSize(fp) / SECTOR_SIZE;//��������
	int StartSector;//����ļ���

	//�ҵ������������ı��ļ�Ƭ�β����
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
				//���á������ȼ��������õȼ��ﵽĳһ����ֵ˵���ı��ν�����
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
	printf_s("�ѳɹ��ָ�%d�����ܵ�txt�ļ�\n", cnt);
}