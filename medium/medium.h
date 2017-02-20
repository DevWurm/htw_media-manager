#ifndef __MEDIUM_H__
#define __MEDIUM_H__

#include "shared/shared.h"

typedef struct {
    char* title;
    char* artist;
    char* borrower;
} tMedium;

tMedium* createMedium(const char* title, const char* artist, const char* borrower);
ERRSTATE deleteMedium(tMedium* target);
ERRSTATE deleteMediumDeleter(void* target);

char* mediumSerializer(void* val);
void* mediumDeserializer(char* val);

#endif
