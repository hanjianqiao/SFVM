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
	char*address = strDest;
	while (*strDest)
	{
		strDest++;
	}

	while (*strDest++ = *strSrc++)
	{
	}
	return address;
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