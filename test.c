#include<stdio.h>
int main()
{
	int k;
	scanf("%d",&k);
while(k--){
	int n=0,i,j;
	int res=0;
	scanf("%d",&n);
	for(i=n;i>=n;i++){
		res = palindrome(i);
		if(res==1)
		break;
	}

	for(j=n;j<=n;j--){
		res = palindrome(j);
		if(res==1)
		break;
	}
if((i-n)<(n-j))
	printf("%d\n",i);
else
	printf("%d\n",j);

}
	return 0;
}


int palindrome(int val)
{
    int a = val;
    int digit=0,res=0;
    while(a)
    {
        digit = a%10;
        a = a/10;
        res = res*10+digit;
    }
    if(res==val)
    return 1;
    else
    return 0;
}
