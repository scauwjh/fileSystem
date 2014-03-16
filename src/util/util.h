#ifndef __UTIL_H__
#define __UTIL_H__

char * getFileName(char *path) {
	char *name = strtok(path, "/");
	while (name) {
		name = strtok(NULL, "/");
	}
	return name;
}

int openDisk(fstream *disk_pointer) {
	disk_pointer -> open(DISK_PATH, ios::in | ios::out | ios::binary);
	if (!*disk_pointer) {
		cerr << "Read disk error!" << endl;
		return -1;
	}
	return 1;
}

int compareName(char *a, char *b) {
	int ret = 0, i;
	for (i = 0; i < FILE_NAME_SIZE; i++) {
		// printf("a[i]:%c b[i]:%c\n", a[i], b[i]);
		if (ret) return ret;
		if (!a[i] && b[i]) return -b[i];
		if (!b[i] && a[i]) return a[i];
		if (!a[i] && !b[i]) return ret;
		ret = a[i] - b[i];
	}
	return ret;
}

void myStrcpy(char *a, char *b, int len) {
	for (int i = 0; i < len; i++) {
		a[i] = b[i];
	}
}

#endif