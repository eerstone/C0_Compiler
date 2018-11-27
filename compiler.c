#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<string.h>


#define	CONSTSY	0//定义词法分析的各个单词类型
#define	INTSY	1
#define	CHARSY	2
#define	VOIDSY	3
#define MAINSY 4
#define	IFSY	5
#define	WHILESY	6
#define	SWITCHSY	7
#define	CASESY	8
#define	DEFAULTSY	9
#define	SCANFSY	10
#define	PRINTFSY	11
#define	RETURNSY	12
#define EOFSY 13

#define CharSY 27
#define NoteSY 28
#define StringSY 29
#define	IdSY	30
#define	IntSY	31
#define	COMMASY	32
#define	EQUSY	33
#define	SEMISY	34
#define	ASSIGNSY	35
#define	PLUSSY	36
#define	MINUSSY	37
#define	STARSY	38
#define	DIVSY	39
#define	UNEQUSY	40
#define	LITTLESY	41
#define	LITTLEEQUSY	42
#define	GREATERSY	43
#define	GREATEREQUSY	44
#define	LPARSY	45
#define	RPARSY	46
#define LBRACK  47
#define RBRACK 48
#define LBRACE 49
#define RBRACE 50
#define COLON 51
#define ERROR 404
#define NOTYPE 405

#define success 666
#define MAX 200//定义其他宏
#define Resvnum 52

#define tmax 1000
#define bmax 1000
#define amax 1000
#define smax 1000
#define dmax 1000

//符号表定义
typedef  struct {
	char name[20];
	int link;
	int obj;
	int types;
	int ref;
	int normal;
	int lev;
	int adr;
}Tab;
Tab tab[tmax];

typedef  struct{
	int last;
	int lastpar;
	int psize;
	int vsize;
}Btab;
Btab btab[bmax];

typedef  struct {
	int eltyp;
	int low,high;
	int elsize,size;
}Atab;
Atab atab[amax];

typedef 	struct{
	char string[1000];
}Stab;
Stab stab[smax];

typedef 	struct{
	int f,x,y,r;
}Order;
Order Dcode[dmax];

typedef struct {
	int elety;
	int elenum;
}element;

int skipflag,Funcflag;

int t,b,a,c2,plc;//符号表索引变量

int types;
char id[MAX];

int sy;//word 的类型
char token[MAX];
int token_top;
int num;//读入的字符串数字转为int
int cc,ll,lastcc;
int lc=-1,lastlc;//program line;
int errpos;
char line[MAX];//读取行源代码
char ch,lastch;//nextch函数的全局变量，存放当前字符
char key[MAX][MAX] ={"const","int","char","void","main","if","while","switch","case","default","scanf","printf","return"};
char ksy[MAX][MAX] ={"constsy","intsy","charsy","voidsy","mainsy","ifsy","whilesy","switchsy","casesy","defaultsy","scanfsy","printfsy","returnsy"};

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
}


char get_Char(){
	lastch =ch;
	if(cc==ll){
		if(errpos!=0){
			errpos = 0;
		}
		ll =0 ;
		cc = 0;
		while(ch!='\n' && ch != EOF ){
			ch = getchar();
			line[ll++]=ch;
		}
		line[ll++]=' ';
		lc+=1;
	}
	ch = line[cc++];
	return ch;
}

void retract(){ //回退一个字符
//	int success=0;
//	off_t cur=0;
//	cur = fseek(stdin,-1,SEEK_CUR);
	cc = cc-1;
//	printf("success == : %d\n",cur);
}

void clear_Token(){
	int i;
	for(i = 0 ; i < MAX;i++){
		token[i]='\0';
	}
	token_top=0;
}

int catToken(){
	token[token_top++]=ch;
}

void printToken(){
	int i;
	for(i = 0; i<token_top;i++){
		printf("%c",token[i]);
	}
}

int isLetter(){
	if((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z') ||(ch =='_'))
		return 1;
	else return 0;
}

int isDight(){
	if(ch >= '0' && ch <= '9') return 1;
	else return 0;
}

int isString(){
	if((ch == 32)||(ch == 33||ch == 9)||(ch>=35&&ch<=126)){
		return 1;
	}
	else return 0;
}


int strcompire(char * x , char * y){//字符串匹配
	int len=0;
	while(x[len]!='\0' || y[len]!='\0'){
		if (x[len]!=y[len]) return 0;
		len++;
	}
	return 1;
}

int Key(){
	int i;
	for(i = 0;i< MAX;i++){
		if(strcompire(token,key[i])){
			return i;//保留字从0开始计数
		}
	}
	return -1;
}

int transNum(){
	int i;
	int ans = 0;
	int Leadingzero = 0;
	if(token[0]=='0'&& token[1]=='0') Leadingzero =1;
	for (i = 0;i<token_top;i++){
		ans=ans*10 + (token[i]-'0');
	}
	if(Leadingzero == 1) printf("next_int has leading zero");
 	return ans;
}

void error(){
	printf("error  [%d,%d]:",lc,cc);
	printf("%d %c\n",ch,ch);
	sy = ERROR;
}

int getsym(){//获取word，将类型存入sy 内容存入token
	lastcc = cc;
	lastlc =lc;//晚一回合更新，用于报错时确认位置信息 
	clear_Token();
	get_Char();
	while(ch == ' '|| ch == '\n'||ch =='\t')
		get_Char();

	if(isLetter()){
		while(isLetter()||isDight()){
			catToken();
			get_Char();
		}
		retract();
		int resultValue = Key();
		if (resultValue==-1) sy = IdSY;
		else sy = resultValue;
	}
	else if (isDight()){
		while(isDight()){
			catToken();
			get_Char();
		}
		retract();
		num = transNum();
		sy = IntSY;
	}
	else if(ch == '/'){
		get_Char();
		if(ch == '/') {
			sy = NoteSY;
			catToken();//读入两个字符’/‘
			while(ch != '\n' ){
				catToken();
				get_Char();
			}//不需要retract
		}
		else {
			sy = DIVSY;
			retract();
		}
	}
	else if(ch == '!'){
        catToken();
		get_Char();
		if(ch== '='){
            catToken();
            sy = UNEQUSY;
		}
		else{
			retract();
			error();
		}
	}
	else if(ch == '='){
        catToken();
		get_Char();
		if(ch == '='){
		    catToken();
			sy = EQUSY;
		}
		else {
			retract();
			sy = ASSIGNSY;
		}
	}
	else if(ch == '<'){
	    catToken();
		get_Char();
		if(ch == '='){
		    catToken();
			sy = LITTLEEQUSY;
		}
		else {
			retract();
			sy = LITTLESY;
		}
	}
	else if(ch == '>'){
	    catToken();
		get_Char();
		if(ch == '='){
		    catToken();
			sy = GREATEREQUSY;
		}
		else {
			retract();
			sy = GREATERSY;
		}
	}
	else if(ch == '\"'){
		get_Char();
		while(ch != '\"' && ch!= '\n') {
			if(isString()){
				catToken();
			}
			else error();
			get_Char();
		}
		sy = StringSY;
		if(ch == '\n'){
			 error();
		}
	}
	else if(ch == '\''){
		get_Char();
		while(ch != '\'' && ch != '\n'){
			if(isLetter()||isDight||ch =='+'||ch=='-'||ch=='*'||ch=='/'){
				catToken();
			}
			else error();
			get_Char();
		}
		sy = CharSY;
		if(ch == '\n') error();
		else if(token_top>1) error();
	}
	else if(ch == EOF){
		sy = EOFSY; 
	}
	else {
		catToken();
		int result = Key();
		if(result!=-1) sy = result;
		else {
			error();
		}
	}
	return 0;
}

void endskip(){//未设置errpos
    skipflag = 0;
}

void Error(int n){
	if(cc>errpos){
		errpos = cc +3;
		if (n>=0) printf("Error line%d loc%d:%d\n",lastlc,lastcc,n);
		switch(n){
			case 10:{
					printf("expect: ; \n");
				break;
			}
			case 31:{
					printf("expect: =\n");
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

void constant(int *fsys){
    int sign = 1;
    test(constbeginsys,fsys,29);//应是常量
    if(constbeginsys[sy]==1){
        printf("This is a constant ");
        if(sy == CharSY){
            printf(" char: %c\n",token[0]);
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
                getsym();
            }
            else{
            	test(semicommabeginsys,fsys,29);
			} 
        }
    }

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
                constant(fsys);
                if(sy == COMMASY ||sy ==SEMISY ) {
                        if (sy==SEMISY) printf("This is a const statement\n");
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
		printf("variabledeclaration %s %s\n",ksy[types],id);
	}
	else if(types == INTSY){
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
					//将该变量存入数组表中 
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
				printf("variabledeclaration %s %s\n",ksy[types],id); 
			}
			else if(types == INTSY){
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
	memset(s,0,MAX);
	plusbeginsys(normalparabeginsys,fsys,s);
	element e;
	if(factorbeginsys[sy]==1){
		e = expression(fsys,e);
		while(sy == COMMASY){
			getsym();
			test(normalparabeginsys,fsys,36);
			if(normalparabeginsys[sy]==1){
				element e2;
				e2 = expression(s,e2);
				//依据符号表进行值参个数判断	
			}
		}
		
	}
	if(sy == RPARSY){
		getsym();
		printf("This is func statement\n");
	} 
	else test(normalparabeginsys,fsys,3); 
}

element factor(int *fsys,element f){
//	test(factorbeginsys,fsys,5);
	f.elenum = 0;
	f.elety = NOTYPE;
	int s[MAX];
	memset(s,0,MAX);
	plusbeginsys(factorbeginsys,fsys,s);
	if(sy == IdSY){
		strcpy(id,token);
		getsym();
		if(sy == LBRACK){
			getsym();
			element e;
//			test(factorbeginsys,fsys,5);

			if(factorbeginsys[sy]==1){
				e = expression(fsys,e);
				if(sy == RBRACK) getsym();
				else Error(9);
//				printf("This is a array factor id:%s index:%d\n",id,e.elenum);
			}
		}
		else if(sy == LPARSY){
			getsym();
			normalparalist(fsys);
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
			getsym(); 
		}
		else test(factorbeginsys,fsys,34);
	}
	else if(sy == CharSY){
		f.elety = CharSY;
		f.elenum = token[0];
		getsym();
	}
	else if(sy == LPARSY){
		getsym();
//		test(factorbeginsys,fsys,36);
		if(factorbeginsys[sy]==1){
			f = expression(fsys,f);
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
	int s[MAX];
	memset(s,0,MAX);
	plusbeginsys(factorbeginsys,fsys,s);
	if(factorbeginsys[sy]==1){
//		test(factorbeginsys,fsys,5);
		if(factorbeginsys[sy]==1){
			element f1;
			f1 = factor(fsys,f1);
			t.elenum = f1.elenum;
			t.elety = f1.elety;
		}
		int t_d[MAX];
		memset(t_d,0,MAX);
		t_d[STARSY]=1;t_d[DIVSY]=1;
		while(t_d[sy]==1){
			int op;
			op =sy;
			getsym();
//			test(factorbeginsys,fsys,5);
			if(factorbeginsys[sy]==1){
				element f2;
				f2=factor(fsys,f2);
				if(op == STARSY) t.elenum = t.elenum * f2.elenum ;
				else if(op == DIVSY) t.elenum = t.elenum /f2.elenum  ;
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
	int s[MAX];
	memset(s,0,MAX);
	plusbeginsys(factorbeginsys,fsys,s);
	if(factorbeginsys[sy]==1){
		int sign =1;
		if(sy == PLUSSY || sy ==MINUSSY){
			if (sy == MINUSSY) sign = -1;
			getsym();
		}
//		test(factorbeginsys,fsys,5);
		if(factorbeginsys[sy]==1){
			element t1;
			t1=terms(fsys,t1);
			e.elety=t1.elety;
			e.elenum=t1.elenum*sign;
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
//				e.elety=t1.elety;//表达式类型暂定为第一个项的类型 
				e.elenum=e.elenum+t1.elenum*sign;
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
			if(relationbeginsys[sy]==1){
				int relation = sy;
				getsym();
//				test(factorbeginsys,fsys,36);
				if(factorbeginsys[sy]==1){
					element e2;
					e2 = expression(fsys,e2);
					if(relation == LITTLESY){
						c.elenum = e1.elenum<e2.elenum?1:0;
					}
					else if(relation == LITTLEEQUSY){
						c.elenum = e1.elenum<=e2.elenum?1:0;
					}
					else if(relation == GREATERSY){
						c.elenum = e1.elenum>e2.elenum?1:0;
					}
					else if(relation == GREATEREQUSY){
						c.elenum = e1.elenum>=e2.elenum?1:0;
					}
					else if(relation == EQUSY){
						c.elenum = e1.elenum==e2.elenum?1:0;
					}
					else if(relation == UNEQUSY){
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
	c1 = condition(fsys,c1);
	statement(fsys);
	if(c1.elety = success) printf("This is if statement \n");
}
void whilestatement(int *fsys){
	element c1;
	c1 = condition(fsys,c1);
	statement(fsys);
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
		//进行符号表的存储操作
		getsym();
		while(sy == COMMASY){
			getsym();
			test(Rmatrix[IdSY],fsys,2);
			if(sy == IdSY) {
				strcpy(id,token);
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
		getsym();
		if(sy == COMMASY){
			getsym();
//			test(factorbeginsys,fsys,36);
			if(factorbeginsys[sy] = 1){
				element e;
				e = expression(fsys,e);
			}
		}
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
	}
	else if(sy != SEMISY) Error(7);
	test(returnbeginsys,fsys,5);
	if(factorbeginsys[sy]==1){
		element e;
		e = expression(fsys,e);
		if(sy == RPARSY){
			printf("This is return statememt\n");
			getsym();
		} 
		else test(statementendsys,fsys,5);
	}
	testsemi(fsys); 
}

void onecase( int *fsys){
	constant(fsys);
	if(sy == COLON){
		getsym();
	}
	else Error(38);
	statement(fsys);
}
void  casetable(int*fsys){
	onecase(fsys);
//	test(Rmatrix[CASESY],fsys,-1);
	while(sy == CASESY){
		getsym();
		onecase(fsys);
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
	if(sy == LPARSY){
		getsym();
	}
	else Error(7);
//	test(factorbeginsys,fsys,5);
	if(factorbeginsys[sy]==1){
		s1 = expression(fsys,s1);
		if(sy == RPARSY) getsym();
		else Error(3);
		if(sy == LBRACE) getsym();
		else Error(32);
//		test(Rmatrix[CASESY],fsys,37);
		if(sy==CASESY){
			getsym();
			casetable(fsys);
			if(sy == DEFAULTSY) {
				getsym();
				defaultstatement(fsys); 
			}
			if (sy==RBRACE) {
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
		getsym(); 
		int s1[MAX],s2[MAX];
		memset(s1,0,MAX);memset(s2,0,MAX);
		plusbeginsys(statementbeginsys,fsys,s2);
		s1[ASSIGNSY]=1;s1[LBRACK]=1;s1[LPARSY]=1;
//		test(s1,s2,5);
		if(sy == ASSIGNSY){
			char assignid[MAX];
			strcpy(assignid,id);
			getsym();
			element e;
			e = expression(fsys,e);
			printf("This is a assign statement %s = %d type:%s\n",assignid,e.elenum,ksy[e.elety]);
			testsemi(fsys);
			//将表达式值与类型填入之前获得的id中 
		}
		else if(sy == LBRACK){
			getsym();
			element ref; 	
			ref =expression(fsys,ref);
			if(sy == RBRACK) getsym();
			else Error(9);
			if(sy == ASSIGNSY){
				getsym(); 
				element e2;
				e2 =expression(fsys,e2);
				printf("This is an array assign statement %s [%d] = %d type:%s\n",id,ref.elenum,e2.elenum,ksy[e2.elety]);
				testsemi(fsys);
			} 
			//为数组变量赋值 
		}
		else if(sy == LPARSY){
			//值参数表 
			getsym();
			normalparalist(fsys);
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
			test(compondstatementbeginsys,fsys,5);
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
	if(types == INTSY || types == CHARSY){
		printf("This is a withreturn Func\n"); 
	}
	else if(types == VOIDSY){
		printf("This is a noreturn Func\n");
	}
	getsym(); 
	parameterlist(fsys);
	compondstatement(fsys); 
	
}
int isfuncdec(){
    if(sy == LPARSY){
        return 1;
    }
    return 0;
}

int main(){
    char datapath[300];
//    printf("请输入带路径的测试程序的文件名：");
//    scanf("%s",&datapath);
//    printf("%s\n",datapath);
//	freopen(datapath,"r",stdin);//测试文件text
	freopen("test_const_error.txt","r",stdin);//测试文件text
	Setup();
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
			test(Rmatrix[EOFSY],programbeginsys,39);
		}
	}
	test(Rmatrix[EOFSY],Rmatrix[EOFSY],39);
	printf("This is a program\n");
    return 0;
}














