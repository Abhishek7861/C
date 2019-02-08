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
    if((MAX_PAGE_SIZE-len)<0){
	    rid =  Table_Insert(tbl, record, len, &rid);
        printf("INSETRED A PAGE\n");
        memset(record, 0, sizeof(record));
        len=strlen(check);
    }
    strcat(record,check);
    memset(check, 0, sizeof(check));
    printf("rid:%d\n",rid);
    rid++;

	int population = atoi(tokens[2]);
    printf("%d\n",population);
    // AM_InsertEntry(population,rid);
	checkerr(err);
    }
    rid =  Table_Insert(tbl, record, len, &rid);
    printf("INSETRED A PAGE\n");
     int indexFD = tbl->FileDesc;
     fclose(fp);
     Table_Close(tbl);
     PF_CloseFile(indexFD);
     err = Table_Open(dbname, sch, true,&tbl);
     Table_Scan(tbl);
     checkerr(err);
     printf("DONE\n");
     return sch;
}

int main() {
    loadCSV();
}
