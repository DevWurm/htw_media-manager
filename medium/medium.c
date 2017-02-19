#include <stdlib.h>
#include <string.h>

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
