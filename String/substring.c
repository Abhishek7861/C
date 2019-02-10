#include<stdio.h>
#include<string.h>

char *substr(char *string,int pos,int length)
{
    char *sub = (char*)malloc(sizeof(char)*(length+1));
    int c=0;
    while(c<length)
    {
        sub[c] = string[c+pos-1];
        c++;
    }    
    sub[c] = '\0';
    return sub;

}


int main()
{
    char string[1000];
    char *sub;
    strcpy(string,"hello how are you\n");
    int length = strlen(string);
    sub = substr(string,5,5);
    puts(sub);
}