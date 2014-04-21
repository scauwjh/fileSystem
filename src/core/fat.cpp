#include "fileSystem.h"
/**
 * get fat table
 * save to global variable fat[MAX_BLOCK_NUM]
 */
void fileSystem::updateFAT() {
	fstream fs, *disk_pointer = &fs;
	if(util.openDisk(disk_pointer) < 0) return;
	disk_pointer -> read((char *) &fat, MAX_BLOCK_NUM);
	disk_pointer -> close();
}

/**
 * next_num is the d_num of block for the entry
 */
int fileSystem::setFAT(int d_num, char next_num) {
	updateFAT();
	fat[d_num] = next_num;
	fstream fs, *disk_pointer = &fs;
	if(util.openDisk(disk_pointer) <= 0) return -1;
	disk_pointer -> seekg(d_num);
	disk_pointer -> write((char *) &next_num, 1);
	disk_pointer -> close();
	return 1;
}
