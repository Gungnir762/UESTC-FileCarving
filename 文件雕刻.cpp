// 文件雕刻.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "BasicFunction.h"
#include "FindText.h"
#include "FindZip.h"
#include "md5.h"


void test_func() {

}

unsigned char buffer[MAX_SIZE];
int main()
{
	char FileName[200] = "D:\\暂存\\数据恢复与取证技术\\文件雕刻\\镜像\\dfrws-2006-challenge.raw";
	FILE* fp;
	fopen_s(&fp, FileName, "rb");

	int res = CreateFolder("D:\\暂存\\数据恢复与取证技术\\文件雕刻\\镜像\\MyOutput");
	if (res == -1) {
		printf_s("创建输出文件夹失败，进程结束\n");
		system("pause\n");
		return 0;
	}
	else {
		if (res == 1) {
			printf_s("输出文件夹已存在，将在该文件夹内进行恢复\n");
		}
	}

	printf_s("恢复zip文件：\n");
	res = CreateFolder("D:\\暂存\\数据恢复与取证技术\\文件雕刻\\镜像\\MyOutput\\zip");
	if (res == -1) {
		printf_s("创建子文件夹失败\n");
	}
	else {
		if (res == 1) {
			printf_s("子文件夹已存在，将在该子文件夹内进行恢复\n");
		}
		FindContinueZip(fp, buffer, "D:\\暂存\\数据恢复与取证技术\\文件雕刻\\镜像\\MyOutput\\zip");
	}

	printf_s("恢复文本文件：\n");
	res = CreateFolder("D:\\暂存\\数据恢复与取证技术\\文件雕刻\\镜像\\MyOutput\\text");
	if (res == -1) {
		printf_s("创建子文件夹失败\n");
	}
	else {
		if (res == 1) {
			printf_s("子文件夹已存在，将在该子文件夹内进行恢复\n");
		}
		FindText(fp, buffer, "D:\\暂存\\数据恢复与取证技术\\文件雕刻\\镜像\\MyOutput\\text");
	}
	printf_s("执行完成！");
	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
