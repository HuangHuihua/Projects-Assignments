#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ro.h"
#include "db.h"

Database* db_ro;
Conf* cf_ro;
Queue* cq;
BufPool pool;

//for free result table
_Table* SR;
_Table* JR;

//free sortMergeJoin
_Table* TTTT;

//for free local array in function
Tuple* ARRAY[2];


//free file pointer
FILE **FPPP;
FILE** fP_T[2];

int ntuple_global = 0;

void init(){

    // printf("init");
    // do some initialization here.

    // example to get the Conf pointer
    //Conf* cf_ro = get_conf();
    cf_ro = get_conf();

    // example to get the Database pointer
    //Database* db_ro = get_db();
    db_ro = get_db();
    
    // printf("init db %d ntavle\n", db_ro->ntables);

    //init the cycle queue that manage file pointer
    cq = initQueue(db_ro, cf_ro);

    pool = initBufferPool(pool, db_ro, cf_ro);
    
    printf("init() is invoked.\n");
}

void release(){

    releaseBufferPool(pool);

    releaseQueue(cq);

    //free(cf_ro);

    //free(db_ro);
    // optional
    // do some end tasks here.
    // free space to avoid memory leak

    //free fp
    for(int i = 0; i < cf_ro->file_limit; i++){
        
    }

    free(FPPP);

    // for(int i = 0; i < 2; i++){
    //     free(fP_T[i]);
    // }
    // free(fP_T);

    // free(TTTT);

    // if(SR != NULL)
    //     free(SR);
    // SR = NULL;

    // if(JR != NULL)
    //     free(JR);
    // JR = NULL;

    printf("release() is invoked.\n");
}

_Table* sel(const UINT idx, const INT cond_val, const char* table_name){
    
   printf("sel() is invoked.\n");


    UINT table_oid;
    _Table* result;
    INT ntuples_per_page = 0;

    // printf("ssle() table_name %s\n",table_name );

    //search the tables in database by index and find the table name = table_name, get table oid
    for(int i  = 0; i < db_ro->ntables; i++){
        // printf("lopp %d \n",i);
        // printf("table names %s\n",db_ro->tables[i].name);
        
        if(strcmp(db_ro->tables[i].name,table_name)==0){
            // printf("table names %s\n",db_ro->tables[i].name);
            result = malloc(sizeof(_Table)+db_ro->tables[i].ntuples*sizeof(Tuple));
            table_oid = db_ro->tables[i].oid;
            // printf("sel() in init, oid %d.\n", table_oid);
            result->nattrs = db_ro->tables[i].nattrs;
            //result->ntuples = db_ro->tables[i].ntuples;
            result->ntuples = 0;
            //tuples in a page is equal to page size minus the size of page id accounted, then divide into Integer size, then t.nattrs is a tuple 
            ntuples_per_page = (cf_ro->page_size-sizeof(UINT64))/sizeof(INT)/db_ro->tables[i].nattrs;
            break;
        }
    }

    // for(int tuplesize = 0; tuplesize< result->ntuples; tuplesize++){
    //     result->tuples[tuplesize] = malloc(sizeof(INT)*result->nattrs);
    // }

    // printf("sel() after init result table.\n");

    FILE** fp = (FILE **)malloc(1 * sizeof(FILE *));
    //FILE* fp[0];
    *fp = NULL;

    FPPP = fp;
    //whether the table file is in pool
    //if not then pop file from queue and push the expected file in queue
    // printf("searchFileInpool() CALL | ");
    if(searchFileInPool(cq, table_oid) != -1){
        // printf("sel() search file pool.");
        int index_2 = searchFileInPool(cq, table_oid);
        // printf(" find File/table oid: %d and index: %d", table_oid, index_2);
        //get file pointer from file pool (the queue)
        fp = cq->filedata[index_2].fp;
        // printf("file pointer %d \n", *fp);
    }else{
        //
        if(IsQueueFull(cq)){
            // printf("Queue full and replace ");
            replaceFileInQueue(cq,table_oid,fp);

            // printf("file pointer %s \n", *fp);
        }else{
            // printf("pushEnd() CALL\n");
            pushEnd(cq, table_oid, fp);//already change in function
            // printf("file pointer %s \n", *fp);
        }
 
    }
    // printf("sel() after search file.\n");
    
    Tuple *t;
    INT table_npages;
    int count = 0;
    for(UINT i = 0; i < db_ro->ntables; i++){
    // printf("table names %s\n",db_ro->tables[i].name);

        if(strcmp(db_ro->tables[i].name,table_name)==0){
            //double double_ = (double)(db_ro->tables[i].ntuples)/(double)(ntuples_per_page);
            //ceil(double_);
            if(db_ro->tables[i].ntuples % ntuples_per_page != 0){
                table_npages = (db_ro->tables[i].ntuples)/ntuples_per_page +1;
                // printf("table_npages: %d ",table_npages);
            }
            else{
                table_npages = db_ro->tables[i].ntuples/ntuples_per_page;
                // printf("table_npages: %d ",table_npages);
            }   
            // printf("npages: %d\n",table_npages );

            for(int j = 0; j < table_npages;j++){//we search each page of the file (from buffer) to find goal tuple
                // printf("j %d ", j);
                // printf("\n");
                //read the pid from the beginning of the page
                
                int slot = cycleSearchPageInBuffer(pool,j,db_ro->tables[i],fp);
                    // printf("slot %d", slot);
                     
                t = fetchTuplesFromPageInBuffer(pool, slot, cf_ro, db_ro->tables[i]);

                for(int tupleidx = 0; tupleidx < ntuples_per_page; tupleidx++){
                    // printf("tuple and attr in sel() %d ", t[tupleidx][idx]);printf("\n");
                        if(t[tupleidx][idx] == cond_val){
                            result->ntuples++;
                            result->tuples[count] = malloc(sizeof(INT)*result->nattrs);
                            // printf("@@@@@@@@@@@@@@ tuple %d in result table: ",count);
                            for(int attr = 0; attr < db_ro->tables[i].nattrs; attr++){
                                result->tuples[count][attr] = t[tupleidx][attr];
                                // printf("%d\' ", result->tuples[count][attr]);
                            }
                            printf("\n");
                            count++;
                        }

                }
                //removePageInBuffer(pool,slot,j);
                // printf("Unpin Page");
                UnPinPage(pool, slot);
                // printf("pool->bufs[slot].pin = %d ", pool->bufs[slot].pin);
                // printf("slot %d ", slot);
                }
            }
            // printf("END of each loop ");
    }

    SR = result;
    // for(int i = 0; i < cf_ro->buf_slots; i++){
    //     UnPinPage(pool, i);
    // }
    // if(*fp != NULL) { 
    // if(fclose(*fp) == EOF) {
    //     perror("Error closing file");
    //     exit(EXIT_FAILURE);
    // }
    // free(fp);
    // fp = NULL;
    // }
    // fclose(*fp);
    // free(fp);
    // fp = NULL;
    // printf("before return result~~~~~~~~~~~~~~~~~~~~~~~~");printf("\n\n\n");
    return result;


}

_Table* join(const UINT idx1, const char* table1_name, const UINT idx2, const char* table2_name){

    printf("\n\njoin() is invoked.\n");

    //use an array to store the two tables to join 
    Table **tables = malloc(sizeof(Table *)*2);//0 is table1, 1 is table2
    for(int i = 0; i < 2 ;i++){
        tables[i] = malloc(sizeof(Table));
    }

        //loop search for the table in db_ro
        for(int j = 0 ;j < db_ro->ntables; j++){
            //grab two table oid in db_ro
            if(strcmp(db_ro->tables[j].name, table1_name) == 0){
                // printf("\n db_ro->tables[j].name %s \n", db_ro->tables[j].name);
                tables[0]->oid = db_ro->tables[j].oid;
                tables[0]->nattrs = db_ro->tables[j].nattrs;
                tables[0]->ntuples = db_ro->tables[j].ntuples;
                
            }

            if(strcmp(db_ro->tables[j].name,table2_name)==0){
                tables[1]->oid = db_ro->tables[j].oid;
                tables[1]->nattrs = db_ro->tables[j].nattrs;
                tables[1]->ntuples = db_ro->tables[j].ntuples;
                
            }
        }

    INT table_npages[2];
    INT ntuples_per_page[2];

    ntuples_per_page[0] = (cf_ro->page_size-sizeof(UINT64))/sizeof(INT)/tables[0]->nattrs;
    ntuples_per_page[1] = (cf_ro->page_size-sizeof(UINT64))/sizeof(INT)/tables[1]->nattrs;

    // printf("ntuples_per_page[%d]: %d ,", 0, ntuples_per_page[0]);
    // printf("ntuples_per_page[%d]: %d ,", 0, ntuples_per_page[1]);

    //calculate page num
    if(tables[0]->ntuples%ntuples_per_page[0] == 0){
    table_npages[0] = tables[0]->ntuples/ntuples_per_page[0];
    
    }else {
    table_npages[0] = tables[0]->ntuples/ntuples_per_page[0] + 1;
    }

    if(tables[1]->ntuples%ntuples_per_page[1] == 0){
    table_npages[1] = tables[1]->ntuples/ntuples_per_page[1];
    
    }else {
    table_npages[1] = tables[1]->ntuples/ntuples_per_page[1] + 1;

    }
    //table_npages[1] = tables[1]->ntuples/ntuples_per_page[1];

    // printf("table_npages[%d]: %d ,", 0, table_npages[0]);
    // printf("table_npages[%d]: %d \n", 1, table_npages[1]);

    int choice = cf_ro->buf_slots < table_npages[0] + table_npages[1] ? 1 : 2;

    //a table a file
    //check if table file already in 
    FILE*** fp_Table = malloc(2*sizeof(FILE**));
    for(int i = 0; i < 2; i++){
        fp_Table[i] = malloc(sizeof(FILE*));
        *(fp_Table[i]) = NULL; 
    }

    fP_T[0] = fp_Table[0];
    fP_T[1] = fp_Table[1];

    if(searchFileInPool(cq, tables[0]->oid) != -1){
        int index_1 = searchFileInPool(cq, tables[0]->oid);
        fp_Table[0] = cq->filedata[index_1].fp;
    }else{
        //
        if(IsQueueFull(cq)){
            replaceFileInQueue(cq,tables[0]->oid,fp_Table[0]);
        }else{
            pushEnd(cq, tables[0]->oid, fp_Table[0]);
        }
 
    }

    if(searchFileInPool(cq, tables[1]->oid) != -1){
        int index_2 = searchFileInPool(cq, tables[1]->oid);
        fp_Table[1] = cq->filedata[index_2].fp;
    }else{
        //
        if(IsQueueFull(cq)){
            replaceFileInQueue(cq,tables[1]->oid,fp_Table[1]);
        }else{
            pushEnd(cq, tables[1]->oid, fp_Table[1]);
        }
 
    }


    //result table num of attributes in a tuple is equal to t1.nattrs + t2.nattrs
    _Table* result = malloc(sizeof(_Table)+(tables[0]->ntuples+tables[1]->ntuples)*sizeof(Tuple));
    //_Table* result;
    //result = (_Table*)malloc(sizeof(_Table)+(tables[0]->nattrs+tables[1]->nattrs)*sizeof(Tuple));
    //result->nattrs = tables[0]->nattrs+tables[1]->nattrs;
    //result->ntuples = tables[0]->ntuples+tables[1]->ntuples;
            for(int i = 0; i < tables[0]->ntuples+tables[1]->ntuples; i++){
                result->tuples[i] = malloc(sizeof(INT)*(tables[1]->nattrs+tables[0]->nattrs));
            }

    result->nattrs = tables[1]->nattrs+tables[0]->nattrs;
    result->ntuples = 0;

    switch(choice)
    {
        case 1:
        //int index_res = 0;
            result = NestedLoopJoin(result, table_npages, fp_Table, ntuples_per_page, tables, idx1, idx2, &(result->ntuples));
            break;
        case 2:
            //result->ntuples = 0;
            ntuple_global=0;
            result= SortMergeJoin(table_npages, fp_Table, ntuples_per_page, tables, idx1, idx2, &(result->ntuples));
            result->nattrs = tables[1]->nattrs+tables[0]->nattrs;
            result->ntuples = ntuple_global;
            break;
        default:
            break;
    }


    // printf("\n\nbefore return 1111111111111111111111111111\n");
    // printf("result->ntupless %d\n",result->ntuples);

    for(int i = 0;i < result->ntuples ;i++){
        for(int j = 0; j < (tables[0]->nattrs+tables[1]->nattrs);j++){
            printf("\nresult[%d][%d] = %d, ",i,j, result->tuples[i][j]);
        }
    }
    ntuple_global=0;
    JR = result;
    return result;
}

//
_Table* NestedLoopJoin(_Table* result, INT *table_npages, FILE** fp_Table[], INT* ntuples_per_page,Table **tables, const UINT idx1,const UINT idx2, UINT *ntuples){

    //result = malloc(sizeof(_Table)+(tables[0]->ntuples+tables[1]->ntuples)*sizeof(Tuple));
    printf("nested function");
    Table *outer = table_npages[0]<table_npages[1] ? tables[0]:tables[1] ;
    Table *inner = table_npages[0]>table_npages[1] ? tables[0]:tables[1] ;

    FILE** fp_outer = table_npages[0]<table_npages[1] ? fp_Table[0]:fp_Table[1];
    FILE** fp_inner = table_npages[0]>table_npages[1] ? fp_Table[0]:fp_Table[1];

    INT outer_npages = table_npages[0]<table_npages[1] ? table_npages[0]:table_npages[1];
    INT inner_npages = table_npages[0]>table_npages[1] ? table_npages[0]:table_npages[1];

    INT outer_ntuples_per_p = table_npages[0]<table_npages[1] ? ntuples_per_page[0]:ntuples_per_page[1];
    INT inner_ntuples_per_p = table_npages[0]>table_npages[1] ? ntuples_per_page[0]:ntuples_per_page[1];

    //read pages from outer table file into N-1 buffer 
    // removeAllPageInBuffer(pool);
    // for(int i = 0; i < cf_ro->buf_slots-1; i++){
    //     insertPageInBuffer(pool,i,i, *outer,fp_outer);
    //     if(i == outer_npages) break;
    // }
    (*ntuples)=0;
    //int index_ = 0;
    int slot_outer = 0;
    int slot_inner = 0;
    Tuple* t1 = (Tuple*) malloc(sizeof(INT*)*outer_ntuples_per_p);
    for(int i = 0; i < outer_ntuples_per_p ;i++){
        t1[i] = malloc(sizeof(INT)*(outer->nattrs));
    }
    Tuple* t2 = (Tuple*) malloc(sizeof(INT*)*inner_ntuples_per_p);
    for(int i = 0; i < inner_ntuples_per_p ;i++){
        t2[i] = malloc(sizeof(INT)*(inner->nattrs));
    }    


    //read the N-2 page of outer table into buffer first
    for(int i = 0; i < (cf_ro->buf_slots - 2) ;i++){
        slot_outer = cycleSearchPageInBuffer(pool, i, *outer, fp_outer);
        fetchTuplesFromPageInBuffer(pool,slot_outer, cf_ro, *outer);
        UnPinPage(pool,slot_outer);
    }


// printThePinSlot(pool);
    for(int i = 0; i < outer_npages; i++){
        // printf("\n###########################sizeof outer pages : %d ", outer_npages);
        //we pass pid and get slot 
        slot_outer = cycleSearchPageInBuffer(pool, i, *outer, fp_outer); //get each tuple in page
        // printThePinSlot(pool);
        // printf("\nslot_outer %d\n",slot_outer);
        t1 = fetchTuplesFromPageInBuffer(pool,slot_outer, cf_ro, *outer);
        // printf("\n\n\nSTART INNER LOOOOOOOOOOOOOOOOOOP");
        for(int j = 0; j < inner_npages; j++){
            // printf("\n###########################sizeof inner pages : %d ", inner_npages);
            slot_inner = cycleSearchPageInBuffer(pool,j,*inner,fp_inner);
            // printThePinSlot(pool);
            // printf("\nslot_inner %d\n",slot_inner);
            t2 = fetchTuplesFromPageInBuffer(pool, slot_inner, cf_ro, *inner);
            //for each tuple in each page
            for(int tupidx1 = 0; tupidx1 < outer_ntuples_per_p; tupidx1++ ){
                // printf("\nouter_ntuples_per_p,%d ", outer_ntuples_per_p);
                for(int tupidx2 = 0; tupidx2 < inner_ntuples_per_p; tupidx2++ ){
                    // printf("inner_ntuples_per_p,%d\n", inner_ntuples_per_p);
                    // printf("t1[tupidx1, %d][idx1,%d] = %d,",tupidx1,idx1,t1[tupidx1][idx1]);
                    // printf("t2[tupidx2, %d][idx2,%d] = %d \n",tupidx2,idx2,t2[tupidx2][idx2]);
                    if(t1[tupidx1][idx1] == t2[tupidx2][idx2]){
                        result->tuples[*ntuples] = malloc(sizeof(INT)*(outer->nattrs+inner->nattrs));
                            result->tuples[*ntuples] = combineTwoTuples(tupidx1,tupidx2,t1,t2,outer->nattrs,inner->nattrs);
                        for(int d = 0; d < outer->nattrs+inner->nattrs; d++ ){
                            // printf("result->tuples[][] = %d  ", result->tuples[*ntuples][d]);
                           // result->tuples[index_][i]
                        }
                        (*ntuples)++;
                    }
                }

            // UnPinPage(pool,slot_inner);
            }
            // printf("before unpin ");
            // printThePinSlot(pool);
            // printf(" | ");
            UnPinPage(pool,slot_inner);
            // printf(" inner Unpin | ");
            // printThePinSlot(pool);
        }
        // printf("before unpin ");
        // printThePinSlot(pool);
        // printf(" | ");
        UnPinPage(pool,slot_outer);
        // printf(" Outer Unpin |");
        // printThePinSlot(pool);
    }

    for(int i = 0; i <cf_ro->buf_slots;i++){
        UnPinPage(pool,i);
    }

    // printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!result->ntuples: %d", *ntuples);

    // printf("\nbefore return result 22222222222222222222222222");
    // for(int i = 0;i < *ntuples ;i++){
    //     for(int j = 0; j < (tables[0]->nattrs+tables[1]->nattrs);j++){
    //         printf("result[%d][%d] = %d, ",0,j, result->tuples[i][j]);
    //     }
    // }

    return result;
}

Tuple combineTwoTuples(int tupidx1, int tupidx2, Tuple* t1, Tuple* t2, UINT nattrs1,UINT nattrs2){

    Tuple *tmpresult = (Tuple*)malloc(sizeof(INT*));

    tmpresult[0] = (Tuple)malloc((nattrs1+nattrs2)*sizeof(INT));
    
    //Tuple *result->tuples[tupleidx] = malloc(sizeof(INT)*(nattrs1+nattrs2));

    for(int i = 0; i< nattrs1; i++){
        tmpresult[0][i] = t1[tupidx1][i];
        // printf("&&&&&&&&&&&&&&&&&&%d\n", tmpresult[0][i]);
    }
    for(int j = 0; j < nattrs2; j++){
        tmpresult[0][j+nattrs1] = t2[tupidx2][j];
        // printf("&&&&&&&&&&&&&&&&&&%d\n", tmpresult[0][j+nattrs1]);

    }

   return tmpresult[0];
    free(tmpresult);
}

_Table* SortMergeJoin(INT *table_npages, FILE** fp_Table[], INT* ntuples_per_page,Table **tables, const UINT idx1,const UINT idx2, UINT *ntuples){
printf("\nSortMergeJoin()\n");
    Tuple* arr[2];

    // printf("============================================table_npages %d", table_npages[0]);

    // arr[0] = (Tuple*) malloc(sizeof(INT*)*(tables[0]->ntuples));
    // for(int i = 0; i < tables[0]->ntuples ;i++){
    //     arr[0][i] = malloc(sizeof(INT)*(tables[0]->nattrs));
        
    // }
    // arr[1] = (Tuple*) malloc(sizeof(INT*)*(tables[1]->ntuples));
    // for(int i = 0; i < tables[1]->ntuples ;i++){
    //     arr[1][i] = malloc(sizeof(INT)*(tables[1]->nattrs));
    // }   

    _Table* ttt = malloc(sizeof(_Table)+(tables[0]->ntuples+tables[1]->ntuples)*sizeof(Tuple));
    //_Table* ttt;
    //ttt = (_Table*)malloc(sizeof(_Table)+(tables[0]->nattrs+tables[1]->nattrs)*sizeof(Tuple));
    //ttt->nattrs = tables[0]->nattrs+tables[1]->nattrs;
    //ttt->ntuples = tables[0]->ntuples+tables[1]->ntuples;
    for(int i = 0; i < tables[0]->ntuples+tables[1]->ntuples; i++){
        ttt->tuples[i] = malloc(sizeof(INT)*(tables[1]->nattrs+tables[0]->nattrs));
    }

    ttt->nattrs = tables[1]->nattrs+tables[0]->nattrs;
    ttt->ntuples = 0;

    (*ntuples) = 0;
    //initialize arr
    // for(int i  = 0; i < (tables[0]->ntuples); i++){
    //     for(int j  = 0; j < (tables[0]->nattrs); j++){
    //         arr[0][i][j] = 0;
    //     }

    // }

    // for(int i  = 0; i < (tables[1]->ntuples); i++){
    //     for(int j  = 0; j < (tables[1]->nattrs); j++){
    //         arr[1][i][j] = 0;
    //     }

    // }


    //load both table 1 2 pages into buffer pool
    loadPageToArr(arr, table_npages, ntuples_per_page, tables, fp_Table);

    //sort table 1 and sort table 2
    BubbleSort(arr[0], tables[0], idx1);

    BubbleSort(arr[1], tables[1], idx2);

    // printf("End of loadPageToArr(): ");
    for(int i = 0; i < tables[0]->ntuples ;i++){
        // printf("\narr[0][%d] = -----",i);
        for(int j = 0; j < tables[0]->nattrs ;j++){
            //arr[0][i] = malloc(sizeof(INT)*(tables[0]->nattrs));
            // printf("%d ",arr[0][i][j]);
        }   
        // printf("\n");
    }
    // printf("\n========================================================================\n");

    for(int i = 0; i < tables[1]->ntuples ;i++){
        // printf("\narr[1][%d] = -------", i);
        for(int j = 0; j < tables[1]->nattrs ;j++){
            //arr[1][i] = malloc(sizeof(INT)*(tables[1]->nattrs));
            // printf("%d ", arr[1][i][j]);
        }
        // printf("\n");   
    }

    //after sort, merge 
    ttt = Merge( arr[0],arr[1], tables, idx1, idx2, ntuples);
    // printf("\nntuples{***************************%d ", *ntuples);
    //printf("ttt[0][0] = %d", ttt->tuples[0][0]);

    for(int i = 0;i < (*ntuples) ;i++){
        for(int j = 0; j < (tables[0]->nattrs+tables[1]->nattrs);j++){
            // printf("ttt[%d][%d] = %d,\n ",i,j, ttt->tuples[i][j]);
        }
    }
    

    for(int i = 0; i < tables[0]->ntuples ;i++){
        free(arr[0][i]);
        
    }
    free(arr[0]);

    // for(int i = 0; i < tables[0]->ntuples ;i++){
    //     free(arr[1][i]);
        
    // }
    // free(arr[1]);

    TTTT = ttt;

    return ttt;

}

//merge two array/table
_Table* Merge(Tuple *arr1,Tuple *arr2, Table **tables, const UINT idx1,const UINT idx2, UINT *ntuples){
    
    _Table* t = malloc(sizeof(_Table)+(tables[0]->ntuples+tables[1]->ntuples)*sizeof(Tuple));

    //a cursor point to the start of 2nd table =idx2 buffer
    //for each arr[0][][idx], we megre every arr[1][][idx]

    (*ntuples) = 0;
    int ss = 0;

    int r = 0;
    int s = 0;
    while(r != (tables[0]->ntuples) && s != (tables[1]->ntuples)){
        while(r != (tables[0]->ntuples) && arr1[r][idx1] < arr2[s][idx2]){
            // printf("++++++++++++++++++++++++++++++++arr1[%d][%d] = %d",r, idx1,arr1[r][idx1]);
            r++;
        }
        if(r == (tables[0]->ntuples)) break;

        while(s != (tables[1]->ntuples) && arr1[r][idx1] > arr2[s][idx2]){
            s++;
        }
        if(s == (tables[1]->ntuples)) break;

        
        ss = s;
        while(r != (tables[0]->ntuples) && arr1[r][idx1] == arr2[s][idx2]){
            while(s != (tables[1]->ntuples) && arr2[s][idx2] == arr1[r][idx1]){
                t->tuples[(*ntuples)] = malloc(sizeof(INT)*((tables[0]->nattrs)+(tables[1]->nattrs)));
                t->tuples[(*ntuples)] = combineTwoTuples(r, s, arr1, arr2, (tables[0]->nattrs), (tables[1]->nattrs));

                s++;
                (*ntuples)++;
                ntuple_global++;
            }
            r++;
            s = ss;
        }

    }



    // (*ntuples )= 0;
    // int ss = 0;
    // for(int index1 = 0; index1 < (tables[0]->ntuples);){
    //     for(int index2 = 0; index2 <(tables[1]->ntuples);){
    //         if((arr1[index1][idx1]) == (arr2[index2][idx2])){
    //             ss = index2;
    //             while((index1 < (tables[0]->ntuples))&&(index2 <(tables[1]->ntuples))&&(arr1[index1][idx1] == arr2[ss][idx2])){
    //                 printf("\nOutsize index2 %d ", index2);
    //                 while((index1 < (tables[0]->ntuples))&&(index2 <(tables[1]->ntuples))&&(arr1[index1][idx1] == arr2[index2][idx2])){
    //                     printf("Inside index2 %d |", index2);
    //                     //result->tuples[(*ntuples)] = malloc(sizeof(INT)*((tables[0]->nattrs)+(tables[1]->nattrs)));
    //                     t->tuples[(*ntuples)] = malloc(sizeof(INT)*((tables[0]->nattrs)+(tables[1]->nattrs)));
    //                     t->tuples[(*ntuples)] = combineTwoTuples(index1, index2, arr1, arr2, tables[0]->nattrs, tables[1]->nattrs);
    //                     printf("Inside ntuples %d |", (*ntuples));
    //                     if(index2 <(tables[1]->ntuples))
    //                         index2++;
    //                     else break;
    //                     (*ntuples)++;
    //                     ntuple_global++;
    //                 }
    //                 printf("\nOutsize index2 %d ", index2);
    //                 index2 = ss;
    //                 printf("\nOutsize index2 %d ", index2);
    //                 if(index1 <(tables[0]->ntuples))
    //                     index1++;
    //                 else break;
    //             }
    //         }
    //         if((index2 <(tables[1]->ntuples))&&( index1 < (tables[0]->ntuples))&&(arr1[index1][idx1] > arr2[index2][idx2])){
    //             printf("arr1[index1][idx1]%d > arr2[index2][idx2]%d ",arr1[index1][idx1],arr2[index2][idx2]);
    //             printf("\nif but index2 ++ ");
    //             index2++;
    //             printf(" index2 = %d,tables[1]->ntuples = %d\n",index2,tables[1]->ntuples);
    //         }
    //             printf("\nif gap");

    //         if(( index1 < (tables[0]->ntuples))&&(index2 <(tables[1]->ntuples))&&(arr1[index1][idx1] < arr2[index2][idx2] )){
    //             printf("if but index1 ++\n");
    //             index1++;
    //         }
    //     }

    //     //index1++;
    // }

    return t;
    free(t);
}

//recurrsion
//sort according to the condition index
//sort separately in a manual allocated memory
void BubbleSort(Tuple *arr, Table *tables, const UINT idx){
    
    Tuple* tmp = malloc(sizeof(INT)*(tables->nattrs));

    for(int i = 0; i < (tables->ntuples); i++){
        for(int j = 0; j < (tables->ntuples)-i-1; j++){
            if(arr[j][idx]>arr[j+1][idx]){
                tmp[0] = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = tmp[0];
            }
        }
    }
    free(tmp);

}


//put each tables tuples in two array
Tuple** loadPageToArr(Tuple *arr[], INT *table_npages, INT* ntuples_per_page, Table **tables, FILE** fp_Table[] ){

printf("\nloadPageToArr() \n");
    int slot_0 = 0;
    int slot_1 = 0;
    
    arr[0] = (Tuple*) malloc(sizeof(INT*)*(tables[0]->ntuples));
    for(int i = 0; i < tables[0]->ntuples ;i++){
        arr[0][i] = malloc(sizeof(INT)*(tables[0]->nattrs));
        
    }
    
    ARRAY[0]=arr[0];
    
    arr[1] = (Tuple*) malloc(sizeof(INT*)*(tables[1]->ntuples));
    for(int i = 0; i < tables[1]->ntuples ;i++){
        arr[1][i] = malloc(sizeof(INT)*(tables[1]->nattrs));
    }   

    ARRAY[1]=arr[1];

    // printf("?????????????????????????????????table_npages[0] %d \n",table_npages[0]);
    // printf("?????????????????????????????????table_npages[1] %d \n",table_npages[1]);

    // printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>tables[0]->ntuples %d \n",tables[0]->ntuples);
    // printf("::::::::::::::::::::::::::::::::::tables[1]->ntuples %d \n",tables[1]->ntuples);

    // printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>ntuples_per_page[0] %d \n",ntuples_per_page[0]);
    // printf("::::::::::::::::::::::::::::::::::ntuples_per_page[1] %d \n",ntuples_per_page[1]);
    
    // printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>tables[0]->nattrs %d \n",tables[0]->nattrs);
    // printf("::::::::::::::::::::::::::::::::::tables[1]->nattrs %d \n",tables[1]->nattrs);
    
    
    Tuple* t1 = (Tuple*) malloc(sizeof(INT*)*(ntuples_per_page[0]));
    for(int i = 0; i < ntuples_per_page[0] ;i++){
        t1[i] = malloc(sizeof(INT)*(tables[0]->nattrs));
    }

    Tuple* t2 = (Tuple*) malloc(sizeof(INT*)*(ntuples_per_page[1]));
    for(int i = 0; i < ntuples_per_page[1] ;i++){
        t2[i] = malloc(sizeof(INT)*(tables[1]->nattrs));
    }  


    for(int i  = 0; i < ntuples_per_page[0]; i++){
        for(int j  = 0; j < tables[0]->nattrs; j++){
            t1[i][j] = 0;
        } 
    }

    for(int i  = 0; i < ntuples_per_page[1]; i++){
        for(int j  = 0; j < tables[1]->nattrs; j++){
            t2[i][j] = 0;
        }
    }

    int index_1 = 0;
    int index_2 = 0;
    //array 1
    for(int pid_0 = 0; pid_0 < table_npages[0]; pid_0++){
        slot_0 = cycleSearchPageInBuffer(pool, pid_0, *(tables[0]), fp_Table[0]);
        t1 = fetchTuplesFromPageInBuffer(pool, slot_0, cf_ro, *(tables[0]));
        for(int i = 0; i < (ntuples_per_page[0]); i++){
            for(int j = 0; j < (tables[0]->nattrs); j++){
                if(index_1 < (tables[0]->ntuples)){
                    arr[0][index_1][j] = t1[i][j];
                    // printf("arr1[][][] = %d ,",arr[0][index_1][j]);
                }
            }
            if(index_1 < (tables[0]->ntuples))
                index_1++;
        }
        // printf("before unpin ");
        // printThePinSlot(pool);
        // printf(" | ");
        UnPinPage(pool,slot_0);
        // printf(" table1 Unpin |");
        // printThePinSlot(pool);

    }

    for(int pid_1 = 0; pid_1 < table_npages[1]; pid_1++){
        slot_1 = cycleSearchPageInBuffer(pool, pid_1, *(tables[1]), fp_Table[1]);
        t2 = fetchTuplesFromPageInBuffer(pool, slot_1, cf_ro, *(tables[1]));
        for(int i = 0; i < (ntuples_per_page[1]); i++){
            for(int j = 0; j < (tables[1]->nattrs); j++){
                if(index_2<(tables[1]->ntuples)){
                    arr[1][index_2][j]= t2[i][j];
                    // printf("arr2[][][] = %d ,", arr[1][index_2][j]);
                }
            }
            if(index_2<(tables[1]->ntuples))
                index_2++;
        }
        // printf("before unpin ");
        // printThePinSlot(pool);
        // printf(" | ");
        UnPinPage(pool,slot_1);
        // printf(" table2 Unpin |");
        // printThePinSlot(pool);
    }

    // printf("End of loadPageToArr(): ");
    for(int i = 0; i < tables[0]->ntuples ;i++){
        for(int j = 0; j < tables[0]->nattrs ;j++){
            //arr[0][i] = malloc(sizeof(INT)*(tables[0]->nattrs));
            // printf("arr[0][%d][%d] = %d-----",i, j, arr[0][i][j]);
        }   
    }
    // printf("\n========================================================================\n");

    for(int i = 0; i < tables[1]->ntuples ;i++){
        for(int j = 0; j < tables[1]->nattrs ;j++){
            //arr[1][i] = malloc(sizeof(INT)*(tables[1]->nattrs));
            // printf("arr[1][%d][%d] = %d-----",i, j, arr[1][i][j]);
        }   
    }

    free(t1);
    free(t2);
        
    // printf("\n");
    return arr;
    
}