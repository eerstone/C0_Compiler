#include<stdio.h>

#include "define.h"
#include "var.h"


char line[MAX];//读取行源代码
char ch,lastch;//nextch函数的全局变量，存放当前字符

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
