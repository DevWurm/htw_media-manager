#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "shared/shared.h"
#include "medium.h"

tMedium* createMedium(const char* title, const char* artist, const char* borrower) {
    if (title == NULL || artist == NULL || borrower == NULL) return NULL;

    tMedium* medium = malloc(sizeof(tMedium));
    if (medium == NULL) return NULL;

    medium->title = malloc((strlen(title) + 1) * sizeof(char));
    medium->artist = malloc((strlen(artist) + 1) * sizeof(char));
    medium->borrower = malloc((strlen(borrower) + 1) * sizeof(char));

    if (medium->title == NULL || medium->artist == NULL || medium->borrower == NULL) {
        free(medium->title); free(medium->artist); free(medium->borrower); free(medium);
        return NULL;
    } 
    
    strcpy(medium->title, title);
    strcpy(medium->artist, artist);
    strcpy(medium->borrower, borrower);

    return medium;
}

ERRSTATE deleteMedium(tMedium* target) {
    if (target == NULL) return ERR;

    free(target->title);
    free(target->artist);
    free(target->borrower);

    free(target);

    return OK;
}

ERRSTATE deleteMediumDeleter(void* target) {
    return deleteMedium((tMedium*) target);
}

char* mediumSerializer(void* vval) {
    if (vval == NULL) return NULL;

    tMedium* val = vval;

    char* res = malloc(((strlen(val->title) + 1) + (strlen(val->artist) + 1) + (strlen(val->borrower) + 1)) * sizeof(char));
    if (res == NULL) return NULL;

    sprintf(res, "%s;%s;%s", val->title, val->artist, val->borrower);

    return res;
}

void* mediumDeserializer(char* val) {
    char* title = strtok(val, ";");
    if (title == NULL) return NULL;

    char* artist = strtok(NULL, ";");
    if (artist == NULL) return NULL;

    char* borrower = strtok(NULL, ";");
    if (borrower == NULL) return NULL;

    return createMedium(title, artist, borrower);
}

BOOL hasTitlePredicate(void* vmedium, int argc, va_list argv) {
    if (argc < 1 || vmedium == NULL) return FALSE;

    tMedium* medium = vmedium;
    char* ref = va_arg(argv, char*);

    return strcmp(medium->title, ref) == 0 ? TRUE : FALSE;
}

BOOL hasArtistPredicate(void* vmedium, int argc, va_list argv) {
    if (argc < 1 || vmedium == NULL) return FALSE;

    tMedium* medium = vmedium;
    char* ref = va_arg(argv, char*);

    return strcmp(medium->artist, ref) == 0 ? TRUE : FALSE;
}

BOOL hasBorrowerPredicate(void* vmedium, int argc, va_list argv) {
    if (argc < 1 || vmedium == NULL) return FALSE;

    tMedium* medium = vmedium;
    char* ref = va_arg(argv, char*);

    return strcmp(medium->borrower, ref) == 0 ? TRUE : FALSE;
}
