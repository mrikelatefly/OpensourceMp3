#ifndef __StringOperation_H
#define __StringOperation_H


/*
这里要实现的函数有
EmptyString
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
void EmptyString(char *s);
char StringTrim(char sysmbol,char *s);
char StringCopy(char *s2,char *s1);
char StringReverse(char *s);
char SplitString(char split,char *s);
char StringEqual(char *s1,char *s2);
char HasSymbol(char symbol,char *s);
int IntString(char *s);
char StringInt(int num);
float FloatString(char *s);
char StringFloat(float number);
double DoubleString(char *s);
char StringDouble(double number);







#endif
