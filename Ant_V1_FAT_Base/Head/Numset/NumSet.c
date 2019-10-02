#include "NumSet.h"
#include "usart.h"
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


//字符串顺序反转
char StringReverse(char *s)
{
    char i=0;
    return i;
}
char StringCopy(char *s1,char *s2)
{
    char isCopy=1;
    while(*s1!='\0')
    {
        *s2=*s1;
        s1++;
        s2++;
    }
    s2++;
    *s2='\0';
    return isCopy;
}
int IntString(char *s)
{
	int num=0;
	while(*s!='\0')
	{
		if(*s-'0'>=0&&*s-'0'<=9)
		{
			num*=10;
			num+=(int)(*s-'0');
		}
		else
			return 0;
		s++;
	}
	return num;
}
//这个结果将会保存在字符数组里面
char StringInt(int num)
{
	char i=0,n=num;
	while(n)
    {
        i++;
        n/=10;
    }
    i++;
    Num_Set_Buff[0][i]='\0';//加入结束符
    while(num)
    {
        i--;
        Num_Set_Buff[0][i]=num%10+'0';
        num/=10;

    }
	return i;//表示有几个数据被分离出来
}

char SplitString(char split,char *s)
{
	char i=0,j=0;
	while(*s!='\0')
	{
		if(*s==split)
		{
			//j++;
			Num_Set_Buff[i][j]='\0';
			i++;
			j=0;
			s++;
			continue;
		}
		Num_Set_Buff[i][j]=*s;
		j++;
		s++;
	}
	Num_Set_Buff[i][j]='\0';
	//printf("I am here\n");
	//printf("order:%s content:%s",Num_Set_Buff[0],Num_Set_Buff[1]);
	return i;
}

char StringEqual(char *s1,char *s2)
{
	char isSame=1;
	while(*s1!='\0'&&*s2!='\0')
	{
		if(*s1-*s2)
		{
			isSame=0;
			break;
		}
		s1++;
		s2++;
	}
	if(*s1!='\0'||*s2!='\0')
	{
		isSame=0;
	}
	return isSame;
}
void NumSetInit()
{
    int i=0;
    for(i=0;i<_String_Set_Size;i++)
    {
        StringSet[i].isUsed=0;
    }
    for(i=0;i<_Num_Set_Size;i++)
    {
         NumSetNum[i].isUsed=0;
         NumSetNum[i].vary=NULL;
    }
}
char NumSetAdd(char *varibleName,int *p)
{
	char isAdd=1,i=0;//默认为1表示添加成功
    //SplitString(' ',Num_Set_Buff[1]);
    for(i=0;i<_Num_Set_Size;i++)
    {
        if(NumSetNum[i].isUsed==1)
        {
            if(StringEqual(varibleName,NumSetNum[i].variableName))
            {
                isAdd=0;
                return isAdd;//重名
            }
        }
        else if(NumSetNum[i].isUsed==0)
        {
            //拷贝字符串
            StringCopy(varibleName,NumSetNum[i].variableName);
            NumSetNum[i].vary=p;
            NumSetNum[i].value=*p;
            NumSetNum[i].isUsed=1;
            break;
        }
    }
    return isAdd;
}
char StringSetAdd(char *variableName,char *s)
{

    char isAdd=1,i=0;//默认为1表示添加成功
    for(i=0;i<_String_Set_Size;i++)
    {
        if(StringSet[i].isUsed==1)
        {
            if(StringEqual(variableName,StringSet[i].variableName))
            {
                isAdd=0;
                return isAdd;//重名
            }
        }
        else if(StringSet[i].isUsed==0)
        {
            //拷贝字符串
            StringCopy(variableName,StringSet[i].variableName);
            StringCopy(s,StringSet[i].String);
            StringSet[i].isUsed=1;
            break;
        }
    }
    return isAdd;

}
char NumSetDelete(char *variableName)
{
	char isDelete=1;
    int i;
    for(i=0;i<_Num_Set_Size;i++)
    {
        if(StringEqual(variableName,NumSetNum[i].variableName))
        {
            NumSetNum[i].isUsed=0;
            break;
        }
    }
    if(i==_Num_Set_Size)
    {
        isDelete=0;
    }
    return isDelete;
}
char NumSetCreate(char *variableName,int num)
{
    char isCreate=1;
    int i;
    for(i=0;i<_Num_Set_Size;i++)
    {
        if(NumSetNum[i].isUsed==1)
        {
            if(StringEqual(variableName,NumSetNum[i].variableName))
            {
                isCreate=0;
                return isCreate;
                //break;
            }
        }
    }
    for(i=0;i<_Num_Set_Size;i++)
    {
        if(NumSetNum[i].isUsed==0)
        {
            StringCopy(variableName,NumSetNum[i].variableName);
            NumSetNum[i].value=num;
            NumSetNum[i].isUsed=1;
            break;
        }
    }
    if(i==_Num_Set_Size)
    {
        isCreate=0;
    }
    return isCreate;
}
char NumSetSet(char *variableName,int num)
{
    char isSet=1;
    int i;
    for(i=0;i<_Num_Set_Size;i++)
    {
        if(StringEqual(variableName,NumSetNum[i].variableName))
        {

            if(NumSetNum[i].vary==NULL)
            {
                NumSetNum[i].value=num;
            }
            else
            {
                *(NumSetNum[i].vary)=num;
                NumSetNum[i].value=num;
            }
            break;
        }
    }
    if(i==_Num_Set_Size)
    {
        isSet=0;
    }
    return isSet;
}

void ShowAllNumAndString()
{
    int i=0;
    //printf("Show all now!\n");
    for(i=0;i<_Num_Set_Size;i++)
    {
        if(NumSetNum[i].isUsed==1)
        {	
						//更新数据
						if(*(NumSetNum[i].vary)!=NumSetNum[i].value&&NumSetNum[i].vary!=NULL)
						{
							NumSetNum[i].value=*(NumSetNum[i].vary);
						}
            printf("variableName:%s   ",NumSetNum[i].variableName);
            printf("value:%d\r\n",NumSetNum[i].value);
        }
    }
    for(i=0;i<_String_Set_Size;i++)
    {
        if(StringSet[i].isUsed==1)
        {
            printf("variableName:%s   ",StringSet[i].variableName);
            printf("String:%s\r\n",StringSet[i].String);
        }
    }
}
void ShowVariable(char *variableName)
{
    int i=0;
    for(i=0;i<_Num_Set_Size;i++)
    {
        if(NumSetNum[i].isUsed==1)
        {
            if(StringEqual(variableName,NumSetNum[i].variableName))
            {
                printf("variableName:%s    ",NumSetNum[i].variableName);
                printf("value:%d\r\n",NumSetNum[i].value);
            }
        }
    }
    for(i=0;i<_String_Set_Size;i++)
    {
        if(StringSet[i].isUsed==1)
        {
            if(StringEqual(variableName,StringSet[i].variableName))
            {
                printf("variableName:%s    ",StringSet[i].variableName);
                printf("String:%s\r\n",StringSet[i].String);
            }
        }
    }
}
//下面就正式进行设置
char NumSetFunction(char *s)
{
    char isOrder=0,error=0;
    SplitString(':',s);//注意这里由于调试环境无法用空格拆分字符串
    if(StringEqual("create",Num_Set_Buff[0]))
    {
        isOrder=Create;
    }
    else if(StringEqual("set",Num_Set_Buff[0]))
    {
        isOrder=Set;
    }
    else if(StringEqual("delete",Num_Set_Buff[0]))
    {
        isOrder=Delete;
    }
    else if(StringEqual("show",Num_Set_Buff[0]))
    {
        isOrder=Show;
    }
    //printf("%s\n",Num_Set_Buff[0]);
    switch(isOrder)
    {
        case Create:printf("\r\nCreate Now\n");
            /*
                if(StringEqual("String",Num_Set_Buff[1]))
                {
                    SplitString('=',Num_Set_Buff[2]);
                    StringSetAdd
                }
            */
            SplitString('=',Num_Set_Buff[1]);
            //printf("%s  %d",Num_Set_Buff[0],IntString(Num_Set_Buff[1]));
            NumSetCreate(Num_Set_Buff[0],IntString(Num_Set_Buff[1]));
        break;

        case Delete:printf("\r\nDelete Now\r\n");
            NumSetDelete(Num_Set_Buff[1]);
        break;

        case Set:printf("\r\nSet Now\r\n");
            SplitString('=',Num_Set_Buff[1]);
            NumSetSet(Num_Set_Buff[0],IntString(Num_Set_Buff[1]));
        break;

        case Show:ShowAllNumAndString();
        break;

        case 0:printf("\r\nInput ERROR!\r\n");
    }
    return error;
}

