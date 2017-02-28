#ifndef __LIBFORMS_H__
#define __LIBFORMS_H__

#include "list/list.h"

// sortation modes
#define SORT_TITLE_ASC 1
#define SORT_TITLE_DESC 2
#define SORT_ARTIST_ASC 3
#define SORT_ARTIST_DESC 4
#define SORT_BORROWER_ASC 5
#define SORT_BORROWER_DESC 6

void insertRecord();
void searchRecords();
void deleteRecord(unsigned long id);
void sortRecords(long mode);

void runLibformsInterface(int argc, char* argv[], tList* data);

#endif
