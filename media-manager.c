#include <stdlib.h>
#include <stdio.h>

#include "interface/cli/cli.h"
#include "list/list.h"
#include "medium/medium.h"

int main(int argc, char* argv[]) {
    tList* list = createList();
    if (list == NULL) {puts("Error while creating datastore"); exit(1);}

    runCliInterface(list);

    if (!deleteList(list, &deleteMediumDeleter)) {puts("Error while deleting datastore"); exit(2);}

    return 0;
}
