#ifndef VAR_H_INCLUDED
#define VAR_H_INCLUDED

#include "define.h"
#include "table.h"
extern int sy;
extern char token[MAX];
extern int token_top;
extern int num;
extern int cc,ll,lastcc;
extern int lc,lastlc;
extern int errpos;
extern char key[MAX][MAX];
extern char ksy[MAX][MAX];
extern char Dset[MAX][MAX];
extern int Tvar,Lab;



Tab tab[tmax];
Btab btab[bmax];
Atab atab[amax];
Stab stab[smax];
Order Dcode[dmax];
int t,b,a,c2,plc;//符号表索引变量

extern char tarcode[10000][MAX];
extern int tarc;
extern int tarb;

#endif // VAR_H_INCLUDED
