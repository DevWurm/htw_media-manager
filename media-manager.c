#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "interface/cli/cli.h"
#include "list/list.h"
#include "medium/medium.h"

void restoreMediaList(char* filename, tList* list) {
    // use file for reading
    FILE* pers = fopen(filename, "a+");
    if (pers == NULL) {fprintf(stderr, "Error while opening persistence file at %s: ERRNO %d\n", filename, errno); exit(3);}
    rewind(pers);
    if (!restoreList(list, pers, &mediumDeserializer)) {fputs("Error while restoring data from disk", stderr); exit(7);}
    if (fclose(pers) == EOF) {fputs("Error while closing persistence file", stderr); exit(4);}
}

void storeMediaList(char* filename, tList* list) {
    FILE* pers = fopen(filename, "w");
    if (pers == NULL) {fprintf(stderr, "Error while opening persistence file at %s: ERRNO %d\n", filename, errno); exit(7);}
    if (!storeList(list, pers, &mediumSerializer)) {fputs("Error while persisting data to local disk", stderr); exit(5);}
    if (fclose(pers) == EOF) {fputs("Error while closing persistence file", stderr); exit(6);}
}

int main(int argc, char* argv[]) {
    char defaultPersLocation[1024];
    snprintf(defaultPersLocation, sizeof(defaultPersLocation), "%s/%s", getenv("HOME"), ".media-manager.csv");

    tList* list = createList();
    if (list == NULL) {fputs("Error while creating datastore", stderr); exit(1);}

    restoreMediaList(defaultPersLocation, list);

    runCliInterface(list);

    storeMediaList(defaultPersLocation, list);

    if (!deleteList(list, &deleteMediumDeleter)) {fputs("Error while deleting datastore", stderr); exit(2);}

    return 0;
}
