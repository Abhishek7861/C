#include<stdio.h>
#include <stdlib.h> 


typedef struct {
    char *msg;
} msgval;

typedef struct  {
    msgval **Msgval;
    int a;
}val;



typedef struct {
    val num;
    int member;
} mystruct;


void myfunc(mystruct **data, val *value) {
    (**data).member = 1;
    (**data).num.a = (*value).a;
    (**data).num.Msgval  = (*value).Msgval;
}


void main(int argc, char *argv) {
    
    mystruct actualThing;
    mystruct *pointer = &actualThing;
    mystruct **data = &pointer;
    val value;
    value.a = 5;
    msgval **array;
    array = (msgval**)malloc(3 * sizeof(msgval));
    array[0] = (char*)malloc(10 * sizeof(char));
    array[1] = (char*)malloc(10 * sizeof(char));
    array[2] = (char*)malloc(10 * sizeof(char));
    
    (*array[0]).msg="hello";
    (*array[1]).msg="bbye";
    (*array[2]).msg="good";

    value.Msgval= array;

    myfunc(data,&value);

    printf("Member: %d\n", (*data)->member);
    printf("number: %d\n", (*data)->num.a);
    printf("number: %s\n", (*data)->num.Msgval[0]->msg);
}


