#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "codec.h"
#include "../pflayer/pf.h"
#include "../amlayer/am.h"
#include "tbl.h"
#include "util.h"

#define checkerr(err) {if (err < 0) {PF_PrintError(); exit(1);}}

#define MAX_PAGE_SIZE 4000


#define DB_NAME "data.db"
#define INDEX_NAME "data.db.0"
#define CSV_NAME "data.csv"


/*
Takes a schema, and an array of strings (fields), and uses the functionality
in codec.c to convert strings into compact binary representations
 */
int encode(Schema *sch, char **fields, byte *record, int spaceLeft) {
    // UNIMPLEMENTED;
       spaceLeft = spaceLeft+strlen(fields[0])+1;
       spaceLeft = spaceLeft+strlen(fields[1])+1;
       spaceLeft = spaceLeft+strlen(fields[2])+2;
    //    printf("%s\n",record);
       return spaceLeft;

    // for each field
    //    switch corresponding schema type is
    //        VARCHAR : EncodeCString
    //        INT : EncodeInt
    //        LONG: EncodeLong
    // return the total number of bytes encoded into record
}

Schema *loadCSV() {
    int ridarray[500];
    int populationarray[500];
    int index=0;
    // Open csv file, parse schema
    FILE *fp = fopen(CSV_NAME, "r");
    if (!fp) {
	perror("data.csv could not be opened");
        exit(EXIT_FAILURE);
    }

    char buf[MAX_LINE_LEN];
    char *line = fgets(buf, MAX_LINE_LEN, fp);
    if (line == NULL) {
	fprintf(stderr, "Unable to read data.csv\n");
	exit(EXIT_FAILURE);
    }
    printf("file opened\n");

    // Open main db file
    Schema *sch = parseSchema(line);
    Table *tbl;
    tbl = (Table*)malloc(sizeof(Table));
    char *dbname  = "toydb";
    int err = Table_Open(dbname, sch, true,&tbl);
    printf("Table opened\n");
    char *tokens[MAX_TOKENS];
    char record[MAX_PAGE_SIZE];
    char check[MAX_PAGE_SIZE];
    RecId rid=0;
    int len=0;
    int headerindex=8;
    int recordcount = 0;
    int freespace=3992;
    int recordindex=3999;
    while (((line = fgets(buf, MAX_LINE_LEN, fp))) !=NULL){
	int n = split(line, ",", tokens);
        // printf("%s\n",*tokens);
	    // assert (n == sch->numColumns);
	    len = len+encode(sch, tokens, record, 0); 
        strcat(check,tokens[0]);
        strcat(check,":");
        strcat(check,tokens[1]);
        strcat(check,":");
        strcat(check,tokens[2]);
        strcat(check,"~");
        strcat(record,check);
	    rid =  Table_Insert(tbl, check, len, &rid);
        if(rid==(-1))
        printf("CANT INSERT A RECORD\n");
        else
        printf("INSERTED :: %s\n",record);
        memset(check, 0, sizeof(check));
        memset(record, 0, sizeof(record));
        printf("rid:%d\n",rid);

	    int population = atoi(tokens[2]);
        printf("%d\n",population);
        ridarray[index]=rid-1;
        populationarray[index++]=population;
    // AM_InsertEntry(population,rid);
	// checkerr(err);
    }
     int indexFD = tbl->FileDesc;
     fclose(fp);
     Table_Close(tbl);
     int a = PF_CloseFile(indexFD);
     if(a==PFE_OK)
     printf("SUCCESSFULLY CLOSED FILE\n");
    //  err = Table_Open(dbname, sch, true,&tbl);
     
    //  Table_Get(tbl,311,record,len);
    //  Table_Scan(tbl);
    //  PF_CloseFile(indexFD);
    //  Table_Close(tbl);
    //  checkerr(err);
     printf("DONE FORMING SLOTTED PAGE STRUCTURE\n");

    int fd;	/* file descriptor for the index */
    char fname[80];	/* file name */
    int recnum;	/* record number */
    int sd;	/* scan descriptor */
    int numrec;	/* # of records retrieved */
    int testval;
    char * RELNAME = "dbindex";
    /* init */
    printf("initializing\n");
    PF_Init();

    /* create index */
    printf("creating index\n");
    AM_CreateIndex(RELNAME,0,'i',sizeof(int));

    /* open the index */
    printf("opening index\n");
    sprintf(fname,"%s.0",RELNAME);
    fd = PF_OpenFile(fname);

    /* first, make sure that simple deletions work */
    printf("inserting into index\n");
    for (recnum=0; recnum < index; recnum++) {
       int a= AM_InsertEntry(fd,'i',sizeof(int),(char *)&populationarray[recnum],ridarray[recnum]);
    }
     printf("DONE FORMING INDEX\n");

    PF_CloseFile(fd);

     return sch;
}

int main() {
    loadCSV();
}
