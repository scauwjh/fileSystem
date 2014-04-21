#ifndef __UTIL_H__
#define __UTIL_H__

#define MAX_OPENED_FILE 5
#define ENTRY_SIZE 8
#define MAX_ENTRY_NUM 8
#define DISK_BLOCK_SIZE 64
#define MAX_BLOCK_NUM 128
#define BUFFER_SIZE 1024
#define FILE_NAME_SIZE 3
#define FILE_OTHER_ATTRIBUTE_SIZE 1
#define MAX_FOLDER_SIZE 10
#define RESOURCE_PATH "resource"
#define DISK_PATH "resource/disk"
#define ENTRY_NAME 3
#define ENTRY_TYPE 2
#define ENTRY_ATTRIBUTE 1
#define ENTRY_NUMBER 1
#define ENTRY_LENGTH 1

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

class Util{
public:
    static string getFileName(char *path);
    static string getParentPath(const char *path);
    static int openDisk(fstream *disk_pointer);
    static int compareName(char *a, char *b);
    static void myStrcpy(char *a, char *b, int len);
    static int getFolderCount(char *path);
    static int startWith(const string &a, const string &b);
    static int endWith(const string &a, const string &b);
};

#endif
