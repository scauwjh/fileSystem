#ifndef _FILE_SYSTEM_H__
#define _FILE_SYSTEM_H__

#define MAX_OPENED_FILE 5
#define ENTRY_SIZE 8
#define MAX_ENTRY_NUM 8
#define DISK_BLOCK_SIZE 64
#define MAX_BLOCK_NUM 128
#define BUFFER_SIZE 1024
#define FILE_NAME_SIZE 3
#define FILE_OTHER_ATTRIBUTE_SIZE 1
#define MAX_FOLDER_SIZE 10
#define RESOURCE_PATH "../../resource"
#define DISK_PATH "../../resource/disk"
#define ENTRY_NAME 3
#define ENTRY_TYPE 2
#define ENTRY_ATTRIBUTE 1
#define ENTRY_NUMBER 1
#define ENTRY_LENGTH 1


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "struct.h"
#include "../util/util.h"
using namespace std;

class fileSystem {
public:
    fileSystem();
	int mkdir(char *path);
	int cd(const char *path);
	void ls();
	int rm(char *path);
	int setPosition(char *path);
	int writeEntry(Entry *entry, Pointer *pointer);
	int readEntry(Entry *entry, Pointer *pointer);
	int searchEntry(char *name, int d_num);
	void setCurretnPath(int d_num, int b_num);
	int writeCheck(int d_num, int b_num);
	int clearEntry(Pointer *pointer);
//private:
    void updateFAT();
    int setFAT(int d_num, char next_num);
	char fat[MAX_BLOCK_NUM];
	char buffer[BUFFER_SIZE];
	Entry entry;
	Pointer current_point;
	Pointer before_last_folder;
	string current_path;
	string current_folder;
	Util util;
};

#endif
