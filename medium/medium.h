#ifndef __MEDIUM_H__
#define __MEDIUM_H__

#include <stdarg.h>

#include "shared/shared.h"

typedef struct {
    unsigned long id;
    char* title;
    char* artist;
    char* borrower;
} tMedium;

tMedium* createMedium(const char* title, const char* artist, const char* borrower);
ERRSTATE deleteMedium(tMedium* target);
ERRSTATE deleteMediumDeleter(void* target);

char* mediumSerializer(void* val);
void* mediumDeserializer(char* val);

BOOL hasIdPredicate(void* vmedium, int argc, va_list argv);
BOOL hasTitlePredicate(void* vmedium, int argc, va_list argv);
BOOL hasArtistPredicate(void* vmedium, int argc, va_list argv);
BOOL hasBorrowerPredicate(void* vmedium, int argc, va_list argv);

#endif
