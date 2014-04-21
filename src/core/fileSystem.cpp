#include "fileSystem.h"
#include "../util/util.h"

fileSystem::fileSystem(){
    this->current_point.d_num = 2;
    this->current_point.b_num = 0;
    this->current_path = "/";
    this->current_folder = "/";
    updateFAT();
}

void fileSystem::setCurretnPath(int d_num, int b_num) {
    this->current_point.d_num = d_num;
    this->current_point.b_num = b_num;
}

int fileSystem::mkdir(char *path) {
    const char *name = util.getFileName(path).c_str();
    int cnt = util.getFolderCount(path);
//    printf("%d %s\n", cnt, name);
    Pointer pre = this -> current_point;
    int setP = setPosition(path);
    if(-setP != cnt) {
        this -> current_point = pre;
        return -1;
    }
    int i, j, flag = 0;
    for (i = current_point.d_num + 1; i < MAX_BLOCK_NUM; i++)
        if (!fat[i]) {
            flag = 1;
            break;
        }
    if (flag) {
        for (j = 0; j < MAX_ENTRY_NUM; j++)
            if (writeCheck(current_point.d_num, j * 8) > 0) {
                flag = 0;
                break;
            }
        if (!flag) {
            Entry entry;
            strcpy(entry.name, name);
            strcpy(entry.type, " \0");
            entry.length = 8;
            entry.attribute = 1;
            int flag = 0;
            entry.number = i;
            Pointer pointer;
            pointer.d_num = current_point.d_num;
            pointer.b_num = j * 8;
            writeEntry(&entry, &pointer);
            this -> current_point = pre;
            // printf("mkdir at %d block %d byte\n", current_point.d_num, j * 8);
            setFAT(current_point.d_num, -1);
            return 1;
        }
    }
    this -> current_point = pre;
    return -1;
}

int fileSystem::setPosition(char *path) {
	if (!path) return -1;
    char cpy[1000];
    strcpy(cpy, path);
	int d_num = 2;
	if (cpy[0] != '/')
		d_num = this -> current_point.d_num;
	char *name = strtok(cpy, "/");
	int cnt = 0, tmp_dnum;
	before_last_folder.d_num = d_num;
	before_last_folder.b_num = 0;
	while (name) {
        cnt++;
        tmp_dnum = searchEntry(name, d_num);
		if (tmp_dnum < 0) return -cnt;
		setCurretnPath(tmp_dnum, 0);
		d_num = tmp_dnum;
		name = strtok(NULL, "/");
		if (name) before_last_folder.d_num = d_num;
	}
	setCurretnPath(tmp_dnum, 0);
	return cnt;
}

int fileSystem::cd(const char *path) {
    if(!strcmp(path, "..")) {
        current_path = util.getParentPath(current_path.c_str());
        cout << current_path << endl;
        return cd(current_path.c_str());
    }
    if (!strcmp(path, "/")) {
        current_point.d_num = 2;
        current_point.b_num = 0;
        current_path = "/";
        current_folder = "/";
        return 1;
    }
    Pointer pre = this -> current_point;
    int ret = setPosition((char *)path);
    if(ret < 0) current_point = pre;
    else {
        const char *tmp = path;
        int cnt = 0;
        while(*tmp++) {
            cnt ++;
        }
        //cout << cnt << endl;
        if(cnt <= 3) {
            current_path += path;
            current_folder = path;
        }
        else {
            current_path = path;
            current_folder = util.getFileName((char *)path);
        }
        if(!util.endWith(current_path, "/"))
            current_path += "/";

    }
    return ret;
}

void fileSystem::ls() {
    Entry e, *entry = &e;
    Pointer p, *pointer = &p;
    pointer -> d_num = current_point.d_num;
    for (int i = 0; i < MAX_ENTRY_NUM; i++) {
        pointer -> b_num = i * 8;
        if (readEntry(entry, pointer) < 0) continue;
        printf("%s ", entry -> name);
    }
    puts("");
}

int fileSystem::rm(char *path) {
    string fileName = util.getFileName(path);
    Pointer prev_path = this->current_point;
    if (setPosition(path) < 0) {
       current_point = prev_path;
       return -1;
    }
    Entry e, *entry = &e;
    Pointer *pointer = &before_last_folder;
    for (int i = 0; i < MAX_ENTRY_NUM; i++) {
        pointer->b_num = i * ENTRY_SIZE;
        if (readEntry(entry, pointer) >= 0) {
            entry->name[ENTRY_NAME] = '\0';
            if (strcmp(entry->name, fileName.c_str()) == 0) {
                clearEntry(pointer);
                setFAT(entry->number, 0);
                break;
            }
        }
    }
    current_point = prev_path;
    return 0;
}
