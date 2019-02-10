
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
void insertSTR(char* a,char* b,int p,int l)
{
int n = l;
for(int i=p;i<n;i++)
{
    a[i]=b[i-p];
}
}


void substr(char *string,char *retstr,int pos,int length)
{

    int c=pos;
    while(c<length)
    {
        retstr[c-pos] = string[c];
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
    int pageNum = 1;
    char *pageBuf=(char*)malloc(sizeof(char)*MAX_PAGE_SIZE);
    int freeSpace=0;
    int noOfRecord=0;
    char freeSpaceS[2]={0};
    char noOfRecordS[2]={0};
    char srid[3];

    int retval = PF_GetFirstPage(fileDes,&pageNum,&pageBuf);
    printf("err code %d\n",retval);
    if(retval!=PFE_OK)
    {  
        retval = PF_AllocPage(fileDes,&pageNum,&pageBuf);
        if(retval!=PFE_OK){
            printf("ERROR GETTING NEW PAGE\n");
            return -1;
        }
        // strcpy(BUFFER,*pageBuf);
        memset(pageBuf,' ',4000);
        // printf("%s\n",pageBuf);
        freeSpace=(MAX_PAGE_SIZE+1000)-4;
        noOfRecord = 1000;
        EncodeShort(freeSpace,freeSpaceS);
        freeSpace = DecodeShort(freeSpaceS);
        printf("%d\n",freeSpace);
        EncodeShort(noOfRecord,noOfRecordS);
        noOfRecord = DecodeShort(noOfRecordS);
        printf("%d\n",noOfRecord);
        insertSTR(pageBuf,freeSpaceS,0,2);
        insertSTR(pageBuf,noOfRecordS,2,4);
        // printf("%s\n",pageBuf);    
        }
    else
    {
        //  strcpy(BUFFER,*pageBuf);BUFFER
        char a[2];
        char b[2];
        printf("not in first alloc\n");
         substr(pageBuf,a,0,2);
         substr(pageBuf,b,2,4);
         freeSpace = (int)DecodeShort(a);
         noOfRecord = (int)DecodeShort(b);
    }
    
    if((freeSpace-1000)<(strlen(record)+400))
    {
        retval = PF_UnfixPage(tbl->FileDesc,pageNum,true);
        if(retval==PFE_OK)
        {
            printf("unfixed page no: %d\n",pageNum);
        }
        printf("get new page\n");
        int retval = PF_GetNextPage(fileDes,&pageNum,&pageBuf);
        if(retval==PFE_OK)
            {
         printf("working with new page\n");
         char a[2];
         char b[2];
         substr(pageBuf,a,0,2);
         substr(pageBuf,b,2,4);
         freeSpace = (int)DecodeShort(a);
         noOfRecord = (int)DecodeShort(b);
        }
        else
        {
        pageNum++;
        retval = PF_AllocPage(fileDes,&pageNum,&pageBuf);
        if(retval!=PFE_OK){
            printf("ERROR GETTING NEW PAGE\n");
            return -1;
        }
        printf("got new page\n");
        freeSpace=(MAX_PAGE_SIZE+1000)-4;
        noOfRecord = 1000;
        memset(pageBuf,' ',4000);
        EncodeShort(freeSpace,freeSpaceS);
        EncodeShort(noOfRecord,noOfRecordS);
        insertSTR(pageBuf,freeSpaceS,0,2);
        insertSTR(pageBuf,noOfRecordS,2,4);
        }
        
    }    
      int index;
      char indexs[2];
    freeSpace = freeSpace-strlen(record);
    printf("FREE Space :%d\n",freeSpace);
    noOfRecord = noOfRecord-1000;
    printf("NO of record %d\n",noOfRecord);
    if((noOfRecord)==0){
        index = MAX_PAGE_SIZE-strlen(record);
        EncodeShort(index,indexs);
        insertSTR(pageBuf,record,index,index+strlen(record));
        insertSTR(pageBuf,indexs,4,6);
    }
    else
    {
        substr(pageBuf,indexs,4+((noOfRecord-1)*2),2);
        index = DecodeShort(indexs);
        index = index-strlen(record)-1000;
        insertSTR(pageBuf,record,index,index+strlen(record));
        EncodeShort(index,indexs);
        insertSTR(pageBuf,indexs,4+(noOfRecord*2),6+(noOfRecord*2));
    }
    char buf[2];
    EncodeShort(freeSpace, buf);
    int l = DecodeShort(buf);
    // printf("%d \n", l);
    noOfRecord = (noOfRecord+1000)+1;
    printf("NO of record %d\n",noOfRecord);
    // EncodeShort(freeSpace,freeSpaceS);
    EncodeShort(noOfRecord,noOfRecordS);
    // freeSpace = DecodeShort(freeSpaceS);
    // printf("%d\n",freeSpace);
    insertSTR(pageBuf,buf,0,2);
    insertSTR(pageBuf,noOfRecordS,2,4);
    printf("%s\n",pageBuf);
    // strcpy(*pageBuf,BUFFER);
    retval = PF_UnfixPage(tbl->FileDesc,pageNum,true);
    printf("UNFIXED PAGE %d\n",pageNum);
    if(retval==PFE_OK)
    {
        int a = *rid;
        a++;
        return  a;
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
    int fileDes = tbl->FileDesc;
    int pageNum = 1;
    char *pageBuf=(char*)malloc(sizeof(char)*MAX_PAGE_SIZE);
    int freeSpace=0;
    int noOfRecord=0;
    char freeSpaceS[2]={0};
    char noOfRecordS[2]={0};

    int retval = PF_GetFirstPage(fileDes,&pageNum,&pageBuf);
    if(retval!=PFE_OK)
    {
        printf("ERROR READING PAGE\n");

    }
        char a[2];
        substr(pageBuf,a,2,4);
        noOfRecord = (int)DecodeShort(a);
        noOfRecord = noOfRecord-1000;
        for(int i=0;i<(noOfRecord);i++){
        char indexS[2];
        int index;
        char record[100];
        substr(pageBuf,indexS,4+(i*2),6+(i*2));
        // noOfRecord = (int)DecodeShort(a);
        index = (int)DecodeShort(indexS);
        int i=0;
        while(pageBuf[index]!='~')
        {
            record[i++] = pageBuf[index++];
        }
        record[i]='\0';
        printf("%s\n",record);
        printf("index %d\n",index);
        printf("no of record: %d\n",noOfRecord);
        }
    while(1)
    {
        int retval = PF_GetNextPage(fileDes,&pageNum,&pageBuf);
    if(retval!=PFE_OK)
    {
        printf("ERROR READING PAGE\n");
        break;
    }
        char a[2];
        substr(pageBuf,a,2,4);
        noOfRecord = (int)DecodeShort(a);
        noOfRecord = noOfRecord-1000;
        for(int i=0;i<(noOfRecord);i++){
        char indexS[2];
        int index;
        char record[100];
        substr(pageBuf,indexS,4+(i*2),6+(i*2));
        // noOfRecord = (int)DecodeShort(a);
        index = (int)DecodeShort(indexS);
        int i=0;
        while(pageBuf[index]!='~')
        {
            record[i++] = pageBuf[index++];
        }
        record[i]='\0';
        printf("%s\n",record);
        printf("index %d\n",index);
        printf("no of record: %d\n",noOfRecord);
        }
    }
        for(int i=0;i<pageNum;i++)
        {
          printf("unfix :%d\n",i); 
        }

    printf("DONE\n");
    // For each page obtained using PF_GetFirstPage and PF_GetNextPage
    //    for each record in that page,
    //          callbackfn(callbackObj, rid, record, recordLen)
}


