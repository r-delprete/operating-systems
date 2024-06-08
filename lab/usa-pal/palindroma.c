#include <string.h>
#include "palindroma.h"
#include "reverse.h"

int palindroma(char *str) {
    char rev_str[strlen(str) + 1];
    reverse(str, rev_str);
    return !strcmp(str, rev_str);
}