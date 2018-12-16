#include "define.h"

int sy;//word 的类型
char token[MAX];
int token_top;
int num;//读入的字符串数字转为int
int cc,ll,lastcc;
int lc=-1,lastlc;//program line;
int errpos;
char key[MAX][MAX] ={"const","int","char","void","main","if","while","switch","case","default","scanf","printf","return"};
char ksy[MAX][MAX] ={"constsy","intsy","charsy","voidsy","mainsy","ifsy","whilesy","switchsy","casesy","defaultsy","scanfsy","printfsy","returnsy"};
char Dset[MAX][MAX]= {"var","funcdec","set","jump","ret","add","sub","mult","div","[]=","[]","callret","call","npara","<=","<",">=",">","==","!=","bnz","bne","scanf","printf","=","pull"};
int Tvar,Lab;

char tarcode[100000][MAX];
int tarc;
int tarb;
