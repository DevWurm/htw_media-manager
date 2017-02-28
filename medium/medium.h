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

#ifndef __MEDIUM_H__
#define __MEDIUM_H__

#include <stdarg.h>

#include "shared/shared.h"

// domain model for a medium
typedef struct {
    unsigned long id;
    char* title;
    char* artist;
    char* borrower;
} tMedium;

tMedium* createMedium(const char* title, const char* artist, const char* borrower);
ERRSTATE deleteMedium(tMedium* target);
ERRSTATE deleteMediumDeleter(void* target);

char* mediumSerializer(void* val);
void* mediumDeserializer(char* val);

BOOL hasIdPredicate(void* vmedium, int argc, va_list argv);
BOOL hasTitlePredicate(void* vmedium, int argc, va_list argv);
BOOL hasArtistPredicate(void* vmedium, int argc, va_list argv);
BOOL hasBorrowerPredicate(void* vmedium, int argc, va_list argv);

ORD titleComperatorAsc(void* vmediuml, void* vmediumr);
ORD titleComperatorDesc(void* vmediuml, void* vmediumr);
ORD artistComperatorAsc(void* vmediuml, void* vmediumr);
ORD artistComperatorDesc(void* vmediuml, void* vmediumr);
ORD borrowerComperatorAsc(void* vmediuml, void* vmediumr);
ORD borrowerComperatorDesc(void* vmediuml, void* vmediumr);

#endif
