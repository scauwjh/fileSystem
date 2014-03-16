#ifndef __DEBUG_H__
#define __DEBUG_H__

extern void writeEntry(Entry *entry, Pointer *pointer);
extern int openDisk(fstream *disk_pointer);
extern Entry * readEntry(Pointer *pointer);
extern char * getFAT();
extern int updateFAT(int d_num, char next_num);

void initDisk() {
	char disk[MAX_BLOCK_NUM * DISK_BLOCK_SIZE];
	memset(fat, 0, sizeof(fat));
	memset(disk, 0, sizeof(disk));
	fat[0] = fat[1] = fat[2] = -1;
	fstream *disk_pointer = new fstream();
	if(openDisk(disk_pointer) <= 0) return;
	disk_pointer -> seekg(0);
	disk_pointer -> write((char *) &disk, sizeof(disk));
	disk_pointer -> seekg(0);
	disk_pointer -> write((char *) &fat, sizeof(fat));
	disk_pointer -> close();
	delete disk_pointer;
}

void initIndex() {
	char name[3][5] = {"rot\0","hom\0","bin\0"};
	for (int i = 0; i < 3; i++){
		Entry *entry = new Entry();
		strcpy(entry -> name, name[i]);
		strcpy(entry -> type, "  ");
		entry -> number = i + 3;
		updateFAT(i + 3, i + 4);
		entry -> length = 0;
		entry -> attribute = 0;
		Pointer *pointer = new Pointer();
		pointer -> d_num = 2;
		pointer -> b_num = i * 8;
		writeEntry(entry, pointer);
		printf("write index: %s and size:%d\n", 
			entry -> name, sizeof(entry -> name));
		delete entry;
		delete pointer;
	}
}

void debug() {
	// read a entry for test
	fstream *disk_pointer = new fstream();
	disk_pointer -> seekg(DISK_BLOCK_SIZE * 2);
	Pointer p;
	p.d_num = 2;
	p.b_num = 16;
	Pointer *pointer = &p;
	Entry *entry = readEntry(pointer);
	char name[FILE_NAME_SIZE];
	strcpy(name, entry -> name);
    printf("%s\n", name);
	disk_pointer -> close();
	delete disk_pointer;

	// check fat table
	char *fat = getFAT();
	for (int i = 0; i < 128; i++)
		printf("%d ", fat[i]);
	puts("");

	// compareName test
	// char a[] = "wjh\0", b[] = "wjh\0";
	// printf("%d\n", compareName(a, b));
}

#endif