#include "fileSystem.h"

void initDisk() {
	memset(fat, 0, sizeof(fat));
	memset(disk, 0, sizeof(disk));
	fat[0] = fat[1] = fat[2] = -1;
	fstream disk_pointer("disk", ios::in | ios::out | ios::binary);
	if (!disk_pointer) {
		cerr << "Format disk error!" << endl;
		abort();
	}
	disk_pointer.seekg(0);
	disk_pointer.write((char *) &disk, sizeof(disk));
	disk_pointer.seekg(0);
	disk_pointer.write((char *) &fat, sizeof(fat));
	disk_pointer.close();
}


void writeEntry(Entry *entry, Pointer *pointer) {
	fstream disk_pointer("disk", ios::in | ios::out | ios::binary);
	if (!disk_pointer) {
		cerr << "Read disk error!" << endl;
		return;
	}
	disk_pointer.seekg(pointer -> d_num * DISK_BLOCK_SIZE + pointer -> b_num);
	disk_pointer.write((char *) &entry -> name, sizeof(entry -> name));
	disk_pointer.write((char *) &entry -> type, sizeof(entry -> type));
	disk_pointer.write((char *) &entry -> attribute, sizeof(entry -> attribute));
	disk_pointer.write((char *) &entry -> number, sizeof(entry -> number));
	disk_pointer.write((char *) &entry -> length, sizeof(entry -> length));
	disk_pointer.close();
}

void initIndex() {
	char name[3][4] = {{'/','\0'},{'u','s','r','\0'},{'w','j','h','\0'}};
	for (int i = 0; i < 3; i++){
		Entry entry;
		strcpy(entry.name, name[i]);
		printf("size:%d\n", sizeof(entry.name));
		printf("char:%s\n", entry.name);
		strcpy(entry.type, "  ");
		entry.number = 3;
		entry.length = 0;
		entry.attribute = 0;
		Pointer pointer;
		pointer.d_num = (i ? 3 : 2);
		pointer.b_num = (i ? (i - 1) * 8 : 0);
		writeEntry((Entry *)&entry, (Pointer *)&pointer);
	}
}


Entry * readEntry(Pointer *pointer) {
	Entry entry;
	fstream disk_pointer("disk", ios::in | ios::out | ios::binary);
	if (!disk_pointer) {
		cerr << "Read disk error!" << endl;
		return NULL;
	}
	disk_pointer.seekg(pointer -> d_num * DISK_BLOCK_SIZE + pointer -> b_num);
	disk_pointer.read((char *) &entry.name, sizeof(entry.name));
	disk_pointer.read((char *) &entry.type, sizeof(entry.type));
	disk_pointer.read((char *) &entry.attribute, sizeof(entry.attribute));
	disk_pointer.read((char *) &entry.number, sizeof(entry.number));
	disk_pointer.read((char *) &entry.length, sizeof(entry.length));
	disk_pointer.close();
	return (Entry *) &entry;
}

char * getFileName(char *path, int length) {
	char ch = path[0];
	char name[FILE_NAME_SIZE];
	if(ch != '/')
		return NULL;
	for(int i = 1, j = 0; i < length; i++) {
		ch = path[i];
		if (j >= FILE_NAME_SIZE)
			j = 0;
		name[j++] = ch;
		if (ch == '/')
			j = 0;
	}
	return (char *) name;
}

int checkExistFile(OFTLE *oftle, Pointer *pointer) {
	Entry *entry = readEntry(pointer);
	if (!entry) {
		return -1;
	}
	char *name = getFileName(oftle -> name, sizeof(oftle -> name));
	if (!strcmp(name, entry -> name)) {
		return 1;
	}
	if(entry -> number > 0 && entry -> number < 128){
		pointer -> d_num = entry -> number;
		pointer -> b_num = 0;
		checkExistFile(oftle, pointer);
	}
	return 0;
}

void updateFile(OFTLE *oftle) {

}

int main() {
	initDisk();
	initIndex();
	strcpy(path, "/usr/tmp");

	fstream disk_pointer("disk", ios::in | ios::out | ios::binary);
	if (!disk_pointer) {
		cerr << "Read disk error!" << endl;
		abort();
	}
	disk_pointer.read((char *) &fat, sizeof(fat));// get fat table

	disk_pointer.seekg(DISK_BLOCK_SIZE * 2);

	Pointer p;
	p.d_num = 3;
	p.b_num = 8;
	Pointer *pointer = &p;
	Entry *entry = readEntry(pointer);
	// cout << entry -> name[0] << endl;
	char name[3];
	strcpy(name, entry -> name);
    printf("%s", name);
	disk_pointer.close();

	// TEST
	// char *name = getFileName(path, sizeof(path));
	// if (name != NULL) {
	// 	for(int i = 0; i < FILE_NAME_SIZE; i++){
	// 		putchar(name[i]);
	// 	}
	// 	puts("");
	// }
	return 0;
}
