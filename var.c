#include "define.h"

int sy;//word ������
char token[MAX];
int token_top;
int num;//������ַ�������תΪint
int cc,ll,lastcc;
int lc=-1,lastlc;//program line;
int errpos;
char key[MAX][MAX] ={"const","int","char","void","main","if","while","switch","case","default","scanf","printf","return"};
char ksy[MAX][MAX] ={"constsy","intsy","charsy","voidsy","mainsy","ifsy","whilesy","switchsy","casesy","defaultsy","scanfsy","printfsy","returnsy"};
