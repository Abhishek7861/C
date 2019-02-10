#include <stdio.h>
#include <stdlib.h>
#include "codec.h"
#include "tbl.h"
#include "util.h"
#include "../pflayer/pf.h"
#include "../amlayer/am.h"
#define checkerr(err) {if (err < 0) {PF_PrintError(); exit(1);}}


void
printRow(void *callbackObj, RecId rid, byte *row, int len) {
    Schema *schema = (Schema *) callbackObj;
    byte *cursor = row;

    // UNIMPLEMENTED;
}

#define DB_NAME "toydb"
#define INDEX_NAME "dbindex.0"
	 
void
index_scan(Table *tbl, Schema *schema, int indexFD, int op, int value) {
    // UNIMPLEMENTED;
    /*
    Open index ...
    while (true) {
	find next entry in index
	fetch rid from table
        printRow(...)
    }
    close index ...
    */
}

int
main(int argc, char **argv) {
    char *schemaTxt = "Country:varchar,Capital:varchar,Population:int";
    Schema *schema = parseSchema(schemaTxt);
    Table *tbl = (Table*)malloc(sizeof(Table));
    char *dbname  = "toydb";
    if (argc == 2 && *(argv[1]) == 's') {
    int err = Table_Open(dbname, schema, true,&tbl);
     Table_Scan(tbl);
     Table_Close(tbl);
     PF_CloseFile(tbl->FileDesc);
    } else {
	// index scan by default
	int indexFD = PF_OpenFile(INDEX_NAME);
	int recnum= 0;
    int count1=0;
    int count2=0;
    int array1[500]; //population <100000
    int array2[500]; //population >100000
    char *pop="100000";
    int sd = AM_OpenIndexScan(indexFD,'i',sizeof(int),2,pop);
    while((recnum=AM_FindNextEntry(sd))>= 0) {
        printf("%d\n",recnum);
        array1[count1] = recnum;
        count1++;
    }
    printf("retrieved %d records\n",count1);

    sd = AM_OpenIndexScan(indexFD,'i',sizeof(int),3,pop);
    while((recnum=AM_FindNextEntry(sd))>= 0) {
        printf("%d\n",recnum);
        array2[count2] = recnum;
        count2++;
    }
    printf("retrieved %d records\n",count2);
    AM_CloseIndexScan(sd);
    PF_CloseFile(indexFD);
    
    printf("more than 100000\n");
    int err = Table_Open(dbname, schema, true,&tbl);
    char record[4000];
    for(int i=0;i<count1;i++)
        Table_Get(tbl,array1[i],record,MAX_TOKENS);
    printf("all\n");

    for(int i=0;i<count2;i++)
        Table_Get(tbl,array2[i],record,MAX_TOKENS);
    printf("all\n");
     Table_Close(tbl);
     PF_CloseFile(tbl->FileDesc);
    printf("less than 100000\n");
    }
}
