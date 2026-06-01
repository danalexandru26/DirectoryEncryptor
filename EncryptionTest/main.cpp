#include<iostream>

#include"Crypto.hpp"
#include"Log.hpp"
#include"Utilities.hpp"

#define STREAM_TEST

void Test_fileReader();

int main()
{
	Test_fileReader();
	return 0;
}

void Test_fileReader()
{
	ReadRaw("test_read.txt");
	ReadArray("test_read_array.txt");
	ReadChunk("D:\\FPGA_Dev\\Digilent\\Zynq-7020\\Arty-Z7-20-Master.xdc");
}
