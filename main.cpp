#include "src/core/fileSystem.h"
#include "src/util/util.h"
#include "windows.h"

fileSystem fsm;
Util util;
void initDisk() {
	char disk[MAX_BLOCK_NUM * DISK_BLOCK_SIZE];
	memset(disk, 0, sizeof(disk));
	fstream fs, *disk_pointer = &fs;
	if(util.openDisk(disk_pointer) < 0) return;
	disk_pointer -> seekg(0);
	disk_pointer -> write((char *) &disk, sizeof(disk));
	disk_pointer -> seekg(0);
	fsm.setFAT(0, -1);
	fsm.setFAT(1, -1);
	fsm.setFAT(2, -1);
	disk_pointer -> close();
	delete disk_pointer;
}

void initIndex() {
	char name[3][5] = {"rot\0","hom\0","bin\0"};
	for (int i = 0; i < 3; i++){
		Entry *entry = new Entry();
		strcpy(entry -> name, name[i]);
		strcpy(entry -> type, " \0");
		entry -> number = i + 3;
		entry -> length = 8;
		entry -> attribute = 0;
		Pointer *pointer = new Pointer();
		pointer -> d_num = 2;
		pointer -> b_num = i * 8;
		fsm.writeEntry(entry, pointer);
		printf("write index: %s and size:%d\n",
			entry -> name, sizeof(entry -> name));
		delete entry;
		delete pointer;
	}

	Entry *entry = new Entry();
	strcpy(entry -> name, "wjh\0");
	strcpy(entry -> type, " \0");
	entry -> number = 6;
	entry -> length = 8;
	entry -> attribute = 0;
	Pointer *pointer = new Pointer();
	pointer -> d_num = 3;
	pointer -> b_num = 0;
	fsm.writeEntry(entry, pointer);
	fsm.setFAT(3, -1);
	printf("write index: %s and size:%d\n",
		entry -> name, sizeof(entry -> name));
	delete entry;
	delete pointer;
}

void printEntry(int d_num, int b_num) {
	// read a entry for test
	fstream fs, *disk_pointer = &fs;
	disk_pointer -> seekg(DISK_BLOCK_SIZE * 2);
	Pointer p;
	p.d_num = d_num;
	p.b_num = b_num;
	Pointer *pointer = &p;
	Entry *entry = new Entry();
	fsm.readEntry(entry, pointer);
	char name[FILE_NAME_SIZE];
	strcpy(name, entry -> name);
    printf("entry:%s and number:%d\n", name, entry -> number);
	disk_pointer -> close();
	delete disk_pointer;

	// check fat table
	fsm.updateFAT();
	for (int i = 0; i < 128; i++)
		printf("%d ", fsm.fat[i]);
	puts("");

	// compareName test
	// char a[] = "wjh\0", b[] = "wjh\0";
	// printf("%d\n", compareName(a, b));
}

void test() {
    fileSystem fs;
//    printf("%d\n",fs.current_path.d_num);
    initDisk();
    initIndex();
    char path1[] = "/rot";
    char path2[] = "/rot/wjh";
//    fs.cd(path) >0 ? puts("cd yes") : puts("cd no");
//    fs.mkdir(path) > 0 ? puts("mkdir yes") : puts("mkdir no");
//    printEntry(6, 8);
    fs.ls();
    fs.cd(path1);
    fs.ls();
    fs.cd(path2);
    fs.ls();
    char mk[] = "/rot/wjh/t2";
    fs.mkdir(mk, 0);
    fs.ls();
}

void run() {
    //test();
    fileSystem fs;
    char comment[100];
    char current_point[10];
    Entry e, *entry = &e;
    Pointer p, *pointer = &p;
    p.b_num = 0;
    while(1) {
        if (fs.current_point.d_num == 2) strcpy(current_point, "/\0");
        else {
            strcpy(current_point, "\0");
        }
        printf("[root@127.0.0.1 %s]# ", fs.current_folder.c_str());
        gets(comment);
        //printf("comment:%s\n",comment);
        char *com = strtok(comment, " ");
        while (com) {
            if (!strcmp(com, "logout\0")) {puts("logout!");Sleep(1000);exit(0);}
            else if (!strcmp(com, "ls\0")) {fs.ls();break;}
            else if (!strcmp(com, "cls\0")) {system("cls");break;}
            else if (!strcmp(com, "cd\0")) {
                com = strtok(NULL, " ");
                if (fs.cd(com) < 0) puts("no such directory");
                break;
            }
            else if (!strcmp(com, "mkdir\0")) {
                com = strtok(NULL, " ");
                if (fs.mkdir(com, 0) < 0) puts("failed to mkdir");
                break;
            }
            else if (!strcmp(com, "rm\0")) {
                com = strtok(NULL, " ");
                if (fs.rm(com) < 0) puts("no such directory");
                break;
            }
            else if (!strcmp(com, "mkfile\0")) {
                com = strtok(NULL, " ");
                if (fs.mkfile(com) < 0) puts("failed to mkfile");
                break;
            }
            else if (!strcmp(com, "edit\0")) {
                com = strtok(NULL, " ");
                if (fs.edit(com) < 0) puts("failed to edit");
                break;
            }
            else if (!strcmp(com, "cat\0")) {
                com = strtok(NULL, " ");
                if (fs.cat(com) < 0) puts("failed to cat");
                break;
            }
            else if(!strcmp(com, "showfat\0")) {
                for (int i = 0; i < 128; i++)
                printf("%d ", fs.fat[i]);
            }
            else if(!strcmp(com, "showpath\0")) {
                cout<< fs.current_path << endl;
            }
            // printf("%s\n", com);
            com = strtok(NULL, com);
        }
    }
}


int main() {
    run();
    return 0;
}
