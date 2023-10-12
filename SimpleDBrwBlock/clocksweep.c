// buffer replacement strategy
// clock sweep policy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "clocksweep.h"
#include "db.h"

Database *db_clock;
Conf *cf_clock;
UINT VictimThreshold = 3;

BufPool initBufferPool(BufPool pool, Database *db, Conf *cf)
{
	printf("initBufferPool()\n");
	db_clock = db;
	cf_clock = cf;
	pool = malloc(sizeof(struct bufPool));
	pool->nbufs = cf_clock->buf_slots;
	// printf("pool->nbufs:%d", pool->nbufs);
	pool->bufs = malloc(pool->nbufs * sizeof(struct buffer));

	for (int i = 0; i < pool->nbufs; i++){
		// printf("Buffer init pool func() ");
		// printf("page_size %d /", cf_clock->page_size);
	    pool->bufs[i].data = (int*)malloc(sizeof(int)*cf->page_size);
		// printf("$$$$$$$$$$$$$$ sizeof pool->bufs[].data %ld\n", sizeof(pool->bufs[i].data));
		//pool->bufs[i].data = "\0";
		for(int j = 0; j<cf->page_size;j++){
			pool->bufs[i].data[j] = 0;
		}
	}

	for(int i = 0; i < pool->nbufs;i++){
		pool->bufs[i].Pid_ = (uint64_t*)malloc(sizeof(uint64_t)*1);
		pool->bufs[i].Pid_[0] = 23;
	}	
	

	for(int i = 0; i < pool->nbufs;i++){
		pool->bufs[i].dirty = 0;
	}

	for(int i = 0; i < pool->nbufs; i++){
		pool->bufs[i].oid = 0;
	}

	for(int i = 0; i < pool->nbufs; i++){
		pool->bufs[i].usage_count = 0;
	}

	for(int i = 0; i < pool->nbufs; i++){
		pool->bufs[i].pin = 0;
	}

	return pool;
}

void releaseBufferPool(BufPool pool){

printf("releaseBufferPool()\n");
	for (int i = 0; i < pool->nbufs; i++){
	    free(pool->bufs[i].data);
		free(pool->bufs[i].Pid_);
	}
	free(pool->bufs);
	free(pool);
	
}

bool isBufferEmpty(BufPool pool, int slot)
{

	if (pool->bufs[slot].dirty == 0)
	{
		return true;
	}

	return false;
}

// cycle search
// when the pointer that point to each buffer reach the end of the bufferslots
// return the slot
// set it back to the begin
int cycleSearchPageInBuffer(BufPool pool, UINT64 pid, Table table_, FILE** fp)
{

printf("cycleSearchPageInBuffer()\n");
	for (int i = 0; i < pool->nbufs; i++)
	{
		if (findPageInBuffer(pool, table_.oid, pid, i))
			return i;
	}
	// if the page not exist in buffer
	//find if there is empty buffer before using clocksweep
	for(int i = 0; i < pool->nbufs; i++)
	{
		if (isBufferEmpty(pool, i)){
			insertPageInBuffer(pool, i, pid, table_ ,fp);
			return i;
		}
	}

	//if buffer pool full
	//find next victim
	int NVB = 0;
	while(!findNextVictim(pool, pid, NVB, table_, fp))
	{

		// if (findNextVictim(pool, pid, NVB, table_, fp))
		// 	return NVB;// return the index of buffer contains our required page
		NVB = (NVB + 1) % pool->nbufs;
	}
	return NVB;

}

// find page
// if page in buffer, set pin =1, and return position
bool findPageInBuffer(BufPool pool, UINT oid, UINT64 pid, int slot)
{
printf("findPageInBuffer()\n");
	// int i;//slot
	// for(i = 0; i < pool->nbufs;i++){
	if(pool->bufs[slot].oid == oid){

		if (pool->bufs[slot].pid == pid)
		{
			PinPage(pool,slot);
			pool->bufs[slot].pin = 1;
			if(pool->bufs[slot].usage_count<VictimThreshold)
				pool->bufs[slot].usage_count++;
			return true;
		}
		
	}
// }
	return false;
}

// when use that page
void PinPage(BufPool pool, int slot)
{
	// printf("PinPage() Function call");
	pool->bufs[slot].pin = 1;
}

void UnPinPage(BufPool pool, int slot)
{
	// printf("UnPinPage() Function call");
	pool->bufs[slot].pin = 0;
}

bool findNextVictim(BufPool pool, UINT64 pid, int NVB, Table table_new, FILE **fp_new)
{ 
	// printf("findNextVictim(): ");
	// printf("isBufferEmpty? |");
	if (isBufferEmpty(pool, NVB))
	{ // if buffer empty, directly insert page
		// printf("| BufferEmpty |");
		insertPageInBuffer(pool, NVB, pid, table_new, fp_new);
		PinPage(pool, NVB);
		return true;
	}
	// printf("| Buffer %d Not Empty |",NVB);
	// scan each buffer until we find the next victim, pin = 0 && usage_count = 0
	if (pool->bufs[NVB].pin == 1)
	{ 
		// printf("Skip pin = 1 buffer |");
		if(pool->bufs[NVB].usage_count>0)
			pool->bufs[NVB].usage_count--;
	}
	else if(pool->bufs[NVB].pin == 0)
	{
		if (pool->bufs[NVB].usage_count == 0)
		{
			// printf("Find out the usage&&pin = 0 |");
			replacePageInBuffer(pool, NVB, pool->bufs[NVB].pid, pid, table_new, fp_new);
			PinPage(pool, NVB);
			return true;
		}
		else{
			if(pool->bufs[NVB].usage_count>0)
				pool->bufs[NVB].usage_count--;

			//check again pin = 0 ? && usage = 0 ?
			// if(pool->bufs[NVB].usage_count == 0){
			// 	//printf("Find out the usage&&pin = 0 |");
			// 	replacePageInBuffer(pool, NVB, pool->bufs[NVB].pid, pid, table_new, fp_new);
			// 	PinPage(pool, NVB);
			// 	return true;
			// }
		}
	}

	return false;
}


Tuple* fetchTuplesFromPageInBuffer(BufPool pool,int slot, Conf* cf, Table table_){
	// printf("\nfetchTuplesFromPageInBuffer()\n");
	INT ntuples_per_page = 0;
	// calculate the num of tuples in a page to allocate the space for a buffer's tuple array
	ntuples_per_page = (cf_clock->page_size - sizeof(UINT64)) / sizeof(INT) / table_.nattrs;
	// printf("ntuple_per_page %d\n", ntuples_per_page);
	Tuple *t= (Tuple*) malloc(sizeof(INT*)*ntuples_per_page);


	for(int i = 0; i < ntuples_per_page; i++){
		//t = pool->bufs[slot].data[i];
		t[i] = (Tuple)malloc(table_.nattrs*sizeof(INT));
		// printf("sizeof t: %ld ", sizeof(t[i]));
	}


	INT attridx  = 0;
	for(int i = 0; i < ntuples_per_page*table_.nattrs; i++){
		if(attridx < table_.nattrs){
			t[i/table_.nattrs][attridx] = pool->bufs[slot].data[i+2]; 
			printf("t[%d][%d]:%d | ",i/table_.nattrs,attridx, t[i/table_.nattrs][attridx]);
			attridx++;
		}else{
			attridx = 0;
			t[i/table_.nattrs][attridx] = pool->bufs[slot].data[i+2];
			printf("t[%d][%d]:%d | ",i/table_.nattrs,attridx, t[i/table_.nattrs][attridx]);
			attridx++;
		}
	}

	return t;
}

// store tuples of a page in one buffer
void storePageTupleInBuffer(BufPool pool, int slot, Table table_, FILE **fp, UINT64 pid)
{
	printf("storePageTupleInBuffer()\n");
	
	
	UINT offset = pool->bufs[slot].pid*(cf_clock->page_size);

	// printf("^^^^^^^^^^^^^^^^^^^^offset%d\n", offset);
	
	//move file pointer to each page begin
	fseek(*fp, offset, SEEK_SET);
	//read page content into buffer
	fread(pool->bufs[slot].data, sizeof(INT),cf_clock->page_size/sizeof(INT),*fp);

	for (int i = 0; i < cf_clock->page_size/sizeof(INT); i++) {
    	printf("%d ", pool->bufs[slot].data[i]);
	}
	// we cannot simply use int to get our page id, it's uint64
	fseek(*fp, offset, SEEK_SET);
	fread(pool->bufs[slot].Pid_, sizeof(uint64_t), 1 ,*fp);
	printf("\nPAGE ID, %jd\n",pool->bufs[slot].Pid_[0]);
	printf("sizeof uint64_t, %ld\n",sizeof(uint64_t));

	log_read_page(pool->bufs[slot].Pid_[0]);
	//printf("buffer %d contain %s", slot ,pool->bufs[slot].data);

	
}

void removePageTupleFromBuffer(BufPool pool, int slot, UINT64 pid){
	log_release_page(pool->bufs[slot].Pid_[0]);
	printf("removePageTupleFromBuffer()\n");
	for(int i = 0; i < cf_clock->page_size/sizeof(INT); i++)
		pool->bufs[slot].data[i] = 0;
}

void replacePaceTupleInBuffer(BufPool pool, int slot, Table table_new, FILE **fp_new, UINT64 old_pid, UINT64 new_pid){
	printf("replacePaceTupleInBuffer()\n");
	removePageTupleFromBuffer(pool, slot, old_pid);
	
	storePageTupleInBuffer(pool, slot, table_new, fp_new, new_pid);
}

void insertPageInBuffer(BufPool pool, int slot, UINT64 pid, Table table_ ,FILE **fp_)
{
	printf("insertPageInBuffer()\n");
	//log_read_page(pid);
	pool->bufs[slot].oid = table_.oid;
	pool->bufs[slot].pid = pid;
	pool->bufs[slot].dirty = 1;
	pool->bufs[slot].usage_count = 1;
	PinPage(pool,slot);
	storePageTupleInBuffer(pool, slot, table_, fp_, pid);
	
}

void replacePageInBuffer(BufPool pool, int slot, UINT64 old_pid, UINT64 new_pid, Table table_new, FILE **fp_new)
{
	printf("replacePageInBuffer()\n");
	//log_release_page(old_pid);
	//log_read_page(new_pid);
	pool->bufs[slot].pid = new_pid;
	pool->bufs[slot].oid = table_new.oid;
	PinPage(pool,slot);
	pool->bufs[slot].usage_count = 1;
	replacePaceTupleInBuffer(pool, slot, table_new, fp_new, old_pid, new_pid);
	
}

void removePageInBuffer(BufPool pool, int slot, UINT64 pid)
{
	printf("removePageInBuffer()\n");
	//log_release_page(pid);
	pool->bufs[slot].pid = 0;
	pool->bufs[slot].dirty = 0;
	UnPinPage(pool,slot);
	pool->bufs[slot].usage_count = 0;
	removePageTupleFromBuffer(pool, slot, pid);
	
}

void removeAllPageInBuffer(BufPool pool)
{
		printf("removeAllPageInBuffer()\n");
	for (int i = 0; i < pool->nbufs; i++)
	{
		if (!isBufferEmpty(pool, i))
			removePageInBuffer(pool, i, pool->bufs[i].pid);
	}
}

void printThePinSlot(BufPool pool){
	printf("\ncall printThePinSlot\n");
	printf("---------------------------------------------------------------------------------------------------------------------------------\n");
	for(int i = 0; i < pool->nbufs; i++ ){

		printf("|");
		//if(pool->bufs[i].pin == 1){
			//printf("slot %d is pined,",i);
			printf("slot %d pin = %d,",i,pool->bufs[i].pin);
		// }else if(pool->bufs[i].pin == 0){
		// 	printf("slot %d pin = %d,",i);
		// }
		printf(" ,usageCount = %d", pool->bufs[i].usage_count);
	}
	printf("\n-------------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n\n");
}
