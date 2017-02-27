#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli.h"
#include "list/list.h"
#include "medium/medium.h"
#include "shared/shared.h"

#define BUF_SIZE 256

// @private
// insert a new medium from the terminal into the given list
ERRSTATE insertNewMedium(tList* list) {
    char title[BUF_SIZE] = "";
    char artist[BUF_SIZE] = "";
    char borrower[BUF_SIZE] = "";

    printf("Title: ");
    fgets(title, BUF_SIZE, stdin);
    title[strlen(title) - 1] = '\0';

    printf("Artist: ");
    fgets(artist, BUF_SIZE, stdin);
    artist[strlen(artist) - 1] = '\0';

    printf("Borrower: ");
    fgets(borrower, BUF_SIZE, stdin);
    borrower[strlen(borrower) - 1] = '\0';

    return insert(list, createMedium(title, artist, borrower));
}

// @private
// print the given list on the terminal
void printMediaList(tList* list) {
    if (list == NULL) return;

    tIterator* it = toIterator(list);
    tMedium* val;

    while ((val = getNext(it))) {
        printf("%s, %s, %s\n", val->title, val->artist, val->borrower);
    }
}

// run a simple command line interface on the terminal
void runCliInterface(int argc, char* argv[], tList* data) {
    int command;
    char buf[BUF_SIZE]; 
    tList* results;

    do {
        printf("[N]ew / [E]xit | [S]how All | Search by [T]itle / [A]rtist / [B]orrower: ");
        command = fgetc(stdin);
        clearBuffer(stdin); // clear the input buffer because \n is still in buffer

        switch (command) {
            case 'N': case 'n': if (!insertNewMedium(data)) return; break;
            case 'S': case 's': printMediaList(data); break;
            case 'T': case 't': 
                                printf("Title: "); fgets(buf, BUF_SIZE, stdin); buf[strlen(buf) - 1] = '\0';
                                results = getWhere(data, &hasTitlePredicate, 1, buf);
                                printMediaList(results);
                                deleteList(results, NULL);
                                break;
            case 'A': case 'a': 
                                printf("Artist: "); fgets(buf, BUF_SIZE, stdin); buf[strlen(buf) - 1] = '\0';
                                results = getWhere(data, &hasArtistPredicate, 1, buf);
                                printMediaList(results);
                                deleteList(results, NULL);
                                break;
            case 'B': case 'b': 
                                printf("Borrower: "); fgets(buf, BUF_SIZE, stdin); buf[strlen(buf) - 1] = '\0';
                                results = getWhere(data, &hasBorrowerPredicate, 1, buf);
                                printMediaList(results);
                                deleteList(results, NULL);
                                break;
        }
    } while (command != 'e' && command != 'E' && command != EOF);
}
