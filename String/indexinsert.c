/* Write a C program to insert a sub-string in to given main string from a given position.
*/

#include <stdio.h>
#include <string.h>

char* insertSTR(char* a,char* b,int p)
{
char c[4000];
int r=0,i=0;
int t=0;
int x,g,s,n,o;

r = strlen(a);
n = strlen(b);
i=0;

while(i <= r)
{
 c[i]=a[i];
 i++;
}
s = n+r;
o = p+n;

for(i=p;i<s;i++)
{
 x = c[i];
 if(t<n)
 {
  a[i] = b[t];
  t=t+1;
 }
 a[o]=x;
 o=o+1;
}
return a;

}


int main()
{
    char a[4000];
    char b[4000]="0000";
    int p=2;
    insertSTR(a,b,0);
    insertSTR(a,b,0);
    printf("%s\n", a);
}

