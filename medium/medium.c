/*
    MIT License

    Copyright (c) 2017 Leo Lindhorst

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

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

// create a new medium containing the specified information; the information get copied; in ID is generated
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

// free a medium
ERRSTATE deleteMedium(tMedium* target) {
    if (target == NULL) return ERR;

    free(target->title);
    free(target->artist);
    free(target->borrower);

    free(target);

    return OK;
}

// free amedium with deleter interface
ERRSTATE deleteMediumDeleter(void* target) {
    return deleteMedium((tMedium*) target);
}

// serializes a medium as CSV using ; as separator
char* mediumSerializer(void* vval) {
    if (vval == NULL) return NULL;

    tMedium* val = vval;

    // allocate and fill a correctly sized string for the serialization
    const int resStrLen = snprintf(NULL, 0, "%lx;%s;%s;%s", val->id, val->title, val->artist, val->borrower) + 1;
    char* res = malloc(resStrLen * sizeof(char));
    snprintf(res, resStrLen, "%lx;%s;%s;%s", val->id, val->title, val->artist, val->borrower);

    return res;
}

// deserializes a medium from CSV using ; as separator
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

// predicate which tests if a medium contains the ID, which is specified via argv
BOOL hasIdPredicate(void* vmedium, int argc, va_list argv) {
    if (argc < 1 || vmedium == NULL) return FALSE;

    tMedium* medium = vmedium;
    unsigned long ref = va_arg(argv, unsigned long);

    return medium->id == ref;
}

// predicate which tests if a medium contains the Title, which is specified via argv
BOOL hasTitlePredicate(void* vmedium, int argc, va_list argv) {
    if (argc < 1 || vmedium == NULL) return FALSE;

    tMedium* medium = vmedium;
    char* ref = va_arg(argv, char*);

    return strcmp(medium->title, ref) == 0 ? TRUE : FALSE;
}

// predicate which tests if a medium contains the Artist, which is specified via argv
BOOL hasArtistPredicate(void* vmedium, int argc, va_list argv) {
    if (argc < 1 || vmedium == NULL) return FALSE;

    tMedium* medium = vmedium;
    char* ref = va_arg(argv, char*);

    return strcmp(medium->artist, ref) == 0 ? TRUE : FALSE;
}

// predicate which tests if a medium contains the Borrower, which is specified via argv
BOOL hasBorrowerPredicate(void* vmedium, int argc, va_list argv) {
    if (argc < 1 || vmedium == NULL) return FALSE;

    tMedium* medium = vmedium;
    char* ref = va_arg(argv, char*);

    return strcmp(medium->borrower, ref) == 0 ? TRUE : FALSE;
}

// comparator for comparing two media ascendingly by title
ORD titleComperatorAsc(void* vmediuml, void* vmediumr) {
    tMedium* mediuml = vmediuml;
    tMedium* mediumr = vmediumr;

    int comp = strcmp(mediuml->title, mediumr->title);

    if (comp == 0) return EQ;
    else return comp < 0 ? LT : GT;
}

// comparator for comparing two media descendingly by title
ORD titleComperatorDesc(void* vmediuml, void* vmediumr) {
    return -1 * titleComperatorAsc(vmediuml, vmediumr);
}

// comparator for comparing two media ascendingly by artist
ORD artistComperatorAsc(void* vmediuml, void* vmediumr) {
    tMedium* mediuml = vmediuml;
    tMedium* mediumr = vmediumr;

    int comp = strcmp(mediuml->artist, mediumr->artist);

    if (comp == 0) return EQ;
    else return comp < 0 ? LT : GT;
}

// comparator for comparing two media descendingly by artist
ORD artistComperatorDesc(void* vmediuml, void* vmediumr) {
    return -1 * artistComperatorAsc(vmediuml, vmediumr);
}

// comparator for comparing two media ascendingly by borrower
ORD borrowerComperatorAsc(void* vmediuml, void* vmediumr) {
    tMedium* mediuml = vmediuml;
    tMedium* mediumr = vmediumr;

    int comp = strcmp(mediuml->borrower, mediumr->borrower);

    if (comp == 0) return EQ;
    else return comp < 0 ? LT : GT;
}

// comparator for comparing two media descendingly by borrower
ORD borrowerComperatorDesc(void* vmediuml, void* vmediumr) {
    return -1 * borrowerComperatorAsc(vmediuml, vmediumr);
}
