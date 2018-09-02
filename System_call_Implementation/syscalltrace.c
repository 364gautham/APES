#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>


int main(int argc,char** argv[])
{
	char a;
	char *mem;
	char str[10];
	char str1[10];
	//size_t len = sizeof(argv[1])+1;
	int len;
	FILE* fp;
	printf("APES Class Homework 2 by Gautham \n");
	fp = fopen("newfil.txt","r+");
	fp = fopen("newfil.txt","w");
	fputc(a,fp);
	fclose(fp);
	fp =fopen("newfil.txt","a+");
	mem = (char*)malloc(len * sizeof(char));
	printf("Enter string\n");
	scanf("%s",str1);
	//memcpy(str1,argv[1],len-1);
	
	fprintf(fp,"%s",str1);
	//fflush(fp);
	fclose(fp);
	fp = fopen("newfil.txt","r");
	a = getc(stdin);
	a = getc(fp);
	printf("%c\n",a);
	printf("Enter string for gets function\n");
	gets(str);	
	fclose(fp);
	free(mem);
	return 0;
}

