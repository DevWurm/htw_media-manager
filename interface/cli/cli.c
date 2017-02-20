#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "list/list.h"
#include "medium/medium.h"
#include "shared/shared.h"

#define BUF_SIZE 256

// @private
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
void printMediaList(tList* list) {
    if (list == NULL) return;

    tIterator* it = toIterator(list);
    tMedium* val;

    while ((val = getNext(it))) {
        printf("%s, %s, %s\n", val->title, val->artist, val->borrower);
    }
}

void runCliInterface(int argc, char* argv[], tList* data) {
    int command;

    do {
        printf("[N]ew / [E]xit: ");
        command = fgetc(stdin);
        clearBuffer(stdin); 

        switch (command) {
            case 'N': case 'n': if (!insertNewMedium(data)) return; break;
        }
    } while (command != 'e' && command != 'E' && command != EOF);
}
