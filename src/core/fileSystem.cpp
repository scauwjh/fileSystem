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

int fileSystem::mkdir(char *path, int type) {
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
            entry.length = 0;
            entry.attribute = type;
            int flag = 0;
            entry.number = i;
            Pointer pointer;
            pointer.d_num = current_point.d_num;
            pointer.b_num = j * 8;
            writeEntry(&entry, &pointer);
            this -> current_point = pre;
            // printf("mkdir at %d block %d byte\n", current_point.d_num, j * 8);
            setFAT(current_point.d_num, -1);
            if(type == 1)
                setFAT(i, -1);
            return i;
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
            current_path += "/";
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
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    // Save the current color
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;
    for (int i = 0; i < MAX_ENTRY_NUM; i++) {
        pointer -> b_num = i * ENTRY_SIZE;
        if (readEntry(entry, pointer) < 0) continue;
        // Set the new color
        if(entry->attribute == 1)
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%s ", entry -> name);
        // Restore the original color
        SetConsoleTextAttribute(h, wOldColorAttrs);
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

int fileSystem::mkfile(char *path) {
    return mkdir(path, 1);

//    string file_name = util.getFileName(path);
//    string parent_path = util.getParentPath(path);
//    if(parent_path == "")
//    cout << file_name << ":" << parent_path << endl;
//    Pointer prev_point = this->current_point;
//    setPosition(parent_path);
//    Entry file, *f = &file;
//    f->attribute = 1;
//    f->length = 0;
//    strcpy(f->name, file_name);
//    strcpy(f->type, " \0");
//    setFAT(current_point.d_num, -1);
}

int fileSystem::edit(char *path) {
    string file_name = util.getFileName(path);
    string parent_path = util.getParentPath(path);
    Pointer prev_point = this->current_point;
    if(parent_path != ""){
        setPosition((char *)parent_path.c_str());
    }
    Entry entry, *e = &entry;
    Pointer *p = &this->current_point;
    int flag = 0;
    for (int i = 0; i < MAX_ENTRY_NUM; i++) {
        p->b_num = i * ENTRY_SIZE;
        readEntry(e, p);
        e->name[ENTRY_NAME] = '\0';
        if (!strcmp(e->name, file_name.c_str())) {
            setFAT(current_point.d_num, -1);
            fstream buf;
            string content;
            if (e->length > 0) {
                content = readFile(e->length, e->number * DISK_BLOCK_SIZE);
                buf.open("resource/buffer", ios::in | ios::out | ios::binary);
                buf.write(content.c_str(), e->length);
                buf.close();
            }
            string com = "notepad resource/buffer";
            system(com.c_str());
            buf.open("resource/buffer", ios::in | ios::out | ios::binary);
            char ch;
            content = "";
            while(buf.peek() != EOF) {
                buf.read((char *) &ch, sizeof(ch));
                content += ch;
            }
            buf.close();
            buf.open("resource/buffer", ios::out | ios_base::trunc);
            buf.close();
            char *tmp = (char *)content.c_str();
            int cnt = 0;
            while(*tmp++) {
                cnt ++;
            }
            int start = e->number * DISK_BLOCK_SIZE;
            int length = (int)e->length;
            clearDisk(length, start);
            e->length = cnt;
            writeEntry(e, p);
            writeFile(content, cnt, start);
            //cout << readFile(cnt, start) << endl;
            flag = 1;
            break;
        }
    }
    this->current_point = prev_point;
    return flag;
}

int fileSystem::cat(char *path) {
    string file_name = util.getFileName(path);
    string parent_path = util.getParentPath(path);
    Pointer prev_point = this->current_point;
    if(parent_path != ""){
        setPosition((char *)parent_path.c_str());
    }
    Entry entry, *e = &entry;
    Pointer *p = &this->current_point;
    int flag = 0;
    for (int i = 0; i < MAX_ENTRY_NUM; i++) {
        p->b_num = i * ENTRY_SIZE;
        readEntry(e, p);
        e->name[ENTRY_NAME] = '\0';
        if (!strcmp(e->name, file_name.c_str()) && e->attribute == 1) {
            int length = e->length;
            int start = e->number * DISK_BLOCK_SIZE;
            string content = readFile(length, start);
            cout << content << endl;
            flag = 1;
            break;
        }
    }
    this->current_point = prev_point;
    return flag;
}
