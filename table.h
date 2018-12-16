#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include "define.h"
//·ûºÅ±í¶¨Òå
typedef  struct {
	char name[MAX];
//	int link;
	int obj;
	int types;
	int ref;
	int normal;
	int lev;
	int adr;
}Tab;


typedef  struct{
    char name[MAX];
    int first;
	int last;
	int lastpar;
	int ref;
	int psize;
	int vsize;
}Btab;


typedef  struct {
	int low,high;
	int elsize,size;
}Atab;


typedef 	struct{
	char string[1000];
}Stab;


typedef 	struct{
    char F[MAX];
    int f;
    char Left[MAX];
    int Ltyp;
    char Right[MAX];
    int Rtyp;
    char Result[MAX];
    int Retyp;
}Order;


typedef struct {
    char name[MAX];
	int elety;
	int elenum;
}element;

#endif // TABLE_H_INCLUDED
