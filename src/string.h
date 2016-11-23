#ifndef STRING_H
#define STRING_H
int stringcmp(const char *str1, const char *str2);
char *stringcat(char *strDest, const char *strSrc);
char *stringcpy(char *des, int size, const char *src);
char *stringcpy(char *des, const char *src);
#endif