#include<stdio.h>
#include <stdlib.h> 


typedef struct
{
    char *msg;
} msgval;

typedef struct 
{
    msgval **Msgval;
    int a;

}val;



typedef struct
{
    val num;
    int member;
} mystruct;


void myfunc(mystruct **data, val *value)
{
    
    (**data).member = 1;
    (**data).num.a = (*value).a;
    (**data).num.Msgval[0] = (*value).Msgval[0];
    (**data).num.Msgval[1] = (*value).Msgval[1];
    (**data).num.Msgval[2] = (*value).Msgval[2];
}


void main(int argc, char *argv)
{
    mystruct actualThing;
    mystruct *pointer = &actualThing;
    mystruct **data = &pointer;
    val value;
    value.a = 5;
    msgval **array;
    array = (msgval**)malloc(sizeof(msgval)*3);
    array[0].msg="hello";
    array[1].msg="bbye";
    array[2].msg="good";
    value.Msgval= array;

    myfunc(data,&value);

    printf("Member: %d\n", (*data)->member);
    printf("number: %d\n", (*data)->num.a);
    printf("number: %s\n", (*data)->num.Msgval[2]->msg);
}


