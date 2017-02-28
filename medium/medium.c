#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "shared/shared.h"
#include "medium.h"

// @private
// create a new medium containing the specified information and with the specified id; the information get copied
tMedium* createMediumWithId(unsigned long id, const char* title, const char* artist, const char* borrower) {
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

    medium->id = id;

    return medium;
}

// create a new medium containing the specified information; the information get copied
tMedium* createMedium(const char* title, const char* artist, const char* borrower) {
    // For creating a new medium the current epoch time, the current 
    // clock cycle since process start and the records content is used as input
    // to the djb2 hash algorithm to generate a ID for the record,
    // because it is an easy, yet practically robust way for identifying a record in most
    // relevant cases.
    time_t currEpochTime;
    time(&currEpochTime);

    clock_t currClockTicks = clock();

    // characteristic string
    const int charStringLen = snprintf(NULL, 0, "%ld%ld%s%s%s", currEpochTime, currClockTicks, title, artist, borrower) + 1;
    char charString[charStringLen];
    snprintf(charString, charStringLen, "%ld%ld%s%s%s", currEpochTime, currClockTicks, title, artist, borrower);

    unsigned long id = djb2Hash(charString);

    return createMediumWithId(id, title, artist, borrower);
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

    const int idStrLen = snprintf(NULL, 0, "%lx", val->id) + 1;
    char idBuffer[idStrLen];
    snprintf(idBuffer, idStrLen, "%lx", val->id);

    char* res = malloc(((strlen(idBuffer) + 1) + (strlen(val->title) + 1) + (strlen(val->artist) + 1) + (strlen(val->borrower) + 1)) * sizeof(char));
    if (res == NULL) return NULL;

    sprintf(res, "%s;%s;%s;%s", idBuffer, val->title, val->artist, val->borrower);

    return res;
}

void* mediumDeserializer(char* val) {
    char* idBuffer = strtok(val, ";");
    if (idBuffer == NULL) return NULL;

    char* title = strtok(NULL, ";");
    if (title == NULL) return NULL;

    char* artist = strtok(NULL, ";");
    if (artist == NULL) return NULL;

    char* borrower = strtok(NULL, ";");
    if (borrower == NULL) return NULL;

    return createMediumWithId(strtoul(idBuffer, NULL, 16),title, artist, borrower);
}

BOOL hasIdPredicate(void* vmedium, int argc, va_list argv) {
    if (argc < 1 || vmedium == NULL) return FALSE;

    tMedium* medium = vmedium;
    unsigned long ref = va_arg(argv, unsigned long);

    return medium->id == ref;
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

ORD titleComperatorAsc(void* vmediuml, void* vmediumr) {
    tMedium* mediuml = vmediuml;
    tMedium* mediumr = vmediumr;

    int comp = strcmp(mediuml->title, mediumr->title);

    if (comp == 0) return EQ;
    else return comp < 0 ? LT : GT;
}

ORD titleComperatorDesc(void* vmediuml, void* vmediumr) {
    return -1 * titleComperatorAsc(vmediuml, vmediumr);
}

ORD artistComperatorAsc(void* vmediuml, void* vmediumr) {
    tMedium* mediuml = vmediuml;
    tMedium* mediumr = vmediumr;

    int comp = strcmp(mediuml->artist, mediumr->artist);

    if (comp == 0) return EQ;
    else return comp < 0 ? LT : GT;
}

ORD artistComperatorDesc(void* vmediuml, void* vmediumr) {
    return -1 * artistComperatorAsc(vmediuml, vmediumr);
}

ORD borrowerComperatorAsc(void* vmediuml, void* vmediumr) {
    tMedium* mediuml = vmediuml;
    tMedium* mediumr = vmediumr;

    int comp = strcmp(mediuml->borrower, mediumr->borrower);

    if (comp == 0) return EQ;
    else return comp < 0 ? LT : GT;
}

ORD borrowerComperatorDesc(void* vmediuml, void* vmediumr) {
    return -1 * borrowerComperatorAsc(vmediuml, vmediumr);
}
