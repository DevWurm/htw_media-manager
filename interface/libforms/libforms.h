#ifndef __LIBFORMS_H__
#define __LIBFORMS_H__

#include "list/list.h"

void insertRecord();
void searchRecords();
void deleteRecord(unsigned long id);

void runLibformsInterface(int argc, char* argv[], tList* data);

#endif
