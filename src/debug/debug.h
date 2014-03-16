#ifndef __DEBUG_H__
#define __DEBUG_H__

extern int writeEntry(Entry *entry, Pointer *pointer);
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
	updateFAT(2, 0);
	for (int i = 0; i < 3; i++){
		Entry *entry = new Entry();
		strcpy(entry -> name, name[i]);
		strcpy(entry -> type, " \0");
		entry -> number = i + 3;
		entry -> length = 8;
		entry -> attribute = 1;
		Pointer *pointer = new Pointer();
		pointer -> d_num = 2;
		pointer -> b_num = i * 8;
		writeEntry(entry, pointer);
		printf("write index: %s and size:%d\n", 
			entry -> name, sizeof(entry -> name));
		delete entry;
		delete pointer;
	}
	updateFAT(2, -1);

	Entry *entry = new Entry();
	strcpy(entry -> name, "tes\0");
	strcpy(entry -> type, " \0");
	entry -> number = 7;
	entry -> length = 8;
	entry -> attribute = 1;
	Pointer *pointer = new Pointer();
	pointer -> d_num = 3;
	pointer -> b_num = 0;
	writeEntry(entry, pointer);
	updateFAT(7, 8);
	printf("write index: %s and size:%d\n", 
		entry -> name, sizeof(entry -> name));
	delete entry;
	delete pointer;
}

void printEntry(int d_num, int b_num) {
	// read a entry for test
	fstream *disk_pointer = new fstream();
	disk_pointer -> seekg(DISK_BLOCK_SIZE * 2);
	Pointer p;
	p.d_num = d_num;
	p.b_num = b_num;
	Pointer *pointer = &p;
	Entry *entry = readEntry(pointer);
	char name[FILE_NAME_SIZE];
	strcpy(name, entry -> name);
    printf("entry:%s and number:%d\n", name, entry -> number);
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