#ifndef CLOCKSWEEP_H
#define CLOCKSWEEP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
// #include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include "db.h"
#include "ro.h"

struct bufPool
{
	int nbufs; 
	struct buffer *bufs;
};

typedef struct bufPool *BufPool;

struct buffer
{
	int oid; // to distinguish different tables' pages
	int pid;
	int pin; // 1 or 0 , indicate the buffer is using
	int usage_count;
	int dirty; // 0 is clear
	//char  *data;//the page itself, store page content in buffer
	uint64_t *Pid_;
	INT *data;
	//need freelist?
	//int freelist
};


BufPool initBufferPool(BufPool pool, Database *db, Conf *cf);

void releaseBufferPool(BufPool pool);

bool isBufferEmpty(BufPool pool, int slot);

int cycleSearchPageInBuffer(BufPool pool, UINT64 pid, Table table_, FILE** fp);


bool findPageInBuffer(BufPool pool, UINT oid, UINT64 pid, int slot);


void PinPage(BufPool pool, int slot);
void UnPinPage(BufPool pool, int slot);

bool findNextVictim(BufPool pool, UINT64 pid, int NVB, Table table_new, FILE **fp_new);

Tuple* fetchTuplesFromPageInBuffer(BufPool pool,int slot, Conf* cf, Table table_);

void storePageTupleInBuffer(BufPool pool, int slot, Table table_, FILE **fp, UINT64 pid);
void removePageTupleFromBuffer(BufPool pool, int slot, UINT64 pid);
void replacePaceTupleInBuffer(BufPool pool, int slot, Table table_new, FILE **fp_new, UINT64 old_pid, UINT64 new_pid);

void insertPageInBuffer(BufPool pool, int slot, UINT64 pid, Table table_ ,FILE **fp_);
void replacePageInBuffer(BufPool pool, int slot, UINT64 old_pid, UINT64 new_pid, Table table_new, FILE **fp_new);
void removePageInBuffer(BufPool pool, int slot, UINT64 pid);

void removeAllPageInBuffer(BufPool pool);

void printThePinSlot(BufPool pool);

#endif