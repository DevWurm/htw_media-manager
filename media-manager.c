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
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "interface/cli/cli.h"
#include "interface/libforms/libforms.h"
#include "list/list.h"
#include "medium/medium.h"

// restore the media list from the given file
void restoreMediaList(char* filename, tList* list) {
    // use file for reading and handle the case if it's not existing
    FILE* pers = fopen(filename, "a+");
    if (pers == NULL) {fprintf(stderr, "Error while opening persistence file at %s: ERRNO %d\n", filename, errno); exit(3);}
    rewind(pers);
    if (!restoreList(list, pers, &mediumDeserializer)) {fputs("Error while restoring data from disk", stderr); exit(7);}
    if (fclose(pers) == EOF) {fputs("Error while closing persistence file", stderr); exit(4);}
}

// store the media list in the given file
void storeMediaList(char* filename, tList* list) {
    FILE* pers = fopen(filename, "w");
    if (pers == NULL) {fprintf(stderr, "Error while opening persistence file at %s: ERRNO %d\n", filename, errno); exit(7);}
    if (!storeList(list, pers, &mediumSerializer)) {fputs("Error while persisting data to local disk", stderr); exit(5);}
    if (fclose(pers) == EOF) {fputs("Error while closing persistence file", stderr); exit(6);}
}

int main(int argc, char* argv[]) {
    // determine the file .media-manager.csv relative to the users home directory
    char defaultPersLocation[1024];
    snprintf(defaultPersLocation, sizeof(defaultPersLocation), "%s/%s", getenv("HOME"), ".media-manager.csv");

    // create a new list and restore the content from the given persistence location
    tList* list = createList();
    if (list == NULL) {fputs("Error while creating datastore", stderr); exit(1);}
    restoreMediaList(defaultPersLocation, list);

    // run the specified interface
    if (argc > 1 && strcmp("-c", argv[1]) == 0) {
       runCliInterface(argc, argv, list);
    } else {
        runLibformsInterface(argc, argv, list);
    }

    // store the data to the persistence location
    storeMediaList(defaultPersLocation, list);

    // remove the list
    if (!deleteList(list, &deleteMediumDeleter)) {fputs("Error while deleting datastore", stderr); exit(2);}

    return 0;
}
