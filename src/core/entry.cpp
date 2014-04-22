#include "fileSystem.h"

int fileSystem::writeEntry(Entry *entry, Pointer *pointer) {
	fstream fs, *disk_pointer = &fs;
	if(util.openDisk(disk_pointer) < 0) return -1;
	disk_pointer -> seekg(pointer -> d_num * DISK_BLOCK_SIZE + pointer -> b_num);
	disk_pointer -> write((char *) &entry -> name, ENTRY_NAME);
	disk_pointer -> write((char *) &entry -> type, ENTRY_TYPE);
	disk_pointer -> write((char *) &entry -> attribute, ENTRY_ATTRIBUTE);
	disk_pointer -> write((char *) &entry -> number, ENTRY_NUMBER);
	disk_pointer -> write((char *) &entry -> length, ENTRY_LENGTH);
	disk_pointer -> close();
	return 1;
}

int fileSystem::writeFile(string content, int length, int start) {
	fstream fs, *disk_pointer = &fs;
	if (util.openDisk(disk_pointer) < 0) return -1;
	disk_pointer -> seekg(start);
	if (length == 0) {
        disk_pointer -> close();
        return 1;
	}
    disk_pointer -> write(content.c_str(), length+1);
	disk_pointer -> close();
	return 1;
}

string fileSystem::readFile(int length, int start) {
	fstream fs, *disk_pointer = &fs;
	char tmp[300];
	string content;
	if(util.openDisk(disk_pointer) < 0) return "";
	disk_pointer -> seekg(start);
	disk_pointer -> read(tmp, length+1);
	disk_pointer -> close();
	content = tmp;
	return content;
}


int fileSystem::readEntry(Entry *entry, Pointer *pointer) {
	fstream fs, *disk_pointer = &fs;
	if(util.openDisk(disk_pointer) < 0) return -1;
	disk_pointer -> seekg(pointer -> d_num * DISK_BLOCK_SIZE + pointer -> b_num);
	disk_pointer -> read((char *) &entry -> name, ENTRY_NAME);
	disk_pointer -> read((char *) &entry -> type, ENTRY_TYPE);
	disk_pointer -> read((char *) &entry -> attribute, ENTRY_ATTRIBUTE);
	disk_pointer -> read((char *) &entry -> number, ENTRY_NUMBER);
	disk_pointer -> read((char *) &entry -> length, ENTRY_LENGTH);
	disk_pointer -> close();
	if (entry->name[0] == NULL) return -1;
	return 1;
}

int fileSystem::searchEntry(char *name, int d_num) {
	Pointer p, *pointer = &p;
	Entry e, *entry = &e;
	pointer -> d_num = d_num;
	for (int i = 0; i < MAX_ENTRY_NUM; i++) {
		pointer -> b_num = i * ENTRY_SIZE;
		readEntry(entry, pointer);
		if(!util.compareName(name, entry -> name)) {
			return entry -> number;
		}
	}
	return -1;
}

int fileSystem::writeCheck(int d_num, int b_num) {
    fstream fs, *fr = &fs;
    if (util.openDisk(fr) < 0) return -1;
    char ch;
    fr -> seekg(d_num * DISK_BLOCK_SIZE + b_num);
    fr -> read((char *) &ch, 1);
    fr->close();
    //printf("ch:%d\n", ch);
    if(!ch) return 1;
    return -1;
}

int fileSystem::clearEntry(Pointer *pointer) {
    fstream fs, *disk_pointer = &fs;
	if(util.openDisk(disk_pointer) < 0) return -1;
	disk_pointer -> seekg(pointer -> d_num * DISK_BLOCK_SIZE + pointer -> b_num);
	char clear = 0;
	disk_pointer->write((char *) clear, ENTRY_SIZE);
	disk_pointer->close();
	return 1;
}

int fileSystem::clearDisk(int length, int start) {
    fstream fs, *disk_pointer = &fs;
	if(util.openDisk(disk_pointer) < 0) return -1;
	char clear[length+1];
	for (int i = 0; i < length+1; i++)
        clear[i] = 0;
    disk_pointer -> seekg(start);
	disk_pointer->write((char *) clear, length);
	disk_pointer->close();
	return 1;
}

