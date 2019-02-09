
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
void insertSTR(char* a,char* b,int p)
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

}


void substr(char *string,char *retstr,int pos,int length)
{

    int c=0;
    while(c<length)
    {
        retstr[c] = string[c+pos];
        c++;
    }    
}



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
        return retval;
    }else
    {
        printf("ERROR CREATING FILE OR FILE ALREADY EXISTS\n");
        return retval;
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
    int pageNum = 0;
    char BUFFER[MAX_PAGE_SIZE];
    char **pageBuf = &BUFFER;
    int freeSpace=0;
    int noOfRecord=0;
    char freeSpaceS[3];
    char noOfRecordS[3];
    char srid[3];

    int retval = PF_GetFirstPage(fileDes,&pageNum,pageBuf);
    if(retval!=PFE_OK)
    {
        retval = PF_AllocPage(fileDes,&pageNum,pageBuf);
        if(retval!=PFE_OK){
            printf("ERROR GETTING NEW PAGE\n");
            return -1;
        }
        // strcpy(BUFFER,*pageBuf);
        // memset(BUFFER,0,sizeof(BUFFER));
        freeSpace=MAX_PAGE_SIZE-6;
        noOfRecord = 0;
        EncodeInt(freeSpace,freeSpaceS);
        freeSpace = DecodeInt(freeSpaceS);
        printf("%d\n",freeSpace);
        EncodeInt(noOfRecord,noOfRecordS);
        insertSTR(*pageBuf,freeSpaceS,0);
        insertSTR(*pageBuf,noOfRecordS,3);    
        }
    else
    {
        //  strcpy(BUFFER,*pageBuf);
         substr(*pageBuf,freeSpaceS,0,3);
         substr(*pageBuf,noOfRecordS,3,6);
         freeSpace = DecodeInt(freeSpaceS);
         noOfRecord = DecodeInt(noOfRecordS);
    }
    
    if(freeSpace<strlen(record))
    {
        retval = PF_AllocPage(fileDes,&pageNum,pageBuf);
        if(retval!=PFE_OK){
            printf("ERROR GETTING NEW PAGE\n");
            return -1;
        }
        // strcpy(BUFFER,*pageBuf);
        freeSpace=MAX_PAGE_SIZE-8;
        noOfRecord = 0;
        EncodeInt(freeSpace,freeSpaceS);
        EncodeInt(noOfRecord,noOfRecordS);
        strcat(*pageBuf,freeSpaceS);
        strcat(*pageBuf,noOfRecordS);
    }    
    noOfRecord = noOfRecord;
    freeSpace = freeSpace-strlen(record);
    EncodeInt(freeSpace,freeSpaceS);
    EncodeInt(noOfRecord,noOfRecordS);
    EncodeInt(*rid,srid);
    insertSTR(*pageBuf,freeSpaceS,0);
    insertSTR(*pageBuf,noOfRecordS,3);
    insertSTR(*pageBuf,srid,6+(noOfRecord*3));
    insertSTR(*pageBuf,record,freeSpace);
    // strcpy(*pageBuf,BUFFER);
    retval = PF_UnfixPage(tbl->FileDesc,pageNum,true);
    if(retval==PFE_OK)
    {
        return *rid;
    }
    else
    {
        printf("ERROR UNFIXING PAGE\n");
        return -1;
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


