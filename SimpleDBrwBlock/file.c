//file replacement policy
//cf->file limit
//db.c line 186: assume each table has only one file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "file.h"

Database *db_file = NULL;
Conf * cf_file = NULL;

FILE**FFPP[1000];

Queue* initQueue(Database* db ,Conf* cf ){
    // printf("initQueue");
    db_file = db;
    cf_file = cf;
    Queue* cq =  malloc(sizeof(Queue));
    cq->front = -1;
    cq->rear = 0;
    cq->queueSize = cf_file->file_limit;
    cq->filedata = (FileArraystruct*)malloc(sizeof(FileArraystruct)*(cq->queueSize));
    for(int i = 0; i < cq->queueSize; i++){
        cq->filedata[i].table_oid = 0;
        cq->filedata[i].fp = (FILE**)malloc(sizeof(FILE*) * 1);
        *(cq->filedata[i].fp) = NULL;
        FFPP[i] = cq->filedata[i].fp;
    }


    // for(int i = 0; i < cq->queueSize; i++){
    //     cq->filedata[i].table_oid = -1;
    //     cq->filedata[i].fp = NULL;
    // }

    return cq;
}

void releaseQueue(Queue* cq){
    for(int i = 0; i < cq->queueSize; i++){
        // closefile(cq->filedata[i].table_oid, cq->filedata[i].fp);
        cq->filedata[i].table_oid = 0;
        // fclose(*(cq->filedata[i].fp));
        free(cq->filedata[i].fp);

    }

    free(cq->filedata);
    free(cq);
}


int searchFileInPool(Queue* cq, UINT oid){
    for(int i = 0; i < cq->queueSize; i++){
        if(cq->filedata[i].table_oid == oid) return i;
    }
    return -1;
}

FILE** returnfpInPool(Queue* cq,int slot){
    // printf("search file in pool");
    return cq->filedata[slot].fp;
}


bool IsQueueEmpty(Queue* cq){
    if(cq -> front == cq ->rear ) return true;
    else return false;
}
// bool IsOneInQueueEmpty(int slot){
//     if(cq->filedata[slot] == -1) return true;
//     return false;
// }

bool IsQueueFull(Queue* cq){
    if(cq->front == (cq->rear+1)% cq->queueSize )return true;
    return false;
}

// Queue* getQueue(){
//     return cq;
// }

void openfile(UINT oid, FILE **fp){

    log_open_file(oid);
    
    UINT FileName;
    FileName = oid;
    char file_path[200];
    sprintf(file_path,"%s/%u",db_file->path, FileName);
    // open file read only
    *fp = fopen(file_path, "r"); 
    //pushEnd(fp);
    if(*fp == NULL){
        printf("The file cannot open %s \n", file_path);
    }
    // printf("file pointer in file.c openfile %s\n", *fp);
    
}

void closefile(UINT oid,  FILE **fp){

    log_close_file(oid);
    fclose(*fp);
    *fp = NULL;
}

bool popfront(Queue* cq){
    //popfront first then push end to ensure previous file close first
    if(IsQueueEmpty(cq)) return false;
    //pop means front point move forward , chasing rear
    closefile(cq->filedata[cq->front+1].table_oid,cq->filedata[cq->front+1].fp);
    *(cq->filedata[cq->front+1].fp) = NULL;
    cq->front = (cq->front+1) %cq->queueSize;
    return true;
}

//only when file not in queue we push file in the end of circle queue
bool pushEnd(Queue* cq, UINT oid, FILE **fp){

    if(IsQueueFull(cq)) return false;
    cq->rear = (cq->rear + 1) % cq->queueSize;
    cq->filedata[cq->rear].table_oid = oid;
    openfile(cq->filedata[cq->rear].table_oid,fp);
    cq->filedata[cq->rear].fp = fp;
    //don't need loop,'cause we open file only when need
    //every file in queue we open it
    return true;
    
}


void replaceFileInQueue(Queue* cq, UINT oid, FILE** fp){
    
    popfront(cq);

    pushEnd(cq, oid, fp);
}


void releaseFileInPool(Queue* cq){

    //free(cq->filedata)
    for(int i = 0; i < cq->queueSize; i++){
        popfront(cq);
    }
}

