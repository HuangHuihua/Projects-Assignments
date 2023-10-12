#ifndef FILE_H
#define FILE_H

//#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "db.h"

typedef char *FileName;
//typedef int DataType;// the data in queue is the table oid

// Database *db_ = NULL;
// Conf * cf_ = NULL;

typedef struct FileArraystruct{
    //file pointer open file and read page
    UINT table_oid;//oid to recognaize the file belongs to which table
    FILE **fp;
}FileArraystruct;

//FIFO
typedef struct Queue{
    int queueSize;
    int front;//front pointer indicate the front of the queue, don't store data
    int rear;// end pointer indicate the end of the queue
    FileArraystruct *filedata;//store oid and file pointer

}Queue;

//Queue *cq = NULL;//queue


Queue* initQueue(Database *db, Conf*cf);

bool IsQueueFull(Queue* cq);

int searchFileInPool(Queue* cq, UINT oid);

FILE** returnfpInPool(Queue* cq,int slot);

bool IsQueueEmpty(Queue* cq);
bool IsQueueFull(Queue* cq);

bool popfront(Queue* cq);
bool pushEnd(Queue* cq, UINT oid, FILE **fp);

void closefile(UINT oid,  FILE **fp);

void replaceFileInQueue(Queue* cq, UINT oid, FILE** fp);

void releaseFileInPool(Queue* cq);

void releaseQueue(Queue* cq);

#endif