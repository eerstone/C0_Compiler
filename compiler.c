#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<string.h>

#include "define.h"
#include "table.h"
#include "var.h"
#include "getsym.h"
#include "target.h"



int skipflag,Funcflag,errorflag;


int types;
char id[MAX];



int  zerobeginsys[MAX],constbeginsys[MAX],varbeginsys[MAX],parameterbeginsys[MAX],typesbeginsys[MAX],programbeginsys[MAX];
int compondstatementbeginsys[MAX],statementbeginsys[MAX];
int funcendsys[MAX],compondstatementendsys[MAX],statementendsys[MAX];
int  Idbeginsys[MAX],semicommabeginsys[MAX];
int factorbeginsys[MAX];
int relationbeginsys[MAX];
int normalparabeginsys[MAX];
int printfbeginsys[MAX],returnbeginsys[MAX];
int Rmatrix[MAX][MAX];//每一维表示只有自己存在的一维集合

void plusbeginsys(int * s1,int * s2,int * s3);
void Error(int n);

void Setup(){
	int i = 0;
	for(i=0;i<MAX;i++){
		for(i=0;i<MAX;i++){
			Rmatrix[i][i]=1;
		}
	}

	factorbeginsys[IdSY]=1;
	factorbeginsys[LPARSY]=1;
	factorbeginsys[PLUSSY]=1;
	factorbeginsys[MINUSSY]=1;
	factorbeginsys[CharSY] =1;
	factorbeginsys[IntSY] =1;

//	normalparabeginsys[IdSY]=1;
//	normalparabeginsys[LPARSY]=1;
//	normalparabeginsys[PLUSSY]=1;
//	normalparabeginsys[MINUSSY]=1;
//	normalparabeginsys[CharSY] =1;
//	normalparabeginsys[IntSY] =1
	normalparabeginsys[RPARSY] =1;
	plusbeginsys(normalparabeginsys,factorbeginsys,normalparabeginsys);

	returnbeginsys[SEMISY]=1;
	plusbeginsys(returnbeginsys,factorbeginsys,returnbeginsys);
//	returnbeginsys[IdSY]=1;
//	returnbeginsys[LPARSY]=1;
//	returnbeginsys[PLUSSY]=1;
//	returnbeginsys[MINUSSY]=1;
//	returnbeginsys[CharSY]=1;
//	returnbeginsys[IntSY]=1;

	printfbeginsys[StringSY] =1;
	plusbeginsys(factorbeginsys,printfbeginsys,printfbeginsys);
//	printfbeginsys[SEMISY] =1;
//	printfbeginsys[LPARSY] =1;
//	printfbeginsys[IdSY] =1;
//	printfbeginsys[PLUSSY] =1;
//	printfbeginsys[MINUSSY] =1;
//	printfbeginsys[CharSY] =1;
//	printfbeginsys[IntSY] =1;


	relationbeginsys[EQUSY]=1;
	relationbeginsys[UNEQUSY]=1;
	relationbeginsys[LITTLESY]=1;
	relationbeginsys[LITTLEEQUSY]=1;
	relationbeginsys[GREATERSY]=1;
	relationbeginsys[GREATEREQUSY]=1;


	varbeginsys[SEMISY] = 1;
	varbeginsys[COMMASY] = 1;


	statementbeginsys[IdSY]=1;
	statementbeginsys[IFSY]=1;
	statementbeginsys[WHILESY]=1;
	statementbeginsys[SCANFSY]=1;
	statementbeginsys[PRINTFSY]=1;
	statementbeginsys[RETURNSY]=1;
	statementbeginsys[SWITCHSY]=1;
	statementbeginsys[LBRACE]=1;
	statementbeginsys[SEMISY]=1;

	compondstatementbeginsys[CONSTSY]=1;
	compondstatementbeginsys[INTSY]=1;
	compondstatementbeginsys[CHARSY]=1;
	plusbeginsys(statementbeginsys,compondstatementbeginsys,compondstatementbeginsys);
//	compondstatementbeginsys[IdSY]=1;
//	compondstatementbeginsys[IFSY]=1;
//	compondstatementbeginsys[WHILESY]=1;
//	compondstatementbeginsys[SCANFSY]=1;
//	compondstatementbeginsys[PRINTFSY]=1;
//	compondstatementbeginsys[RETURNSY]=1;
//	compondstatementbeginsys[SWITCHSY]=1;
//	compondstatementbeginsys[SEMISY]=1;
//	compondstatementbeginsys[LBRACE]=1;
//	compondstatementbeginsys[RBRACE]=1;//复合语句部分不含左花括号

	parameterbeginsys[INTSY]=1;
	parameterbeginsys[CHARSY]=1;
	parameterbeginsys[RPARSY]=1;
	parameterbeginsys[LBRACE]=1;//容错参数无右括号无内容

	typesbeginsys[INTSY]=1;
	typesbeginsys[CHARSY]=1;
	typesbeginsys[VOIDSY]=1;

    semicommabeginsys[SEMISY]=1;
    semicommabeginsys[COMMASY]=1;

    constbeginsys[PLUSSY]=1;
    constbeginsys[MINUSSY]=1;
    constbeginsys[IntSY]=1;
    constbeginsys[CharSY]=1;

    programbeginsys[CONSTSY]=1;
    programbeginsys[INTSY]=1;
    programbeginsys[CHARSY]=1;
    programbeginsys[VOIDSY]=1;
	programbeginsys[EOFSY]=1;

	funcendsys[VOIDSY]=1;
	funcendsys[INTSY]=1;
	funcendsys[CHARSY]=1;


	compondstatementendsys[EOFSY]=1;
	plusbeginsys(funcendsys,compondstatementendsys,compondstatementendsys);

	plusbeginsys(statementbeginsys,compondstatementendsys,statementendsys);


	strcpy(key[COMMASY] , ",");
	strcpy(key[EQUSY] , "==");
	strcpy(key[SEMISY] , ";");
	strcpy(key[ASSIGNSY] , "=");
	strcpy(key[PLUSSY] , "+");
	strcpy(key[MINUSSY] , "-");
	strcpy(key[STARSY] , "*");
	strcpy(key[DIVSY] , "/");
	strcpy(key[UNEQUSY] , "!=");
	strcpy(key[LITTLESY] , "<");
	strcpy(key[LITTLEEQUSY] , "<=");
	strcpy(key[GREATERSY] , ">");
	strcpy(key[GREATEREQUSY] , ">=");
	strcpy(key[LPARSY] , "(");
	strcpy(key[RPARSY] , ")");
	strcpy(key[LBRACK] , "[");
	strcpy(key[RBRACK] , "]");
	strcpy(key[LBRACE] , "{");
	strcpy(key[RBRACE] , "}");
	strcpy(key[COLON] , ":");


	strcpy(ksy[CharSY],"char");
	strcpy(ksy[NoteSY],"note");
	strcpy(ksy[StringSY],"string");
	strcpy(ksy[IdSY] ,"ident");
	strcpy(ksy[IntSY] , "int");
	strcpy(ksy[COMMASY] , "comma");
	strcpy(ksy[EQUSY] , "euq");
	strcpy(ksy[SEMISY] , "semi");
	strcpy(ksy[ASSIGNSY] , "assign");
	strcpy(ksy[PLUSSY] , "plus");
	strcpy(ksy[MINUSSY] , "minus");
	strcpy(ksy[STARSY] , "star");
	strcpy(ksy[DIVSY] , "div");
	strcpy(ksy[UNEQUSY] , "unequ");
	strcpy(ksy[LITTLESY] , "little");
	strcpy(ksy[LITTLEEQUSY] , "littleequ");
	strcpy(ksy[GREATERSY] , "greater");
	strcpy(ksy[GREATEREQUSY] , "greaterequ");
	strcpy(ksy[LPARSY] , "lpar");
	strcpy(ksy[RPARSY] , "rpar");
	strcpy(ksy[LBRACK] , "lbrack");
	strcpy(ksy[RBRACK] , "rbrack");
	strcpy(ksy[LBRACE] , "lbrace");
	strcpy(ksy[RBRACE] , "rbrace");
	strcpy(ksy[COLON] , "colon");
	strcpy(ksy[NOTYPE] , "notype");
	strcpy(ksy[Const] , "Const");
	strcpy(ksy[Variable] , "Variable");
	strcpy(ksy[Withrefunc] , "Withrefunc");
	strcpy(ksy[Norefunc] , "Norefunc");
	strcpy(ksy[Array] , "Array");
	strcpy(ksy[Para] , "Para");
}

char * getTvar(char * tvar){
    char name[MAX];
    char str_int[30];
    itoa(Tvar,str_int,10);
    memset(name,'\0',MAX);
    name[0]='$';
    name[1]='t';
    int i;
    for(i=0;str_int[i]!='\0';i++){
        name[i+2]=str_int[i];
    }
    strcpy(tvar,name);
    Tvar++;
}

void getLabel(char * label){
    char name[MAX];
    char str_int[30];
    memset(name,'\0',MAX);
    strcpy(name,"label");
    itoa(Lab,str_int,10);
    int i;
    for(i=0;str_int[i]!='\0';i++){
        name[i+5]=str_int[i];
    }
    strcpy(label,name);
    Lab++;
}

void fatal(int n){
    Error(n);
}

void enter(char * Id,int obj,int types, int adr){
    int i,j,k;
    if(t == tmax){
        fatal(40);
    }
    else {
        for(i=btab[b].first;i<t;i++){
            if(strcmp(tab[i].name,Id)==0) Error(1);
        }
        if(obj == Withrefunc || obj == Norefunc){
            for(i=0;i<t;i++){
                if(tab[i].obj==Withrefunc || tab[i].obj ==Norefunc){
                    if(strcmp(Id,tab[i].name)==0) Error(1);
                }
            }
        }
        strcpy(tab[t].name,Id);
        tab[t].obj = obj;
        tab[t].types = types;
        if(obj == Variable ||obj ==Array || obj == Para) tab[t].adr = adr * 4;
        else tab[t].adr = adr;
        tab[t].lev=0;
        tab[t].normal=1;
        if(obj == Array) tab[t].ref = a;
        else if(obj == Withrefunc||obj == Norefunc) tab[t].ref = b+1;
        else tab[t].ref =0;
        t++;
    }
}
void enterbtab(){
    if(b == bmax) fatal(40);
    else{
        btab[b].last=t;
        b++;
        btab[b].first = t;
        btab[b].ref = t-1;
        btab[b].psize=0;
        btab[b].vsize=0;
        strcpy(btab[b].name,tab[btab[b].ref].name);
    }
}
void enteratab(int high){
    if(a==amax) fatal(40);
    else{
        atab[a].elsize =4;
        atab[a].high = high;
        atab[a].size = atab[a].elsize*high;
        a++;
    }
}
void enterDcode(int f,char * left,char * right ,char * result){
    Dcode[plc].f =f;
    strcpy(Dcode[plc].F,Dset[f]);
    strcpy(Dcode[plc].Left,left);
    strcpy(Dcode[plc].Right,right);
    strcpy(Dcode[plc].Result,result);
    plc++;
}
void printtable(){
    int i;
    printf("          ************Tab*************            \n");
    printf("    name                obj                 typ                 ref                 adr                 \n");
    for(i= 0;i<t;i++){
        printf("%-4d%-20s%-20s%-20s%-20d%-20d\n",i,tab[i].name,ksy[tab[i].obj],ksy[tab[i].types],tab[i].ref,tab[i].adr);
    }
    printf("\n\n          ************Btab************            \n");
    printf("    name                first               last                ref                 psize               vsize         \n");
    for(i= 0;i<=b;i++){
        printf("%-4d%-20s%-20d%-20d%-20d%-20d%-20d\n",i,btab[i].name,btab[i].first,btab[i].last,btab[i].ref,btab[i].psize,btab[i].vsize);
    }
}
void printDcode(){
    int i;
    printf("     ***********************************DCODE**********************************     \n");
    printf("    Op                  Left                Right               Result              \n");
    for(i = 0; i<plc;i++){
        printf("%-4d%-20s%-20s%-20s%-20s\n",i,Dcode[i].F,Dcode[i].Left,Dcode[i].Right,Dcode[i].Result);
    }
}
void printtarcode(){
    int j;
    for(j =0 ;j<tarc;j++){
        printf("%s",tarcode[j]);
    }
}

int loc(char * Id){
    int i;
    if(b == 0){
        for(i=btab[0].first;i<t;i++){
            if(strcmp(tab[i].name,Id)==0){
                return i;
            }
        }
        return -1;
    }
    else{
        for(i=btab[b].first;i<t;i++){
            if(strcmp(tab[i].name,Id)==0){
                return i;
            }
        }
        for(i = btab[0].first;i<btab[0].last;i++){
            if(strcmp(tab[i].name,Id)==0){
                return i;
            }
        }
        for(i = 0; i<= b;i++){
            if(strcmp(btab[i].name,Id)==0){
                return btab[i].ref;
            }
        }
        return -1;
    }
}
void endskip(){//未设置errpos
    skipflag = 0;
}


void Error(int n){
    if (n>=0) errorflag =1;
	if(cc>errpos){
		errpos = cc +3;
		if (n>=0) printf("Error line%d loc%d:%d\n",lastlc,lastcc,n);
		switch(n){
		    case 0:{
					printf("该标识符未定义\n");
				break;
			}
			case 1:{
					printf("该标识符重复定义\n");
				break;
			}
			case 2:{
					printf("应是标识符\n");
				break;
			}
			case 3:{
					printf("应是右小括号\n");
				break;
			}
			case 4:{
					printf("应是冒号\n");
				break;
			}
			case 5:{
					printf("非法符号，编译将跳读该符号和后面某些符号\n");
				break;
			}
			case 6:{
					printf("形式参数表中，形参说明应以类型说明开头\n");
				break;
			}
			case 7:{
					printf("应是左小括号\n");
				break;
			}
			case 8:{
					printf("应是左中括号\n");
				break;
			}
			case 9:{
					printf("应是右中括号\n");
				break;
			}
			case 10:{
					printf("应是分号\n");
				break;
			}
			case 11:{
					printf("有返回值函数返回值必须是int或者char\n");
				break;
			}
			case 12:{
					printf("数字太大，最多为10位数字\n");
				break;
			}
			case 13:{
					printf("程序结尾应为右花括号，请检查相应的花括号\n");
				break;
			}
			case 14:{
					printf("case后面必须是常量\n");
				break;
			}
			case 15:{
					printf("非法字符\n");
				break;
			}
			case 16:{
					printf("类型不一致\n");
				break;
			}
			case 17:{
					printf("没有这样的数组\n");
				break;
			}
			case 18:{
					printf("应是类型标识符\n");
				break;
			}
			case 19:{
					printf("该算术表达式类型不合法，数组整体不能作为算术或逻辑运算的操作数\n");
				break;
			}
			case 20:{
					printf("实参和对应的形参类型应相同\n");
				break;
			}
			case 21:{
					printf("应是变量\n");
				break;
			}
			case 22:{
					printf("字符串至少有一个字符\n");
				break;
			}
			case 23:{
					printf("实参个数与形参个数不等\n");
				break;
			}
			case 24:{
					printf("scanf的参数类型不正确\n");
				break;
			}
			case 25:{
					printf("表达式中不能出现类型标识符\n");
				break;
			}
			case 26:{
					printf("应是变量或函数标识符\n");
				break;
			}
			case 27:{
					printf("在赋值语句被赋变量应与表达式类型相同\n");
				break;
			}
			case 28:{
					printf("内存溢出\n");
				break;
			}
			case 29:{
					printf("应是常量\n");
				break;
			}
			case 30:{
					printf("因子必须以标识符，整数，字符或’(’开始，或因子为有返回值函数\n");
				break;
			}
			case 31:{
					printf("应是赋值等号\n");
				break;
			}
			case 32:{
					printf("应是左花括号\n");
				break;
			}
			case 33:{
					printf("应是右花括号\n");
				break;
			}
			case 34:{
					printf("应是无符号整数\n");
				break;
			}
			case 35:{
					printf("应是加减运算符\n");
				break;
			}
			case 36:{
					printf("应是表达式\n");
				break;
			}
			case 37:{
					printf("应为case\n");
				break;
			}
			case 38:{
					printf("应为冒号\n");
				break;
			}
			case 39:{
					printf("应为EOF\n");
				break;
			}
			case 40:{
					printf("表溢出\n");
				break;
			}
		}
	}
}

void skip(int* fsys,int n){
    Error(n);
    skipflag = 1;
    while(fsys[sy]!= 1 ){
        getsym();
    }
    if(skipflag==1) endskip();
}


void test(int s1[],int  s2[],int n){
    if(s1[sy]!=1){
        int s3[MAX];
        int i;
        for(i = 0;i<MAX;i++){
            s3[i]=0;
            if(s1[i]==1||s2[i]==1){
                s3[i]=1;
            }
        }
        skip(s3,n);
    }
}

void testsemi(int *fsys){
    if(sy == SEMISY) getsym();
    else{
        if(sy == COLON || sy == COMMASY){
            Error(10);//应为分号
            getsym();
        }
        else test(fsys,zerobeginsys,5);//5号错误表示非法符号，即我也不知道你在这到底写了些啥
    }
}
element constant(int *fsys,element c){
    memset(c.name,'\0',MAX);
    int sign = 1;
    test(constbeginsys,fsys,29);//应是常量
    if(constbeginsys[sy]==1){
        printf("This is a constant ");
        if(sy == CharSY){
            printf(" char: %c\n",token[0]);
            c.name[0] = '\'';
            c.name[2] = '\'';
            c.name[1] = token[0];
            if(types != CHARSY &&types != CharSY) Error(16);
            if(types == CHARSY) enter(id,Const,CharSY,token[0]);
            getsym();
        }
        else {
            if(sy == PLUSSY || sy ==MINUSSY){
                if(sy == MINUSSY) sign = -1;
                getsym();
            }
            if(sy==IntSY) {
                int number;
                number = num *sign;
                printf(" int: %d\n",number);
                itoa(number,c.name,10);
                if(types != INTSY&&types != IntSY) Error(16);
                if(types == INTSY) enter(id,Const,IntSY,number);//仅应在定义常量中出现
                getsym();
            }
            else{
            	test(semicommabeginsys,fsys,29);
			}
        }
    }
    return c;

}

void constdec(int * fsys){//单次常量定义
        if(sy == INTSY || sy ==CHARSY){
            types = sy;
            getsym();
            test(Rmatrix[IdSY],fsys,2);//要么是标识符，要么重新开始新的模块
            while(sy == IdSY){
                strcpy(id,token);
                getsym();
                if(sy == ASSIGNSY){
                    getsym();
                }
                else if(sy == EQUSY){
                    Error(31);//应是赋值等
                    getsym();
                }
                else Error(31);//容错 不存在等号
                element c;
                constant(fsys,c);
                if(sy == COMMASY ||sy ==SEMISY ) {
                        if (sy==SEMISY) {
                            printf("This is a const statement\n");
                            getsym();
                            break;
                        }
                        getsym();
                }
                else skip(fsys,10);//非法符号
            }
        }
        else{
            skip(fsys,18);//应是类型标识符
        }

}

void variabledeclaration(int *fsys){//编写时间过长 ，可能有较多地方写不清楚
	//将当前获取的变量存入符号表中
	if(sy == LBRACK ||sy == IntSY){
		if(sy == LBRACK) getsym();
		else Error(8);//容错无左括号
		if(sy == IntSY){
			//将该变量存入数组表中
			char str_num[MAX];
			itoa(num,str_num,10);
			if(types == INTSY) {
                enter(id,Array,IntSY,btab[b].vsize);
                enterDcode(VAR,INT,str_num,id);
			}
			else if(types == CHARSY) {
                enter(id,Array,CharSY,btab[b].vsize);
                enterDcode(VAR,CHAR,str_num,id);
			}
			btab[b].vsize+= num;
			enteratab(num);
			printf("variabledeclaration array %s %s\n",ksy[types],id);
			getsym();
			if(sy == RBRACK) getsym();
			else test(varbeginsys,fsys,9);
		}
		else {
			test(varbeginsys,fsys,34);
		}
	}
	else if(types == CHARSY ){
        enter(id,Variable,CharSY,btab[b].vsize);
        btab[b].vsize+= 1;
        enterDcode(VAR,CHAR,NULLL,id);
		printf("variabledeclaration %s %s\n",ksy[types],id);
	}
	else if(types == INTSY){
        enter(id,Variable,IntSY,btab[b].vsize);
        btab[b].vsize+= 1;
        enterDcode(VAR,INT,NULLL,id);
		printf("variabledeclaration %s %s\n",ksy[types],id);
	}
	test(varbeginsys,fsys,5);
	while(sy == COMMASY ){
		getsym();
		int s1[MAX];
		memset(s1,0,MAX);
		s1[IdSY]=1;
		plusbeginsys(varbeginsys,s1,s1);
//		test(s1,fsys,2);
		if(sy == IdSY){
			strcpy(id,token);
			getsym();
			if(sy == LBRACK ||sy == IntSY){
				if(sy == LBRACK) getsym();
				else Error(8);//容错无左括号
				if(sy == IntSY){
                    char str_num[MAX];
                    itoa(num,str_num,10);
					//将该变量存入数组表中
                    if(types == INTSY) {
                        enter(id,Array,IntSY,btab[b].vsize);
                        enterDcode(VAR,INT,str_num,id);
                    }
                    else if(types == CHARSY) {
                        enter(id,Array,CharSY,btab[b].vsize);
                        enterDcode(VAR,CHAR,str_num,id);
                    }
                    btab[b].vsize+= num;
                    enteratab(num);
					printf("variabledeclaration array %s %s",ksy[types],id);
					getsym();
					if(sy == RBRACK) getsym();
					else test(varbeginsys,fsys,9);
				}
				else {
					test(varbeginsys,fsys,34);
				}
			}
			else if(types == CHARSY ){
                enter(id,Variable,CharSY,btab[b].vsize);
                btab[b].vsize+= 1;
                enterDcode(VAR,CHAR,NULLL,id);
				printf("variabledeclaration %s %s\n",ksy[types],id);
			}
			else if(types == INTSY){
                enter(id,Variable,IntSY,btab[b].vsize);
                btab[b].vsize+= 1;
                enterDcode(VAR,INT,NULLL,id);
				printf("variabledeclaration %s %s\n",ksy[types],id);
			}
		}
	}//sy == comma
	testsemi(fsys);
}

void parameterlist(int * fsys){
//	test(parameterbeginsys,fsys,5);//参数表首符号集合
	if(sy == RPARSY){
		 printf("This is a none parameterlist\n");
		 getsym();
	}
	else if(sy == LBRACE){
		Error(3);//应是右小括号
	}
	else {
		//存储当前符号表栈顶
		while(sy==INTSY||sy == CHARSY||sy==IdSY){
			types = NOTYPE;
			if(sy == INTSY ||sy==CHARSY){
				types = sy;
				getsym();
				if(sy!= IdSY) Error(2);
			}
			if(sy == IdSY){//如果
			    if(types == NOTYPE) Error(6);
                else{
                    if(types == INTSY) enter(token,Para,IntSY,btab[b].vsize);
                    if(types == CHARSY) enter(token,Para,CharSY,btab[b].vsize);
                    btab[b].vsize++;
                    btab[b].psize++;
                }
				printf("parameter %s type %s\n",token,ksy[sy]);
				getsym();
			}
			if(sy == COMMASY) getsym();
			else test(parameterbeginsys,fsys,5);
		}
		if(sy == RPARSY) {
			printf("This is a parameterlist\n");
			getsym();
		}
		else Error(3);
	}
	if(sy == LBRACE) {
		getsym();
//		test(compondstatementbeginsys,fsys,5);
	}
	else {
		Error(32);//应该是左花括号
//		test(compondstatementbeginsys,fsys,5);
	}
}

void plusbeginsys(int * s1,int * s2,int * s3){
	int i=0;
	for(i= 0;i<MAX;i++){
		if(s1[i]==1||s2[i]==1){
			s3[i]=1;
		}
	}
//	return s3;
}


element expression(int *fsys, element e);

void normalparalist(int *fsys){//置空

//	test(normalparabeginsys,fsys,5);
	int s[MAX];
	int npcount =0;
	memset(s,0,MAX);
	plusbeginsys(normalparabeginsys,fsys,s);
	element e;
	int re = loc(id);//如果函数未定义，re为-1，在之前跳读
	int realcount = btab[tab[re].ref].psize;
	if(factorbeginsys[sy]==1){
		e = expression(fsys,e);
        enterDcode(Npara,NULLL,NULLL,e.name);
		npcount++;
		if(e.elety != tab[re+npcount].types) Error(20);
		while(sy == COMMASY){
			getsym();
			test(normalparabeginsys,fsys,36);
			if(normalparabeginsys[sy]==1){
				element e2;
				e2 = expression(s,e2);
                enterDcode(Npara,e2.name,NULLL,NULLL);
				npcount++;//依据符号表进行值参个数判断
				if(npcount<=realcount){
                    if(e2.elety!= tab[re+npcount].types) Error(20);
				}
			}
		}

	}
	if(sy == RPARSY){
        if(npcount!= realcount) Error(23);
		getsym();
		printf("This is func statement\n");
	}
	else test(normalparabeginsys,fsys,3);
}

element factor(int *fsys,element f){
//	test(factorbeginsys,fsys,5);
	f.elenum = 0;
	f.elety = NOTYPE;
	memset(f.name,'\0',MAX);
	int s[MAX];
	memset(s,0,MAX);
	plusbeginsys(factorbeginsys,fsys,s);
	if(sy == IdSY){
		strcpy(id,token);
        int  re = loc(id);
        if(re == -1) Error(0);
        else {
            strcpy(f.name,id);
            if(tab[re].obj != Norefunc) f.elety = tab[re].types;//types应为Int或Char
            else Error(30);
        }
		getsym();
		if(sy == LBRACK){
            if(tab[re].obj != Array) Error(17);
			getsym();
			char arrayid[MAX];
			strcpy(arrayid,id);
			element e;
//			test(factorbeginsys,fsys,5);
			if(factorbeginsys[sy]==1){
				e = expression(fsys,e);
                getTvar(f.name);
                enterDcode(Arrayout,arrayid,e.name,f.name);
				if(e.elety!=IntSY) Error(16);
				if(sy == RBRACK) getsym();
				else Error(9);
//				printf("This is a array factor id:%s index:%d\n",id,e.elenum);
			}
		}
		else if(sy == LPARSY){
            if(tab[re].obj != Withrefunc) Error(30);
			getsym();
			if(re != -1)    {
			    getTvar(f.name);
			    char func[MAX];
			    strcpy(func,id);
                normalparalist(fsys);//不知是否会出现状态机错误
                enterDcode(Callret,func,NULLL,f.name);
			}
		}
	}
	else if(sy == PLUSSY ||sy == MINUSSY||sy == IntSY){
		int sign =1;
		if(sy == PLUSSY|| sy == MINUSSY){
			if (sy == MINUSSY) sign =-1;
			getsym();
		}
		if(sy == IntSY){
			f.elety = IntSY;
			f.elenum = num *sign;
            itoa(f.elenum,f.name,10);
			getsym();
		}
		else test(factorbeginsys,fsys,34);
	}
	else if(sy == CharSY){
		f.elety = CharSY;
		f.elenum = token[0];
		f.name[0] = '\'';
		f.name[1] = token[0];
		f.name[2] = '\'';
		getsym();
	}
	else if(sy == LPARSY){
		getsym();
//		test(factorbeginsys,fsys,36);
		if(factorbeginsys[sy]==1){
			f = expression(fsys,f);//带括号的表达式类型为Int
			f.elety = IntSY;
			if(sy == RPARSY){
				getsym();
			}
			else test(factorbeginsys,fsys,3);
		}
	}
	return f;
}

element terms(int *fsys,element t){
//	test(factorbeginsys,fsys,5);
	t.elenum = 0;
	t.elety = NOTYPE;
    element f1;//第一个因子
	int s[MAX];
	memset(s,0,MAX);
	plusbeginsys(factorbeginsys,fsys,s);
	if(factorbeginsys[sy]==1){
//		test(factorbeginsys,fsys,5);
		if(factorbeginsys[sy]==1){
			f1 = factor(fsys,f1);
			strcpy(t.name,f1.name);
			t.elenum = f1.elenum;
			t.elety = f1.elety;
		}
		int tflag = 0;
		int t_d[MAX];
		memset(t_d,0,MAX);
		t_d[STARSY]=1;t_d[DIVSY]=1;
		while(t_d[sy]==1){
            if(tflag  == 0){
                tflag =1 ;
                getTvar(t.name);
                enterDcode(MULT,ONE,f1.name,t.name);
            }
			int op;
			op =sy;
			getsym();
//			test(factorbeginsys,fsys,5);
			if(factorbeginsys[sy]==1){
				element f2;
				f2=factor(fsys,f2);
				t.elety = IntSY;
				if(op == STARSY){
//                    t.elenum = t.elenum * f2.elenum ;
                    enterDcode(MULT,t.name,f2.name,t.name);
				}
				else if(op == DIVSY) {
//                    t.elenum = t.elenum /f2.elenum ;
                    enterDcode(DIV,t.name,f2.name,t.name);
				}
			}
//			test(t_d,s,34);
		}
	}
	return t;
}

element expression(int *fsys,element e){
//	test(factorbeginsys,fsys,5);
	e.elenum = 0;
	e.elety = NOTYPE;
	getTvar(e.name);
	int s[MAX];
	memset(s,0,MAX);
	plusbeginsys(factorbeginsys,fsys,s);
	if(factorbeginsys[sy]==1){
		int sign =1;
		if(sy == PLUSSY || sy ==MINUSSY){
            e.elety =IntSY;
			if (sy == MINUSSY) sign = -1;
			getsym();
		}
//		test(factorbeginsys,fsys,5);
		if(factorbeginsys[sy]==1){
			element t1;
                t1=terms(fsys,t1);
			if(e.elety!= IntSY) e.elety=t1.elety;//如果在首项前出现符号，必为Int
			e.elenum=t1.elenum*sign;
			if(sign == 1) enterDcode(ADD,ZERO,t1.name,e.name);
			else enterDcode(SUB,ZERO,t1.name,e.name);
		}
		int p_m[MAX];
		memset(p_m,0,MAX);
		p_m[PLUSSY]=1;p_m[MINUSSY]=1;
		while(p_m[sy]==1){
			int sign = 1;
			if (sy == MINUSSY) sign = -1;
			getsym();
//			test(factorbeginsys,fsys,5);
			if(factorbeginsys[sy]==1){
				element t1;
				t1=terms(fsys,t1);
				e.elety=IntSY;//参与过运算，表达式类型为Int
				e.elenum=e.elenum+t1.elenum*sign;
				if(sign == 1) enterDcode(ADD,e.name,t1.name,e.name);
                else enterDcode(SUB,e.name,t1.name,e.name);
			}
//			test(p_m,s,34);
		}

		return e;
	}

}

element condition(int *fsys,element c){
	c.elety= NOTYPE;
	c.elenum =0;
	int s[MAX];
	memset(s,0,MAX);
	plusbeginsys(fsys,s,s);
	s[LPARSY] = 1;
//	test(s,fsys,5);
	if(sy == LPARSY|| factorbeginsys[sy]==1){
		if(sy != LPARSY) Error(7);
		getsym();
		test(factorbeginsys,fsys,36);
		if(factorbeginsys[sy]==1){
			element e1;
			e1 = expression(fsys,e1);
			c.elenum = e1.elenum;
			strcpy(c.name,e1.name);
			if(relationbeginsys[sy]==1){
				int relation = sy;
				getsym();
//				test(factorbeginsys,fsys,36);
				if(factorbeginsys[sy]==1){
					element e2;
					e2 = expression(fsys,e2);
                    getTvar(c.name);
					if(relation == LITTLESY){
                        enterDcode(LIT,e1.name,e2.name,c.name);
						c.elenum = e1.elenum<e2.elenum?1:0;
					}
					else if(relation == LITTLEEQUSY){
                        enterDcode(LITEQU,e1.name,e2.name,c.name);
						c.elenum = e1.elenum<=e2.elenum?1:0;
					}
					else if(relation == GREATERSY){
                        enterDcode(GRT,e1.name,e2.name,c.name);
						c.elenum = e1.elenum>e2.elenum?1:0;
					}
					else if(relation == GREATEREQUSY){
                        enterDcode(GRTEQU,e1.name,e2.name,c.name);
						c.elenum = e1.elenum>=e2.elenum?1:0;
					}
					else if(relation == EQUSY){
                        enterDcode(EQUU,e1.name,e2.name,c.name);
						c.elenum = e1.elenum==e2.elenum?1:0;
					}
					else if(relation == UNEQUSY){
                        enterDcode(UNEQU,e1.name,e2.name,c.name);
						c.elenum = e1.elenum!=e2.elenum?1:0;
					}
				}
			}
			c.elety = success;
			if(sy == RPARSY) getsym();
			else test(statementbeginsys,fsys,3);
			return c;

		}
	}
}

void ifstatement(int *fsys ){
	element c1;
	char label[MAX];
    getLabel(label);
	c1 = condition(fsys,c1);
    enterDcode(BNZ,c1.name,ZERO,label);
	statement(fsys);
    enterDcode(SET,NULLL,NULLL,label);
	if(c1.elety = success) printf("This is if statement \n");
}
void whilestatement(int *fsys){
	element c1;
	char label1[MAX],label2[MAX];
	getLabel(label1);
	getLabel(label2);
    enterDcode(SET,NULLL,NULLL,label1);
	c1 = condition(fsys,c1);
    enterDcode(BNZ,c1.name,ZERO,label2);
	statement(fsys);
    enterDcode(JUMP,NULLL,NULLL,label1);
    enterDcode(SET,NULLL,NULLL,label2);
	if(c1.elety = success) printf("This is while statement \n");
}
void scanfstatement(int *fsys){
	int s[MAX];
	if(sy == LPARSY){
		getsym();
	}
	else Error(7);
	memset(s,0,MAX);
	s[IdSY] = 1; s[SEMISY]=1;
//	test(s,fsys,2);
	if(sy == IdSY){
		strcpy(id,token);
        int re = loc(id);
        if(re == -1) Error(0);
        else{
            enterDcode(SCANF,ksy[tab[re].types],NULLL,id);
        }
		//进行符号表的存储操作
		getsym();
		while(sy == COMMASY){
			getsym();
			test(Rmatrix[IdSY],fsys,2);
			if(sy == IdSY) {
				strcpy(id,token);
				re = loc(id);
				if(re == -1) Error(0);
				else {
                    enterDcode(SCANF,ksy[tab[re].types],NULLL,id);
				}
				//进行符号表的操作
				getsym();
			}

		}
		if(sy == RPARSY){
			printf("This is scanf statement \n");
			getsym();
		}
		else test(fsys,fsys,3);
	}
	testsemi(fsys);
}

void printfstatement(int *fsys){
	if(sy == LPARSY){
		getsym();
	}
	else Error(7);
//	test(printfbeginsys,fsys,5);
	if(sy == StringSY){
		strcpy(id,token);
		enterDcode(PRINTF,STRING,id,NULLL);
		getsym();
		if(sy == COMMASY){
			getsym();
//			test(factorbeginsys,fsys,36);
			if(factorbeginsys[sy] = 1){
				element e;
				e = expression(fsys,e);
                enterDcode(PRINTF,ksy[e.elety],e.name,NULLL);
			}
		}
		if(sy == RPARSY){
			printf("This is printfstatement\n");
			getsym();
		}
		else test(fsys,fsys,3);
	}
	else if(factorbeginsys[sy]==1){
        element e2;
        e2 = expression(fsys,e2);
        enterDcode(PRINTF,ksy[e2.elety],e2.name,NULLL);
        if(sy == RPARSY){
			printf("This is printfstatement\n");
			getsym();
		}
		else test(fsys,fsys,3);
	}
	testsemi(fsys);
}
void returnstatement(int *fsys){
	if(sy == LPARSY){
		getsym();
		test(returnbeginsys,fsys,5);
        if(factorbeginsys[sy]==1){
            element e;
            e = expression(fsys,e);
            enterDcode(RET,btab[b].name,e.name,ksy[e.elety]);
            if(tab[btab[b].ref].types != e.elety) Error(16);
            if(sy == RPARSY){
                printf("This is return statememt\n");
                getsym();
            }
            else test(statementendsys,fsys,5);
        }
        testsemi(fsys);
	}
	else if(sy == SEMISY) {
        enterDcode(RET,btab[b].name,NULLL,NULLL);
        if(tab[btab[b].ref].types!=NOTYPE) Error(16);
        testsemi(fsys);
	}
	else Error(7);

}

void onecase( int *fsys,char* sname,char* label){
    char caselabel[MAX];
    memset(caselabel,'0',MAX);
    getLabel(caselabel);
    element c;
	c = constant(fsys,c);
	enterDcode(BNE,sname,c.name,caselabel);
    enterDcode(PULL,NULLL,NULLL,NULLL);
	if(sy == COLON){
		getsym();
	}
	else Error(38);
	statement(fsys);
	enterDcode(JUMP,NULLL,NULLL,label);
	enterDcode(SET,NULLL,NULLL,caselabel);
}
void  casetable(int*fsys,char*sname,char* label){
	onecase(fsys,sname,label);
//	test(Rmatrix[CASESY],fsys,-1);
	while(sy == CASESY){
		getsym();
		onecase(fsys,sname,label);
	}
}
void defaultstatement(int *fsys){
	if(sy == COLON){
		getsym();
	}
	else Error(38);
	statement(fsys);
}
void switchstatement(int *fsys){//未完成
	element s1;
	s1.elenum = 0;
	s1.elety = NOTYPE;
	char label[MAX];
    getLabel(label);
	if(sy == LPARSY){
		getsym();
	}
	else Error(7);
//	test(factorbeginsys,fsys,5);
	if(factorbeginsys[sy]==1){
		s1 = expression(fsys,s1);
		types = s1.elety;//用于后面常量比较
		if(sy == RPARSY) getsym();
		else Error(3);
		if(sy == LBRACE) getsym();
		else Error(32);
//		test(Rmatrix[CASESY],fsys,37);
		if(sy==CASESY){
			getsym();
			casetable(fsys,s1.name,label);
            enterDcode(PULL,NULLL,NULLL,NULLL);
			if(sy == DEFAULTSY) {
				getsym();
				defaultstatement(fsys);
			}
			if (sy==RBRACE) {
                enterDcode(SET,NULLL,NULLL,label);
				printf("This is switchstatement\n");
				getsym();
			}
			else Error(33);

		}
	}
}
void statement(int *fsys){//单个语句处理

	int s3[MAX];
	memset(s3,0,MAX);
	plusbeginsys(statementbeginsys,fsys,s3);
//	test(statementbeginsys,fsys,5);
	int lbraceflag = 0;
	if(sy == LBRACE){
		lbraceflag = 1;
		getsym();
		while(statementbeginsys[sy]==1){
			statement(s3);
		}
	}
	else if(sy == IFSY) {
		getsym();
		ifstatement(fsys);
	}
	else if(sy == WHILESY) {
		getsym();
		whilestatement(fsys);
	}
	else if(sy == SCANFSY) {
		getsym();
		scanfstatement(fsys);
	}
	else if(sy == PRINTFSY) {
		getsym();
		printfstatement(fsys);
	}
	else if(sy == SWITCHSY) {
		getsym();
		switchstatement(fsys);
	}
	else if(sy == RETURNSY) {
		getsym();
		returnstatement(fsys);
	}
	else if(sy == SEMISY){
		printf("This is none statement\n");
		testsemi(fsys);
	}
	else if(sy == IdSY) {//赋值 有返回值的函数调用 无返回值的函数调用
		strcpy(id,token);
		int re = loc(id);
		int assignt = re ;//表示id符号表位置
        char assignid[MAX];
		strcpy(assignid,id);
		if(re == -1) Error(0);
		getsym();
		int s1[MAX],s2[MAX];
		memset(s1,0,MAX);memset(s2,0,MAX);
		plusbeginsys(statementbeginsys,fsys,s2);
		s1[ASSIGNSY]=1;s1[LBRACK]=1;s1[LPARSY]=1;
//		test(s1,s2,5);
		if(sy == ASSIGNSY){
			getsym();
			element e;
			e = expression(fsys,e);
            if(tab[assignt].types !=e.elety)    Error(27);
            else{
                enterDcode(ASSIGN,e.name,NULLL,assignid);
            }
			printf("This is a assign statement %s = %d type:%s\n",assignid,e.elenum,ksy[e.elety]);
			testsemi(fsys);
			//将表达式值与类型填入之前获得的id中
		}
		else if(sy == LBRACK){
            if(tab[assignt].obj!= Array) Error(17);
            //中间代码生成，若标识符不为数组，则不管方括号
			getsym();
			element ref;
			ref =expression(fsys,ref);
			if(ref.elety!=IntSY) Error(16);
			if(sy == RBRACK) getsym();
			else Error(9);
			if(sy == ASSIGNSY){
				getsym();
				element e2;
				e2 =expression(fsys,e2);
                if(tab[assignt].types !=e2.elety)    Error(27);
                else{
                    enterDcode(Arrayasg,e2.name,ref.name,assignid);
                }
				printf("This is an array assign statement %s [%d] = %d type:%s\n",id,ref.elenum,e2.elenum,ksy[e2.elety]);
				testsemi(fsys);
			}
			//为数组变量赋值
		}
		else if(sy == LPARSY){
			//值参数表
			int re = loc(id);
			if(re==-1) Error(0);
			getsym();
			if(re!= -1){
                 char func[MAX];
                 strcpy(func,id);
                 normalparalist(fsys);//不知此处是否会有状态机错误
                 enterDcode(Call,func,NULLL,NULLL);
			}
			testsemi(fsys);
		}
	}
	if(lbraceflag == 1){
		if(sy != RBRACE) Error(33);
		else getsym();
	}

}//end statement

void compondstatement(int * fsys){//【注意对于函数的复合语句，应当确保有return语句】
	test(compondstatementbeginsys,fsys,5);
	if(compondstatementbeginsys[sy]==1){
		while(sy == CONSTSY){
	        getsym();
			constdec(fsys);
//			test(compondstatementbeginsys,fsys,5);
		}
		while(sy == INTSY || sy == CHARSY){
			types = sy;
			getsym();
	//		test(Rmatrix[IdSY],compondstatementbeginsys,2);
			if(sy == IdSY){
				strcpy(id,token);
				getsym();
				int s2[MAX];
				memset(s2,0,MAX);
				plusbeginsys(compondstatementbeginsys,fsys,s2);
				s2[CONSTSY]=0;
				variabledeclaration(s2);
	//			test(s2,fsys,5);//非法符号
			}
		}
		while(statementbeginsys[sy]==1){
			int s3[MAX];
			memset(s3,0,MAX);
			plusbeginsys(compondstatementbeginsys,fsys,s3);
			s3[CONSTSY]=0;s3[INTSY]=0;s3[CHARSY]=0;
			statement(s3);
		}
		if(sy == RBRACE){
			getsym();
	//		test(programbeginsys,programbeginsys,5);//存在疑问，停止集合是这样写么
		}
		else{
			Error(33);
	//		test(programbeginsys,programbeginsys,5);
		}
	}
}
void funcdeclaration(int * fsys){
	//将函数类型与标识符压入栈中
    char func[MAX];
    strcpy(func,id);
	if(types == INTSY || types == CHARSY){
        if(types==INTSY) {
            enter(id,Withrefunc,IntSY,0);//函数adr相对地址未想清楚
            enterDcode(SET,NULLL,NULLL,id);
            enterDcode(Funcdec,INT,NULLL,id);
        }
        else if(types==CHARSY){
            enter(id,Withrefunc,CharSY,0);
            enterDcode(SET,NULLL,NULLL,id);
            enterDcode(Funcdec,CHAR,NULLL,id);
        }
        enterbtab();
		printf("This is a withreturn Func\n");
	}
	else if(types == VOIDSY){
        enter(id,Norefunc,NOTYPE,0);
        enterDcode(SET,NULLL,NULLL,id);
        enterDcode(Funcdec,VOID,NULLL,id);
        enterbtab();
		printf("This is a noreturn Func\n");
	}
	getsym();
	parameterlist(fsys);
	compondstatement(fsys);
	enterDcode(RET,func,NULLL,NULLL);
}
int isfuncdec(){
    if(sy == LPARSY){
        return 1;
    }
    return 0;
}

int main(){
    char datapath[300];
    printf("请输入带路径的测试程序的文件名：");
    scanf("%s",&datapath);
//    printf("%s\n",datapath);
	freopen(datapath,"r",stdin);//测试文件text
//	freopen("lj.txt","r",stdin);//测试文件text
	Setup();
	btab[0].first=0;
    getsym();
    test(Rmatrix[CONSTSY],programbeginsys,-1);
    while(sy == CONSTSY){
        getsym();
    	constdec(programbeginsys);
    	test(programbeginsys,programbeginsys,5);
	}
	programbeginsys[CONSTSY]=0;//将const 从程序集合中删除
	test(typesbeginsys,programbeginsys,-2);
	while(sy == INTSY ||sy == CHARSY ||sy == VOIDSY){
		types = sy;
		getsym();
		int s2[MAX];
		memset(s2,0,MAX);
		s2[IdSY]=1;s2[MAINSY]=1;//主函数或其他
		test(s2,programbeginsys,2);
		if(sy == IdSY){
			strcpy(id,token);
			getsym();
			int s3[MAX];
			memset(s3,0,MAX);
			s3[SEMISY]=1;s3[COMMASY]=1;s3[LPARSY]=1;s3[LBRACK]=1;s3[IdSY]=1;//参数表或变量定义
			test(s3,programbeginsys,5);
			if(sy == LPARSY){
                //第一次进入函数时运行时首先跳到主函数部分执行
                if(Funcflag==0) enterDcode(Call,MAIN,NULLL,NULLL);
				Funcflag = 1; //进入函数定义，不允许再读变量
				funcdeclaration(programbeginsys);
			}
			else if(sy == SEMISY||sy ==COMMASY ||sy ==IdSY||sy == LBRACK){
				if(Funcflag==1) {//确保函数声明后不允许再声明变量
					printf("variable declaration illegal\n");
					test(typesbeginsys,programbeginsys,5);
				}
				else{
					variabledeclaration(programbeginsys);
					test(typesbeginsys,programbeginsys,5);
				}

			}
			else test(typesbeginsys,programbeginsys,5);
		}
		else if(sy == MAINSY && types == VOIDSY){
		    char name[MAX];
            strcpy(name,"main");
            enter(name,Norefunc,NOTYPE,0);
            //若无函数也先跳至主函数部分运行
            if(Funcflag==0) enterDcode(JUMP,NULLL,NULLL,MAIN);
            enterDcode(SET,NULLL,NULLL,name);
            enterDcode(Funcdec,VOID,NULLL,name);
            enterbtab();
			getsym();
			if(sy == LPARSY ) getsym();
			else {
				Error(7);
			}
			if(sy == RPARSY) getsym();
			else Error(3);
			if(sy == LBRACE) getsym();
			else Error(32);
			programbeginsys[INTSY]=0;programbeginsys[CHARSY]=0;programbeginsys[VOIDSY]=0; //将int char void 从程序后继集合中删去
			compondstatement(programbeginsys);
            enterDcode(RET,MAIN,NULLL,NULLL);
            enter("end",Norefunc,NOTYPE,0);
            enterbtab();
			test(Rmatrix[EOFSY],programbeginsys,39);
		}
	}
	test(Rmatrix[EOFSY],Rmatrix[EOFSY],39);
	printf("This is a program\n");
//    freopen("table.txt","w",stdout);
    printtable();
    if(errorflag== 0){
        printDcode();
        target();
        freopen("result.txt","w",stdout);
        printtarcode();
    }
    else {
        printf("ERROR exists\n");
    }
    return 0;
}














