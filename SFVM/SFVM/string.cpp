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
	//����returnaddress���ʲ��ܷ���assert����֮������address
	char*address = strDest;
	while (*strDest)//��while(*strDest!=��\0��)�ļ���ʽ
	{
		//��ʹ��while(*strDest++)����������Ϊѭ��������strDest����ִ��һ��++,
		//��ôstrDest��ָ��'\0'����һ��λ�á�/����Ҫ��ѭ������++����ΪҪ��*strDest���ָ
		//����ַ����Ľ�����־��\0����
		strDest++;
	}

	while (*strDest++ = *strSrc++)
	{
		//NULL;//��ѭ�������ڿ�����++��
	}//�˴����Լ����*strDest=��\0��;�ޱ�Ҫ
	return address;//Ϊ��ʵ����ʽ��������Ŀ�ĵ�ַ����
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