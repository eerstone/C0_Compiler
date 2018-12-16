#include<stdio.h>
#include<string.h>

#include "define.h"
#include "table.h"
#include "var.h"

void tarIn(char * code){
    strcpy(tarcode[tarc],code);
    tarc++;
}
void spmovedown(int off){
    char code[MAX];
    memset(code,'\0',MAX);
    strcpy(code,"sub $sp,$sp,");
    char str_off[MAX];
    itoa(off,str_off,10);
    sprintf(code,"%s%s\n",code,str_off);
    tarIn(code);
}
void spmoveup(int off){
    char code[MAX];
    memset(code,'\0',MAX);
    strcpy(code,"add $sp,$sp,");
    char str_off[MAX];
    itoa(off,str_off,10);
    sprintf(code,"%s%s\n",code,str_off);
    tarIn(code);
}
int tarloc(char * Id){
    int i;
    if(tarb == 0){
        for(i=btab[0].first;i<t;i++){
            if(strcmp(tab[i].name,Id)==0){
                return i;
            }
        }
        return -1;
    }
    else{
        for(i=btab[tarb].first;i<btab[tarb].last;i++){
            if(strcmp(tab[i].name,Id)==0){
                return i;
            }
        }
        for(i = btab[0].first;i<btab[0].last;i++){
            if(strcmp(tab[i].name,Id)==0){
                return i;
            }
        }
        for(i = 0; i<= tarb;i++){
            if(strcmp(btab[i].name,Id)==0){
                return btab[i].ref;
            }
        }
        return -1;
    }
}
void tarleft(char *left){
    int re = tarloc(left);
    char code[MAX];
    memset(code,'\0',MAX);
    if(left[0]=='$'){//临时变量
        spmoveup(4);
        tarIn("lw $t1,0($sp)\n");
    }
    else if(re != -1){//标识符
        if(tab[re].obj==Const){
            sprintf(code,"add $t1,$0,%d\n",tab[re].adr);
            tarIn(code);
        }
        //如果是变量
        else if(re<btab[0].last){
            sprintf(code,"lw $t1,-%d($s0)\n",tab[re].adr);
            tarIn(code);
        }
        else{
            sprintf(code,"lw $t1,-%d($s1)\n",tab[re].adr);
            tarIn(code);
        }
    }
    else {//值
        if(((left[0]>='0')&&(left[0]<='9'))||(left[0]=='+')||(left[0]=='-')){
            int number;
            number = atoi(left);
            sprintf(code,"add $t1,$0,%d\n",number);
            tarIn(code);
        }
        else{
            sprintf(code,"add $t1,$0,%d\n",left[1]);
            tarIn(code);
        }
    }
}
void tarright(char *right){
    int re = tarloc(right);
    char code[MAX];
    memset(code,'\0',MAX);
    if(right[0]=='$'){
        spmoveup(4);
        tarIn("lw $t2,0($sp)\n");
    }
    else if(re != -1){
        if(tab[re].obj==Const){
            sprintf(code,"add $t2,$0,%d\n",tab[re].adr);
            tarIn(code);
        }
        //如果是变量
        else if(re<btab[0].last){
            sprintf(code,"lw $t2,-%d($s0)\n",tab[re].adr);
            tarIn(code);
        }
        else{
            sprintf(code,"lw $t2,-%d($s1)\n",tab[re].adr);
            tarIn(code);
        }
    }
    else {
        if(((right[0]>='0')&&(right[0]<='9'))||(right[0]=='+')||(right[0]=='-')){
            int number;
            number = atoi(right);
            sprintf(code,"add $t2,$0,%d\n",number);
            tarIn(code);
        }
        else{
            sprintf(code,"add $t2,$0,%d\n",right[1]);
            tarIn(code);
        }
    }
}
void tarresult(char * result){
    int re =tarloc(result);
    char code[MAX];
    memset(code,'\0',MAX);
    if(result[0]=='$'){
        tarIn("sw $t3,0($sp)\n");
        spmovedown(4);
    }
    else{
        if(re<btab[0].last){
            sprintf(code,"sw $t3,-%d($s0)\n",tab[re].adr);
            tarIn(code);
        }
        else{
            sprintf(code,"sw $t3,-%d($s1)\n",tab[re].adr);
            tarIn(code);
        }
    }
}
void target(){
    tarb = b;//将生成代码函数指针赋初值
    tarIn("move $s0,$sp\n");
    int i;
    for(i = 0;i<plc;i++){
        int f = Dcode[i].f;
        char left[MAX],right[MAX],result[MAX],F[MAX];
        strcpy(left,Dcode[i].Left);
        strcpy(right,Dcode[i].Right);
        strcpy(result,Dcode[i].Result);
        strcpy(F,Dcode[i].F);
        char code[MAX];
        if(f == VAR){
            int re = tarloc(result);
            if(re < btab[0].last){//全局变量，则压栈
                if(strcmp(right,NULLL)==0){
                    spmovedown(4);
                }
                else{
                    int number;
                    number = atoi(right);
                    spmovedown(4*number);
                }
            }

        }
        else if(f == SET){
            memset(code,'\0',MAX);
            sprintf(code,"%s:\n",result);
            tarIn(code);
        }
        else if(f == Funcdec){
            memset(code,'\0',MAX);
            tarIn("sw $ra,0($sp)\n");
            spmovedown(4);
            int fun = tarloc(result);//从
            tarb = tab[fun].ref;
        }
        else if(f == Call || f ==Callret){
            memset(code,'\0',MAX);
            int re = tarloc(left);
            int psize = btab[tab[re].ref].psize;
            int vsize = btab[tab[re].ref].vsize;
            spmovedown((vsize-psize)*4);//分配局部变量
            if (strcmp(left,MAIN)==0){
                tarIn("sw $s0,0($sp)\n");
            }
            else{
                tarIn("sw $s1,0($sp)\n");
            }
            spmovedown(4);
            sprintf(code,"add $s1,$sp,%d\n",(vsize+1)*4);//函数基址上移，指向该函数形参变量入口
            tarIn(code);
            memset(code,'\0',MAX);
            sprintf(code,"jal %s\n",left);
            tarIn(code);
            if(f == Call){
                spmoveup(4);
            }
            if(strcmp(left,MAIN)==0){
                tarIn("li $v0,10\n");
                tarIn("syscall\n");
            }
        }
        else if(f == Npara){
            //do nothing
        }
        else if(f == RET){
            memset(code,'\0',MAX);
            int re = tarloc(left);
            int vsize = 0;
            vsize = btab[tab[re].ref].vsize;
            if(strcmp(result,NULLL)!=0){//有返回值时弹出
                tarright(right);
            }
            spmoveup(4);
            tarIn("lw $ra,0($sp)\n");
            spmoveup(4);
            tarIn("lw $s1,0($sp)\n");
            spmoveup(vsize*4);
            if(strcmp(result,NULLL)==0){
                tarIn("sw $0,0($sp)\n");
                spmovedown(4);
            }
            else{
                tarIn("sw $t2,0($sp)\n");
                spmovedown(4);
            }
            tarIn("jr $ra\n");


        }//RET
        else if(f == ASSIGN){
            tarleft(left);
            tarIn("add $t3,$t1,$0\n");
            tarresult(result);
        }
        else if(f == Arrayasg){
            memset(code,'\0',MAX);
            tarleft(left);//left是后压进栈先取出来
            tarright(right);
            memset(code,'\0',MAX);
            int re = tarloc(result);
            if(re<btab[0].last){
                tarIn("sll $t2,$t2,2\n");//数组偏移量
                sprintf(code,"sub $t3,$s0,%d\n",tab[re].adr);//数组基地址
                tarIn(code);
                tarIn("sub $t3,$t3,$t2\n");
                tarIn("sw $t1,($t3)\n");
            }
            else{
                tarIn("sll $t2,$t2,2\n");
                sprintf(code,"sub $t3,$s1,%d\n",tab[re].adr);
                tarIn(code);
                tarIn("sub $t3,$t3,$t2\n");
                tarIn("sw $t1,($t3)\n");
            }
        }//Arrayasg
        else if(f == Arrayout){
            memset(code,'\0',MAX);
//            tarleft(left);//数组标识符变量不应取值
            int re = tarloc(left);
            if(re<btab[0].last){
                sprintf(code,"sub $t1,$s0,%d\n",tab[re].adr);
                tarIn(code);
            }
            else{
                sprintf(code,"sub $t1,$s1,%d\n",tab[re].adr);
                tarIn(code);
            }
            tarright(right);
            tarIn("sll $t2,$t2,2\n");
            tarIn("sub $t1,$t1,$t2\n");
            tarIn("lw $t3,($t1)\n");
            tarresult(result);
        }
        else if(f == LIT){
            tarright(right);
            tarleft(left);
            tarIn("slt $t3,$t1,$t2\n");
            tarresult(result);
        }
        else if(f == LITEQU){
            tarright(right);
            tarleft(left);
            tarIn("sub $t4,$t2,$t1\n");
            tarIn("add $t5,$0,-1\n");
            tarIn("slt $t3,$t5,$t4\n");
            tarresult(result);
        }
        else if(f == GRT){
            tarright(right);
            tarleft(left);
            tarIn("slt $t3,$t2,$t1\n");
            tarresult(result);
        }
        else if(f == GRTEQU){
            tarright(right);
            tarleft(left);
            tarIn("sub $t4,$t1,$t2\n");
            tarIn("add $t5,$0,-1\n");
            tarIn("slt $t3,$t5,$t4\n");
            tarresult(result);
        }
        else if(f == EQUU){
            tarright(right);
            tarleft(left);
            tarIn("sub $t4,$t1,$t2\n");
            tarIn("add $t5,$0,-1\n");
            tarIn("slt $t4,$t5,$t4\n");
            tarIn("sub $t6,$t2,$t1\n");
            tarIn("slt $t6,$t5,$t6\n");
            tarIn("and $t3,$t4,$t6\n");
            tarresult(result);
        }
        else if(f == UNEQU){
            tarright(right);
            tarleft(left);
            tarIn("sub $t3,$t1,$t2\n");
            tarresult(result);
        }
        else if(f == ADD){
            tarright(right);
            tarleft(left);
            tarIn("add $t3,$t1,$t2\n");
            tarresult(result);
        }
        else if(f == SUB){
            tarright(right);
            tarleft(left);
            tarIn("sub $t3,$t1,$t2\n");
            tarresult(result);
        }
        else if(f == MULT){
            tarright(right);
            tarleft(left);
            tarIn("mult $t1,$t2\n");
            tarIn("mflo $t3\n");
            tarresult(result);
        }
        else if( f == DIV){
            tarright(right);
            tarleft(left);
            tarIn("div $t1,$t2\n");
            tarIn("mflo $t3\n");
            tarresult(result);
        }
        else if(f == BNZ){
            memset(code,'\0',MAX);
            tarleft(left);
            sprintf(code,"beq $t1,$0,%s\n",result);
            tarIn(code);
        }
        else if(f == BNE){
            memset(code,'\0',MAX);
            tarleft(left);
            spmovedown(4);//保存switch处的表达式结果不弹出
            tarright(right);
            sprintf(code,"bne $t1,$t2,%s\n",result);
            tarIn(code);
        }
        else if(f == PULL){
            spmoveup(4);
        }
        else if(f == JUMP){
            memset(code,'\0',MAX);
            if(strcmp(result,MAIN)==0){
                tarIn("move $s1,$sp\n");//函数基址的初始化
            }
            sprintf(code,"j %s\n",result);
            tarIn(code);
        }
        else if(f == SCANF){
            if(strcmp(left,"int")==0){
                tarIn("li $v0,5\n");
            }
            else if(strcmp(left,"char")==0){
                tarIn("li $v0,12\n");
            }
            tarIn("syscall\n");
            tarIn("move $t3,$v0\n");
            tarresult(result);
        }
        else if(f == PRINTF){
            if(strcmp(left,"string")==0){
                int j;
                int len = strlen(right);
                tarIn("li $v0,11\n");
                for(j = 0;j<len;j++){
                    memset(code,'\0',MAX);
                    sprintf(code,"li $a0,%d\n",right[j]);
                    tarIn(code);
                    tarIn("syscall\n");
                }
                memset(code,'\0',MAX);
                sprintf(code,"li $a0,%d\n",'\n');
                tarIn(code);
                tarIn("syscall\n");
            }
            else {
                tarright(right);
                tarIn("add $a0,$0,$t2\n");
                if(strcmp(left,INT)==0){
                    tarIn("li $v0,1\n");
                }
                else if(strcmp(left,CHAR)==0){
                    tarIn("li $v0,11\n");
                }
                tarIn("syscall\n");
                memset(code,'\0',MAX);
                tarIn("li $v0,11\n");
                sprintf(code,"li $a0,%d\n",'\n');
                tarIn(code);
                tarIn("syscall\n");
            }
        }
        else{

        }
    }
}
