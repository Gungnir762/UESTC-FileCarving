#include"sector.h"
#include <exception>

void ReadSector(FILE* file, unsigned int sector, unsigned char* buffer)
{
	fseek(file, sector * SECTOR_SIZE, SEEK_SET);
	fread_s(buffer, SECTOR_SIZE, sizeof(unsigned char), SECTOR_SIZE, file);
}

void BGC(FILE* file, unsigned int sector, unsigned char* buffer)
{

}

void OutputFile(char* filename, unsigned char* buffer, unsigned int begin, unsigned int offset)
{
	FILE* file = NULL;
	fopen_s(&file, filename, "wb");
	try
	{
		fwrite(&buffer[begin], sizeof(unsigned char), offset, file);
		fclose(file);
	}
	catch(exception e)
	{

	}
}
