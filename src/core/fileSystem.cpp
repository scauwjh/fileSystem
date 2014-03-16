#include "fileSystem.h"
#include "../util/util.h"
#include "../debug/debug.h"
#include "diskOperation/entry.cpp"
#include "diskOperation/fat.cpp"

int mkdir(char * a_path) {
	if (!*a_path) return -1;
	Pointer *pointer = new Pointer();
	pointer -> d_num = 2;
	pointer -> b_num = 0;
	Entry *entry = readEntry(pointer);
	char *name = strtok(a_path, "/");
	char pre_name[FILE_NAME_SIZE];
	int count = 0;
	while (name) {
		if (strlen(name) > FILE_NAME_SIZE) {
			delete pointer;
			return -1;
		}
		count ++;
		int flag = 0;
		for (int i = 0; i < MAX_ENTRY_NUM; i++) {
			if (!compareName(name, entry -> name)) {
				flag = 1;
				break;
			}
			pointer -> b_num = (i + 1) * ENTRY_SIZE;
			entry = readEntry(pointer);
			if (!entry) return -1;
		}
		// suppose it had found out, then get the next index
		pointer -> d_num = entry -> number;
		for(int i = 0; i < FILE_NAME_SIZE; i++)
			pre_name[i] = name[i];
		name = strtok(NULL, "/");
		// if not found and name is not end, return
		if (!flag && name) {
			delete pointer;
			return -1;
		}
	}
	if (count <= 1) {
		pointer -> d_num = 2;
		for (int i = 0; i < MAX_ENTRY_NUM; i++) {
			pointer -> b_num = i * ENTRY_SIZE;
			entry = readEntry(pointer);
			if (!compareName(entry -> name, pre_name)) {
				return -1;
			}
			if (!*entry -> name) {
				strcpy(entry -> name, pre_name);
				strcpy(entry -> type, " \0");
				entry -> attribute = 1;
				entry -> length = ENTRY_SIZE;
				char *fat = getFAT();
				int fat_flag = 0;
				for (int i = 3; i < MAX_BLOCK_NUM; i++) {
					if (fat[i] == 0) {
						entry -> number = i;
						fat_flag = i;
						break;
					}
				}
				if (fat_flag) {
					updateFAT(2, 0);
					// printf("pointer:%d %d\n", pointer -> d_num, pointer -> b_num);
					writeEntry(entry, pointer);
					updateFAT(fat_flag, fat_flag + 1);
					updateFAT(2, -1);
					delete pointer;
					return 1;
				}
			}
		}
		return -1;
	}
	// strcpy(entry -> name, pre_name);
	// strcpy(entry -> type, " \0");
	// entry -> number = pointer -> d_num + 1;
	// entry -> length = ENTRY_SIZE;
	// pointer -> b_num = 0;
	// writeEntry(entry, pointer);
	// updateFAT(pointer -> d_num + 1, pointer -> d_num + 2);
	return 1;
	// entry = new Entry();
	// strcpy(entry -> name, pre_name);
	// strcpy(entry -> type, "  ");
	// strcpy(entry -> number, pre_name);

	

	// if(entry -> number > 0 && entry -> number < 128){
	// 	pointer -> d_num = entry -> number;
	// 	pointer -> b_num = 0;
	// 	// checkExistFile(oftle, pointer);
	// }
	// delete entry;
}




Pointer * searchEntry(char *name, int d_num) {
	Entry *entry;
	Pointer *pointer = new Pointer();
	pointer -> d_num = d_num;
	for (int i = 0; i < MAX_ENTRY_NUM; i++) {
		pointer -> b_num = i * ENTRY_SIZE;
		entry = readEntry(pointer);
		if(!compareName(name, entry -> name)) {
			pointer -> d_num = entry -> number;
			return pointer;
		}
	}
	return NULL;
}

Pointer * cd(char *path) {
	if (!path) return NULL;
	int d_num = 2;
	Pointer *pointer = new Pointer();
	if (path[0] != '/')
		d_num = current_path -> d_num;
	char *name = strtok(path, "/");
	while (name) {
		pointer = searchEntry(name, d_num);
		if(!pointer) return NULL;
		// printf("d_num %d\n", pointer -> d_num);
		d_num = pointer -> d_num;
		name = strtok(NULL, "/");
	}
	current_path = pointer;
	return pointer;
}


int main() {
	// initDisk();
	// initIndex();
	current_path = new Pointer();
	current_path -> d_num = 2;
	current_path -> b_num = 0;
	char path[] = "/rot";
	//int a = mkdir(path);
	// printf("%d\n", a);
	if (cd(path)) puts("yes");
	printf("%d\n", current_path -> d_num);
	printEntry(3, 0);
	// OFTLE *oftle = new OFTLE();
	// strcpy(oftle -> name, "wjh\0");
	// oftle -> attribute = '0';
	// cout << checkExistFile()
	return 0;
}
