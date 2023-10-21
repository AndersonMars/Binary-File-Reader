//Marshall Anderson
//CS: 450 Packet Decoder

#include <stdio.h>

//this struct will allow pulling multiple numbers at once from the binary file

int main(int argc, char** argv)
{
	//if only the call for the program was run, then exit
	if (argc == 1)
	{
		printf("Error: Please give the name of the binary file you wish to print.\n");
		return 0;
	}

	//if more than 2 arguments are given, exit
	if (argc > 2)
	{
		printf("Error: Please only give one binary file to print\n");
		return 0;
	}
	
	char* fileName = argv[1];
	
	//create the file pointer
	FILE* fp;
	
	//open the file in read mode, use rb since this is a non-text file
	fp = fopen(fileName, "rb");
	
	//if the file couldn't be opened.
	if(fp == NULL)
	{
		printf("Error opening file.\n");
		return 0;
	}
	
	//int[] for the reading of binary file
	char data[4];

	//read the destination MAC
	fread(&data, sizeof(data), 1, fp);
	
	//print the destination MAC
	for(int i = 0; i < sizeof(data); i++)
	{
	
		printf("%d%d:%d%d", data[0], data[1], data[2], data[3]);
	
	}


	//close the file
	fclose(fp);
}
