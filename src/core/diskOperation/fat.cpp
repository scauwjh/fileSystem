#ifndef __FAT_CPP__
#define __FAT_CPP__

/**
 * get fat table
 * save to global variable fat[MAX_BLOCK_NUM]
 */
char * getFAT() {
	fstream *disk_pointer = new fstream();
	if(openDisk(disk_pointer) <= 0){
		delete disk_pointer;
		return NULL;
	}
	disk_pointer -> read((char *) &fat, MAX_BLOCK_NUM);
	delete disk_pointer;
	return fat;
}

/**
 * next_num is the d_num of block for the entry
 */
int updateFAT(int d_num, char next_num) {
	char *fat = getFAT();
	fat[d_num] = next_num;
	fstream *disk_pointer = new fstream();
	if(openDisk(disk_pointer) <= 0) {
		delete disk_pointer;
		return -1;
	}
	disk_pointer -> seekg(d_num);
	disk_pointer -> write((char *) &next_num, 1);
	disk_pointer -> close();
	delete disk_pointer;
	return 1;
}

#endif