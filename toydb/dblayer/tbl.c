
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tbl.h"
#include "codec.h"
#include "../pflayer/pf.h"

#define SLOT_COUNT_OFFSET 2
#define checkerr(err) {if (err < 0) {PF_PrintError(); exit(EXIT_FAILURE);}}
#define MAX_PAGE_SIZE 4000

int  getLen(int slot, byte *pageBuf);
int  getNumSlots(byte *pageBuf);
void setNumSlots(byte *pageBuf, int nslots);
int  getNthSlotOffset(int slot, char* pageBuf);


/**
   Opens a paged file, creating one if it doesn't exist, and optionally
   overwriting it.
   Returns 0 on success and a negative error code otherwise.
   If successful, it returns an initialized Table*.
 */
int Table_Open(char *dbname, Schema *schema, bool overwrite, Table **ptable)
{
    PF_Init();
    int retval = PF_CreateFile(dbname);
    if(1)
    {   
        (**ptable).FileDesc=PF_OpenFile(dbname);
        (**ptable).tablename = dbname;
        memset((**ptable).dirtypageNUM,0,sizeof((**ptable).dirtypageNUM));
        (**ptable).pages=0;
        (**ptable).schema = (Schema*)malloc(sizeof(schema));
        (**ptable).schema->columns = (ColumnDesc**)malloc(sizeof(ColumnDesc)*(*schema).numColumns);
        (*((**ptable).schema)).columns = (*schema).columns;
        (*((**ptable).schema)).numColumns = (*schema).numColumns;
    }else
    {
        printf("ERROR CREATING FILE OR FILE ALREADY EXISTS\n");
    }
    
    // Initialize PF, create PF file,
    // allocate Table structure  and initialize and return via ptable
    // The Table structure only stores the schema. The current functionality
    // does not really need the schema, because we are only concentrating
    // on record storage. 
}

void Table_Close(Table *tbl) {
    for(int i=0;i<tbl->pages;i++)
        {
           int retval = PF_UnfixPage(tbl->FileDesc,i,tbl->dirtypageNUM[i]);
           if(retval!=PFE_OK)
           {
               printf("ERROR UNFIXING PAGE\n");
           }
           else
           {
               printf("FIXED PAGE %d\n",i);
           }
           
        }
    int retval = PF_CloseFile(tbl->FileDesc);
    if(retval!=PFE_OK)
    {
        printf("ERROR CLOSING FILE\n");
    }
    else
    {
        printf("CLOSED FILE\n");
    }
    
    
}


int Table_Insert(Table *tbl, byte *record, int len, RecId *rid) {
    
    int fileDes = tbl->FileDesc;
    int pageNO = 0;
    tbl->pages = tbl->pages+1;
    tbl->dirtypageNUM[pageNO]=1;
    int *pageNum=&pageNO;
    char BUFFER[MAX_PAGE_SIZE];
    char **pageBuf = &BUFFER;
    int retval = PF_AllocPage(fileDes,pageNum,pageBuf);
    if(retval==PFE_OK)
    {
        strcpy(*pageBuf,record);
        memset(record, 0, sizeof(record));
        printf("%s INTO A PAGE %d\n",*pageBuf,*pageNum);
        int retval = ((*pageNum)<<8)+(*rid);
        return retval;
    }
    else
    {
        printf("ERROR ALLOCATING PAGE");
    }
    
    // Allocate a fresh page if len is not enough for remaining space
    // Get the next free slot on page, and copy record in the free
    // space
    // Update slot and free space index information on top of page.
}

#define checkerr(err) {if (err < 0) {PF_PrintError(); exit(EXIT_FAILURE);}}

/*
  Given an rid, fill in the record (but at most maxlen bytes).
  Returns the number of bytes copied.
 */
int Table_Get(Table *tbl, RecId rid, byte *record, int maxlen) {
    int slot = rid & 0xFFFF;
    int pageNum = rid >> 16;

    // UNIMPLEMENTED;
    // PF_GetThisPage(pageNum)
    // In the page get the slot offset of the record, and
    // memcpy bytes into the record supplied.
    // Unfix the page
    // return len; // return size of record
    return 1;
}

void Table_Scan(Table *tbl) {
    int errVal;
    int fileDes = tbl->FileDesc;
    int pageNum = 0;
    int pages = tbl->pages;
    char BUFFER[MAX_PAGE_SIZE+2];
    char *tokens[4000];
    int i=0;
    char string[100];
    char **pageBuf = &BUFFER;
    printf("%s:%s:%s\n",tbl->schema->columns[0]->name,tbl->schema->columns[1]->name,tbl->schema->columns[2]->name);
    errVal = PF_GetFirstPage(fileDes,&pageNum,pageBuf);
    strcpy(BUFFER,*pageBuf);
    // strcat(BUFFER,"<>");
        // printf("%s\n",BUFFER);
        // printf("HELLO\n");
    int n = split(BUFFER, "~", tokens);
    // memset(BUFFER, 0, sizeof(BUFFER));
    while(tokens[i]!=NULL){
        strcpy(string,tokens[i]);
        tokens[i++]=NULL;
        printf("%s\n",string);
    }
    // memset(BUFFER, 0, sizeof(BUFFER));
    for(i=pageNum;i<=pageNum;i++)
    {
        errVal = PF_GetNextPage(fileDes,&pageNum,pageBuf);
        if(errVal==PFE_OK)
        {
            strcpy(BUFFER,*pageBuf);
            // printf("%s\n",BUFFER);
                n = split(BUFFER, "~", tokens);
            while(tokens[i]!=NULL){
                 strcpy(string,tokens[i]);
                tokens[i++]=NULL;
                printf("%s\n",string);
            } 
            // memset(BUFFER, 0, sizeof(BUFFER));
        }
        else
        {
            printf("ERROR GETTING NEXT PAGE\n");
        }
        
    }
    printf("DONE\n");
    // For each page obtained using PF_GetFirstPage and PF_GetNextPage
    //    for each record in that page,
    //          callbackfn(callbackObj, rid, record, recordLen)
}


