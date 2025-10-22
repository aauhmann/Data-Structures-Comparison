#include "strcompare.h"
#include <ctype.h>
#include <string.h>

int strcompare(const char* str1, const char* str2) {

    const char* a = str1;
    const char* b = str2;

    while (*a != 0 && *b != 0) {
        int ca = tolower(*a);
        int cb = tolower(*b);

        if (ca != cb) return ca - cb;

        a++;
        b++;
    }

    return tolower(*a) - tolower(*b);

}