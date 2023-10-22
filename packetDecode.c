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
	unsigned char data[6];

	//read the destination MAC
	fread(&data, sizeof(data), 1, fp);
	
	printf("Destination MAC address: ");
	//print the destination MAC
	for(int i = 0; i < sizeof(data); i++)
	{
		printf("%02x", data[i]);
		if(i != sizeof(data) - 1)printf(":");
	}
	printf("\n");
	
	//read the Source MAC address
	fread(&data, sizeof(data), 1, fp);
	
	printf("Source MAC address: ");
	//print the destination MAC
	for(int i = 0; i < sizeof(data); i++)
	{
		printf("%02x", data[i]);
		if(i != sizeof(data) - 1)printf(":");
	}
	printf("\n");
	
	unsigned char type[2];
	
	fread(&type, sizeof(type), 1, fp);
	//print the type
	printf("Type: ");
	for(int i = 0; i < 2; i++)
	{
		printf("%02x", type[i]);
	}
	printf("\n");
	
	//print the rest of the file, will do in batches of 8
	printf("Payload: \n");
	
	unsigned char payload[8];
	
	//get end of file
	while(!feof(fp))
	{
		//4 columns
		for(int i = 0; i < 4; i++)
		{
			//read 8 bytes
			fread(&payload, sizeof(payload), 1, fp);
			
			//print 8 bytes
			for(int j = 0; j < 8; j++)
			{
				
				printf("%02x ", payload[j]);
			
			}
			//tab
			printf("\t");
		
		}
		//new line after 4 columns
		printf("\n");
	
	}
	
	//close the file
	fclose(fp);
}
