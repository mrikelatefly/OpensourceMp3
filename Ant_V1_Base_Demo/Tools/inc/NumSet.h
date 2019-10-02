#ifndef __NUMSET_H
#define __NUMSET_H
/*
numset 调试框架将会加入到function的中以便获得更好的效果
在有标识的状态下有
set:variableName=number;
create:[ variableType] variableName[=vary];
delete:variableName;
check:variableName[all];

不过数组的大小还有待考证

这些基本的操作只需要将变量的地址加入到要操作的链表中就可以进行操作

对于一个输入的一般的分解步骤
1 拆分为命令 操作 变量
2 将拆分的结果保存到数组
3 调用函数进行操作
*/


#define _Num_Set_Size  100
#define _String_Set_Size 50

/*
下面是命令标识符
*/
#define Create 1
#define Delete 2
#define Show   3
#define Set    4


//在外界可以直接分配变量进入
struct __Num_Set
{
	char isUsed;
	int *vary;
	int value;
	char variableName[20];
};
struct __String_Set
{
	char isUsed;
	char variableName[20];
	char String[50];
};

//下面直接用数组代替链表便于操作但是
//用于保存结果
extern char Num_Set_Buff[5][50];
extern struct __String_Set StringSet[_String_Set_Size];
extern struct __Num_Set NumSetNum[_Num_Set_Size];


char StringReverse(char *s);
char StringCopy(char *s1,char *s2);
int IntString(char *s);
char StringInt(int num);
char SplitString(char split,char *s);
char StringEqual(char *s1,char *s2);
void NumSetInit(void);
char NumSetAdd(char *varibleName,int *p);
char StringSetAdd(char *variableName,char *s);
char NumSetDelete(char *variableName);
char NumSetCreate(char *variableName,int num);
char NumSetSet(char *variableName,int num);
void ShowAllNumAndString(void);
void ShowVariable(char *variableName);
char NumSetFunction(char *s);
#endif

