#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

//·ûºÅ±í¶¨Òå
typedef  struct {
	char name[100];
//	int link;
	int obj;
	int types;
	int ref;
	int normal;
	int lev;
	int adr;
}Tab;


typedef  struct{
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
	int f,x,y,r;
}Order;


typedef struct {
	int elety;
	int elenum;
}element;

#endif // TABLE_H_INCLUDED
