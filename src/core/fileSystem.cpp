#include "fileSystem.h"
#include "../util/util.h"
#include "../debug/debug.h"
#include "diskOperation/entry.cpp"
#include "diskOperation/fat.cpp"

int mkdir(char * a_path) {
	Pointer *pointer = new Pointer();
	pointer -> d_num = 2;
	pointer -> b_num = 0;
	Entry *entry = readEntry(pointer);
	char *name = strtok(a_path, "/");
	char pre_name[3];
	int count = 0;
	while (name) {
		if (strlen(name) > 3)
			return -1;
		count ++;
		int flag = 0;
		for (int i = 0; i < MAX_ENTRY_NUM; i++) {
			if (!compareName(name, entry -> name)) {
				flag = 1;
				break;
			}
			pointer -> b_num = (i + 1) * 8;
			entry = readEntry(pointer);
			if (!entry) return -1;
		}
		pointer -> d_num = entry -> number;
		for(int i = 0; i < FILE_NAME_SIZE; i++)
			pre_name[i] = name[i];
		name = strtok(NULL, "/");
		if (!flag && name)
			return -1;
	}
	if (count <= 1)
		return -1;
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


int main() {
	// initDisk();
	// initIndex();
	// debug();
	char path[] = "/rot/wjh";
	int a = mkdir(path);
	printf("%d\n", a);
	// OFTLE *oftle = new OFTLE();
	// strcpy(oftle -> name, "wjh\0");
	// oftle -> attribute = '0';
	// cout << checkExistFile()
	return 0;
}
