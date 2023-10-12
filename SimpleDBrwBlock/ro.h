#ifndef RO_H
#define RO_H
#include <math.h>
#include "db.h"
#include "file.h"
#include "clocksweep.h"

void init();
void release();

// equality test for one attribute
// idx: index of the attribute for comparison, 0 <= idx < nattrs
// cond_val: the compared value
// table_name: table name
_Table* sel(const UINT idx, const INT cond_val, const char* table_name);

_Table* join(const UINT idx1, const char* table1_name, const UINT idx2, const char* table2_name);

_Table* NestedLoopJoin(_Table* result, INT *table_npages, FILE** fp_Table[], INT* ntuples_per_page,Table **tables, const UINT idx1,const UINT idx2, UINT *ntuples);
Tuple combineTwoTuples(int tupidx1, int tupidx2, Tuple* t1, Tuple* t2, UINT nattrs1,UINT nattrs2);

_Table* SortMergeJoin( INT *table_npages, FILE** fp_Table[], INT* ntuples_per_page,Table **tables, const UINT idx1,const UINT idx2, UINT *ntuples);
Tuple** loadPageToArr(Tuple *arr[], INT *table_npages, INT* ntuples_per_page, Table **tables, FILE** fp_Table[] );

_Table* Merge(Tuple *arr1, Tuple *arr2, Table **tables, const UINT idx1,const UINT idx2, UINT *ntuples);
void BubbleSort(Tuple *arr, Table *tables, const UINT idx);

#endif