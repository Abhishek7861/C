
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tbl.h"
#include "codec.h"
#include "../pflayer/pf.h"

#define SLOT_COUNT_OFFSET 2
#define checkerr(err) {if (err < 0) {PF_PrintError(); exit(EXIT_FAILURE);}}

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
        (**ptable).schema = (Schema*)malloc(sizeof(schema));
        (**ptable).schema->columns = (ColumnDesc**)malloc(sizeof(ColumnDesc)*(*schema).numColumns);
        (*((**ptable).schema)).columns = (*schema).columns;
        (*((**ptable).schema)).numColumns = (*schema).numColumns;

        printf("FD :%d\n",(**ptable).FileDesc);
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

void
Table_Close(Table *tbl) {
    
    
}


int Table_Insert(Table *tbl, byte *record, int len, RecId *rid) {
    
    int fileDes = tbl->FileDesc;
    int pageNO = 0;
    int *pageNum=&pageNO;
    char **pageBuf;
    int retval = PF_AllocPage(fileDes,pageNum,pageBuf);
    if(retval==PFE_OK)
    {
        strcpy(*pageBuf,record);
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

void Table_Scan(Table *tbl, void *callbackObj, ReadFunc callbackfn) {

    

    // For each page obtained using PF_GetFirstPage and PF_GetNextPage
    //    for each record in that page,
    //          callbackfn(callbackObj, rid, record, recordLen)
}


