#include "util.h"

string Util::getFileName(char *path) {
    string pre = "";
    char cpy[1000];
    strcpy(cpy, path);
	char *name = strtok(cpy, "/");
	while (name) {
        pre = name;
		name = strtok(NULL, "/");
//		printf("%s\n", pre);
	}
	return pre;
}

string Util::getParentPath(const char *path) {
    string parentPath = startWith(path, "/") > 0 ? "/" : "";
    char cpy[1000];
    strcpy(cpy, path);
    int folder_cnt = getFolderCount((char *)path);
    int cnt = 1;
	char *name = strtok(cpy, "/");
	while (name) {
        if(cnt++ > folder_cnt - 1) break;
        parentPath += name;
        parentPath += "/";
		name = strtok(NULL, "/");
	}
	return parentPath;
}

int Util::getFolderCount(char *path) {
    char cpy[1000];
    strcpy(cpy, path);
    char *name = strtok(cpy, "/");
	int cnt = 0;
	while (name) {
        cnt ++;
		name = strtok(NULL, "/");
	}
	return cnt;
}

int Util::openDisk(fstream *disk_pointer) {
	disk_pointer -> open(DISK_PATH, ios::in | ios::out | ios::binary);
	if (!*disk_pointer) {
		cerr << "Open disk error!" << endl;
		return -1;
	}
	return 1;
}

int Util::compareName(char *a, char *b) {
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

void Util::myStrcpy(char *a, char *b, int len) {
	for (int i = 0; i < len; i++) {
		a[i] = b[i];
	}
}

int Util::endWith(const string &a, const string &b) {
    if(a.empty() || b.empty()) return -1;
    return a.compare(a.size() - b.size(), b.size(), b) == 0 ? 1 : -1;
}

int Util::startWith(const string &a, const string &b) {
    if(a.empty() || b.empty()) return false;
    return a.compare(0, b.size(), b) == 0 ? 1 : -1;
}

//int main() {
//    Util u;
//    fstream fs, *disk_pointer = &fs;
//    u.openDisk(disk_pointer);
//}
