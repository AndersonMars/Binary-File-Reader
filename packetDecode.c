//Marshall Anderson
//CS: 450 Packet Decoder

#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>

int hexToDecimal(unsigned char hex);

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
	
	printf("Ethernet header: \n");
	printf("---------------- \n");
	
	//int[] for the reading of binary file
	unsigned char data[6];

	//read the destination MAC
	fread(&data, sizeof(data), 1, fp);
	
	printf("Destination MAC address: %9s", " ");
	//print the destination MAC
	for(int i = 0; i < sizeof(data); i++)
	{
		printf("%02x", data[i]);
		if(i != sizeof(data) - 1)printf(":");
	}
	printf("\n");
	
	//read the Source MAC address
	fread(&data, sizeof(data), 1, fp);
	
	printf("Source MAC address: %14s", " ");
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
	printf("Type: %28s", " ");
	for(int i = 0; i < 2; i++)
	{
		printf("%02x", type[i]);
	}
	printf("\n\n");
	
	//IPv4 Header
	printf("IPv4 Header: \n");
	printf("---------------- \n");
	
	//For the next byte, 4 bits belong to the version
	//while the other 4 belong to the ihl
	
	unsigned char version;
	fread(&version, sizeof(version), 1, fp);
	
	//idk why, but I have to do it this way for the correct output?
	unsigned char ihl = version << 4;
	ihl = ihl >> 4;
	version = version >> 4;

	printf("Version: %25s", " ");
	
	printf("%02x", version);
	
	printf("\n");
		
	printf("Internet Header Length: %10s", " ");
	
	printf("%02x", ihl);
	
	printf("\n");

	int ihlDecimal = hexToDecimal(ihl);
	bool option = ihlDecimal > 5;
	
	//for the next byte, 6 bits belong to the DSCP, while the other 2 belong to the ECN
	unsigned char dscp;
	fread(&dscp, sizeof(dscp), 1, fp);

	//&3 to get the ecn value, then shift right 2 for the dscp
	unsigned char ecn = dscp & 3;

	dscp = dscp >> 2;

	printf("DSCP: %27s %02x\n", " ", dscp);

	printf("ECN: %28s %02x", " ",ecn);

	if(ecn == 00) printf(" Non-ECT Packet\n");
	else if(ecn == 01) printf(" ECT(1) Packet\n");
	else if(ecn == 10) printf(" ECT(0) Packet\n");
	else if(ecn == 11) printf(" Congestion Experienced\n");

	//time to get the length of the file

	unsigned char length[2];
	fread(&length, sizeof(length), 1, fp);

	int totalLength = length[0] << 8 | length[1];

	printf("Total length: %19s %d\n", " ", totalLength);

	//print the identification
	unsigned char id[2];
	fread(&id, sizeof(id), 1, fp);

	int identifier = id[0] << 8 | id[1];

	printf("Identification: %17s %d\n", " ", identifier);

	//similarly to how we pulled the ecn and the dscp, we must pull the flags and fragment offset

	unsigned char offset[2];
	fread(&offset, sizeof(offset), 1, fp);

	//&3 to get the flags value, then shift right 2 for the fragment offset
	unsigned char flag = offset[0] >> 5;

	unsigned char flagBits[2];
	for(int i = 2; i >= 0; i--)
	{
		flagBits[i] = ((flag >> i) & 1);
	}

	//bit of a brute force approach, but it works (hopefully)
	if(flagBits[1] == 1)
	{
		printf("Flag: %27s Don't Fragment\n", " ");
	}
	else if(flagBits[2] == 1)
	{
		printf("Flag: %27s More Fragments\n", " ");
	}
	else
	{
		printf("Flag: %27s Reserved\n", " ");
	}

	
	offset[0] = offset[0] << 3;

	int trueOffset = offset[0] << 8 | offset[1];

	printf("Fragment Offset: %16s %d\n", " ", trueOffset);


	//Time to Live
	unsigned char ttl;
	fread(&ttl, sizeof(ttl), 1, fp);

	int ttlVal = hexToDecimal(ttl);

	printf("Time to Live: %19s %d\n", " ", ttlVal);

	//Protocol
	unsigned char protocol;
	fread(&protocol, sizeof(protocol), 1, fp);

	int protoVal = hexToDecimal(protocol);

	printf("Protocol: %23s %d\n", " ", protoVal);

	//Header Checksum, which is 2 bytes long
	unsigned char headerCheck[2];
	fread(&headerCheck, sizeof(headerCheck), 1, fp);

	printf("IP Checksum: %20s 0x%02x%02x\n", " ", headerCheck[0], headerCheck[1]);

	//Source IP Address, which is 4 bytes
	unsigned char sourceIP[4];
	fread(&sourceIP, sizeof(sourceIP), 1, fp);

	int sectionFirst = hexToDecimal(sourceIP[0]);
	int sectionSecond = hexToDecimal(sourceIP[1]);
	int sectionThird = hexToDecimal(sourceIP[2]);
	int sectionFourth = hexToDecimal(sourceIP[3]);

	printf("Source IP Address: %14s %d.%d.%d.%d\n", " ", sectionFirst, sectionSecond, sectionThird, sectionFourth);

	//Destination IP Address, which is 4 bytes

	unsigned char desIP[4];
	fread(&desIP, sizeof(desIP), 1, fp);

	int desFirst = hexToDecimal(desIP[0]);
	int desSecond = hexToDecimal(desIP[1]);
	int desThird = hexToDecimal(desIP[2]);
	int desFourth = hexToDecimal(desIP[3]);

	printf("Destination IP Address: %9s %d.%d.%d.%d\n", " ", desFirst, desSecond, desThird, desFourth);

	//Options, check if the previous bool is true, if it is print which option it is, with the four bytes
	if(option == false)
	{
		printf("Options: %25sNo Options \n", " ");
	}
	else
	{
		int optionCount = 0;
		int options = hexToDecimal(ihl);
		for(int i = options; i > 5; i--)
		{
			unsigned char optionByte[4];
			fread(&optionByte, sizeof(optionByte), 1, fp);
			printf("Option Word %d: %19s0x%02x%02x%02x%02x\n", optionCount, " ", optionByte[0], optionByte[1], optionByte[2], optionByte[3]);
			optionCount++;
		}
	}

	//Currently printing one extra byte at the end, a copy of the last byte
	//print the rest of the file, will do in batches of 8
	printf("\n");
	printf("Payload: \n");
	
	unsigned char payload;
	//get end of file
	while(!feof(fp))
	{
		//4 columns
		for(int i = 0; i < 4; i++)
		{
			//read and print 8 bytes
			for(int j = 0; j < 8; j++)
			{
				fread(&payload, sizeof(payload), 1, fp);
				if(feof(fp)) 
				{
					break;
				}	
				printf("%02x ", payload);
			}
			//tab
			printf("   ");
		}
		//new line after 4 columns
		printf("\n");	
	}
	
	//close the file
	fclose(fp);

}


//function that converts hex value to a binary form, and then converts that binary form to decimal, brute force method
//since I did not want to import math library, making compiler call more complicated
//this function assumes an 8 bit byte is given as input, so I will do this manually for smaller chunks
int hexToDecimal(unsigned char hex)
{
	unsigned char binary[8];
	for(int i = 7; i >= 0; i--)
	{
		binary[i] = ((hex >> i) & 1);
	}

	int val = 0;
	for(int i = 0; i < 8; i++)
	{

		if(binary[i] == 1)
		{
			if(i == 0) val += 1;
			else if(i == 1) val += 2;
			else if(i == 2) val += 4;
			else if(i == 3) val += 8;
			else if(i == 4) val += 16;
			else if(i == 5) val += 32;
			else if(i == 6) val += 64;
			else if(i == 7) val += 128;
		}
	}

	return val;
}
