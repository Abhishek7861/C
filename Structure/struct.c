#include<stdio.h>


typedef struct
{
    char *msg;
} msgval;

typedef struct 
{
    msgval Msgval;
    int a;

}val;



typedef struct
{
    val num;
    int member;
} mystruct;


void myfunc(mystruct **data)
{
    (**data).member = 1;
    (**data).num.a=1;
    (**data).num.Msgval.msg="hello";
}


void main(int argc, char *argv)
{
    mystruct actualThing;
    mystruct *pointer = &actualThing;
    mystruct **data = &pointer;
    myfunc(data);

    printf("Member: %d\n", (*data)->member);
    printf("number: %d\n", (*data)->num.a);
    printf("number: %s\n", (*data)->num.Msgval.msg);
}