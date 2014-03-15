#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;

#define MAX_OPENED_FILE 5
#define DISK_BLOCK_SIZE 64
#define MAX_BLOCK_NUM 128
#define BUFFER_SIZE 1024
#define FILE_NAME_SIZE 3
#define FILE_OTHER_ATTRIBUTE_SIZE 1

typedef struct {
	int d_num; // disk block num
	int b_num; // byte num
}Pointer;

typedef struct {
	char name[20]; // file absoluted path
	char attribute; // file attribute, 1 byte
	int number; // begining num of disk
	int length; // file length
	int flag; // type of operation, 0 read, 1 write
	Pointer read; // read pointer
	Pointer write; // write pointer
}OFTLE;

typedef struct {
	OFTLE file[MAX_OPENED_FILE]; // opened file, less than 5
	int length;
}Openfile;

typedef struct {
	char name[3];
	char type[2];
	char attribute;
	char number;
	char length;
}Entry;

char disk[MAX_BLOCK_NUM * DISK_BLOCK_SIZE];
char fat[MAX_BLOCK_NUM];
char buffer[BUFFER_SIZE];
char path[12];
#endif