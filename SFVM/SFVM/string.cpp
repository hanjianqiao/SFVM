#include "string.h"
int stringcmp(const char *str1, const char *str2)
{
	while (*str1 == *str2)
	{
		if (*str1 == '\0')
			return 0;
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

char *stringcat(char *strDest, const char *strSrc)
{
	//后文returnaddress，故不能放在assert断言之后声明address
	char*address = strDest;
	while (*strDest)//是while(*strDest!=’\0’)的简化形式
	{
		//若使用while(*strDest++)，则会出错，因为循环结束后strDest还会执行一次++,
		//那么strDest将指向'\0'的下一个位置。/所以要在循环体内++；因为要是*strDest最后指
		//向该字符串的结束标志’\0’。
		strDest++;
	}

	while (*strDest++ = *strSrc++)
	{
		//NULL;//该循环条件内可以用++，
	}//此处可以加语句*strDest=’\0’;无必要
	return address;//为了实现链式操作，将目的地址返回
}
char *stringcpy(char *des, int size, const char *src){
	char *r = des;
	while ((size-- > 0) && (*des++ = *src++));
	return r;
}
char *stringcpy(char *des, const char *src){
	char *r = des;
	while (*des++ = *src++);
	return r;
}