#ifndef __ENTRY_CPP__
#define __ENTRY_CPP__


int writeEntry(Entry *entry, Pointer *pointer) {
	char *fat = getFAT();
	if (fat[pointer -> d_num]) return -1;
	fstream *disk_pointer = new fstream();
	if(openDisk(disk_pointer) <= 0) {
		delete disk_pointer;
		return -1;
	}
	disk_pointer -> seekg(pointer -> d_num * DISK_BLOCK_SIZE + pointer -> b_num);
	disk_pointer -> write((char *) &entry -> name, ENTRY_NAME);
	disk_pointer -> write((char *) &entry -> type, ENTRY_TYPE);
	disk_pointer -> write((char *) &entry -> attribute, ENTRY_ATTRIBUTE);
	disk_pointer -> write((char *) &entry -> number, ENTRY_NUMBER);
	disk_pointer -> write((char *) &entry -> length, ENTRY_LENGTH);
	disk_pointer -> close();
	delete disk_pointer;
	return 1;
}


Entry * readEntry(Pointer *pointer) {
	fstream *disk_pointer = new fstream();
	if(openDisk(disk_pointer) <= 0) {
		delete disk_pointer;
		return NULL;
	}
	Entry *entry = new Entry();
	disk_pointer -> seekg(pointer -> d_num * DISK_BLOCK_SIZE + pointer -> b_num);
	disk_pointer -> read((char *) &entry -> name, ENTRY_NAME);
	disk_pointer -> read((char *) &entry -> type, ENTRY_TYPE);
	disk_pointer -> read((char *) &entry -> attribute, ENTRY_ATTRIBUTE);
	disk_pointer -> read((char *) &entry -> number, ENTRY_NUMBER);
	disk_pointer -> read((char *) &entry -> length, ENTRY_LENGTH);
	disk_pointer -> close();
	delete disk_pointer;
	return entry;
}

#endif