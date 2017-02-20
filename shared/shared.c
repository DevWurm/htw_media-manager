#include <stdio.h>

#include "shared.h"

void clearBuffer(FILE* stream) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}
