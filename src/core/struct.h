#ifndef _POINTER_H__
#define _POINTER_H__

typedef struct {
	int d_num = 2; // disk block num
	int b_num = 0; // byte num
}Pointer;

typedef struct {
	char name[3];
	char type[2];
	char attribute;
	char number;
	char length;
}Entry;

#endif
