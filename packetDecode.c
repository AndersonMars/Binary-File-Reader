//Marshall Anderson
//CS: 450 Packet Decoder

#include <stdio.h>

int main(int argc, char* argv)
{
	//if only the call for the program was run, then exit
	if (argc == 1)
	{
		printf("Error: Please give the name of the binary file you wish to print.");
		return 0;
	}

	//if more than 2 arguments are given, exit
	if (argc > 2)
	{
		printf("Error: Please only give one binary file to print");
		return 0;
	}
	
	//create the file pointer
	FILE* fptr;

	//open the file in read mode, use rb since this is a non-text file
	fp = fopen(argv[1], "rb");

	//calculate filesize
	int fileSize = ftell(fp);

	printf(fileSize);

	//close the file
	fclose(fp);
}