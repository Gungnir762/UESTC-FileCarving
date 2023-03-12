#include"sector.h"
#include<iostream>
#include"md5.h"

void printMD5(const string& message) {
	cout << "md5(\"" << message << "\") = "
		<< MD5(message).toStr() << endl;
}

int main()
{
	//unsigned char* buffer = (unsigned char*)malloc(1024 * SECTOR_SIZE);
	//FILE* file = NULL;
	//if (file)
	//{
	//	fopen_s(&file, "test.img", "rb");
	//	ReadSector(file, 0, buffer);
	//	BGC(file, 0, buffer);
	//	//OutputFile("test.txt", buffer, 0x1C6, 0x1C6 + 0x1C);
	//	fclose(file);
	//}
	//free(buffer);



	printMD5("");
	printMD5("a");
	printMD5("abc");
	printMD5("message digest");
	printMD5("abcdefghijklmnopqrstuvwxyz");
	printMD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	
	return 0;
}
