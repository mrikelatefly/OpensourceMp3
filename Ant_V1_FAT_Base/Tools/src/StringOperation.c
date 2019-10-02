#include "StringOperation.h"
//#include<stdlib.h>
/*
这里要实现的函数有
StringReverse 字符串反转顺序
SplitString
IntString
StringInt
FloatString
StringFloat
DoubleString
StringDouble
StringEqual
StringCopy
HasSymbol 判断是否含有某一个字符串
StringTrim

*/




//用于保存结果
char StringOperation_Buff[5][50]={'\0'};


//下面是一些字符串的操作相关的函数

void EmptyString(char *s){
	while(*s!='\0')
	{
		*s='\0';
		s++;
	}
}
char StringTrim(char sysmbol,char *s)
{
    char isTrim=0;
    char *p;
    p=s;
    while(*s!='\0')
    {
        if(*p==sysmbol)
            p++;
        else
        {
            isTrim=1;
            *s=*p;
            s++;
        }
        p++;
    }
    *s='\0';
    return isTrim;

}

char StringCopy(char *s2,char *s1)
{
    char isCopy=1;
    while(*s1!='\0')
    {
        *s2=*s1;
        s1++;
        s2++;
    }
    //s2++;
    *s2='\0';
    return isCopy;
}
//字符串顺序反转
char StringReverse(char *s)
{
    char i=0,*p;
    p=StringOperation_Buff[1];
    StringCopy(s,StringOperation_Buff[1]);
    while(*p!='\0')p++;
    p--;//跳过结束符
    while(*s!='\0')
    {
        *s=*p;
        s++;
        p--;
    }
    return i;
}
//拆分以后 不含拆分符号
char SplitString(char split,char *s)
{
	char i=0,j=0;
	while(*s!='\0')
	{
		if(*s==split)
		{
			//j++;
			StringOperation_Buff[i][j]='\0';
			i++;
			j=0;
			s++;
			continue;
		}
		StringOperation_Buff[i][j]=*s;
		j++;
		s++;
	}
	StringOperation_Buff[i][j]='\0';
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
char HasSymbol(char symbol,char *s)
{
	char isHave=0;
	while(*s!='\0')
	{
		if(*s==symbol)
		{
			isHave=1;
			break;
		}
		s++;
	}
	return isHave;
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
//转换结果有问题还未解决
char StringInt(int num)
{
	char i=0;
	while(num)
    {
        StringOperation_Buff[0][i]=num%10+'0';
        i++;
        num/=10;
    }
    StringOperation_Buff[0][i]='\0';
    StringReverse(StringOperation_Buff[0]);
	return i;//表示有几个数据被分离出来
}
float FloatString(char *s)
{
	float result=0;
	float num=0,pointer=0;
	while(*s!='\0')
	{
        if(*s=='.')
        {
            break;
        }
        if(*s-'0'>=0&&*s-'0'<=9)
        {
            num*=10;
            num+=(float)(*s-'0');
        }
        else
        {
            result=0;//TypeError
            return result;
        }
		s++;
	}
	if(HasSymbol('.',s))
    {
        while(*s!='\0')s++;//跳到尾
        s--;//跳过‘\0’
        while(*s!='.')
        {
            if(*s-'0'>=0&&*s-'0'<=9)
            {
                pointer/=10;
                pointer+=(double)((*s-'0')/10.0);
            }
            else
            {
                result=0;
                return result;
            }
            s--;
        }
    }
    result=num+pointer;
    return result;
}
//此方法有问题
char StringFloat(float number)
{
    char j=0;
    int num=(int)number;
    float pointer=number-num;
    while(num)
    {
        StringOperation_Buff[0][j]=num%10+'0';
        j++;
        num/=10;
    }
    StringOperation_Buff[0][j]='\0';
    StringReverse(StringOperation_Buff[0]);
    if(pointer)
    StringOperation_Buff[0][j]='.';
    while(pointer)
    {
        pointer*=10;
        num=(int)pointer;
        pointer-=num;
        j++;
        if(j>40)
        {
            break;
        }
        StringOperation_Buff[0][j]=num+'0';
    }
    j++;
    StringOperation_Buff[0][j]='\0';
    return j;//返回注册
}
double DoubleString(char *s)
{
	double result=0;
	double num=0,pointer=0;
	while(*s!='\0')
	{
        if(*s=='.')
        {
            break;
        }
        if(*s-'0'>=0&&*s-'0'<=9)
        {
            num*=10;
            num+=(double)(*s-'0');
        }
        else
        {
            result=0;//TypeError
            return result;
        }
		s++;
	}
	if(HasSymbol('.',s))
    {
        while(*s!='\0')s++;//跳到尾
        s--;//跳过‘\0’
        while(*s!='.')
        {
            if(*s-'0'>=0&&*s-'0'<=9)
            {
                pointer/=10;
                pointer+=(double)((*s-'0')/10.0);
            }
            else
            {
                result=0;
                return result;
            }
            s--;
        }
    }
    result=num+pointer;
    return result;
}
char StringDouble(double number)
{
    char j=0;
    int num=(int)number;
    double pointer=number-num;
    while(num)
    {
        StringOperation_Buff[0][j]=num%10+'0';
        j++;
        num/=10;
    }
    StringOperation_Buff[0][j]='\0';
    StringReverse(StringOperation_Buff[0]);
    if(pointer)
    StringOperation_Buff[0][j]='.';
    while(pointer)
    {
        pointer*=10;
        num=(int)pointer;
        pointer-=num;
        if(j>40)
        {
            break;//防止越界
        }
        j++;
        StringOperation_Buff[0][j]=num+'0';
    }
    j++;
    StringOperation_Buff[0][j]='\0';
    return j;
}

